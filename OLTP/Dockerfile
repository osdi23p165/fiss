FROM mxschen/grpc-glog:latest

# setup ssh server
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    openssh-server \
    git \
    pkg-config \
    build-essential \
    clang \
    libapr1-dev libaprutil1-dev \
    libboost-all-dev \
    libyaml-cpp-dev \
    python-dev \
    python-pip \
    python-tk\
    libgoogle-perftools-dev \
    net-tools \
    python3-pip \
    iputils-ping \
    psmisc \
    iproute2 \
    &&  apt-get clean \
    && rm -rf /var/lib/apt/lists/*


RUN mkdir /var/run/sshd && service ssh start

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd \
    && mkdir -p ~/.ssh && ssh-keygen -t rsa -N '' -f ~/.ssh/id_rsa && cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys \
    && ssh-keyscan -H 127.0.0.1 >> ~/.ssh/known_hosts

COPY ./OLTP/dast/requirements.txt /root/
COPY ./ /htap

RUN pip3 install pyyaml
RUN cd /root && pip install --upgrade "pip < 21.0" && pip install -r /root/requirements.txt

# No need to build
# RUN cd /htap/OLTP && mkdir -p build && cd build && cmake .. && make -j
