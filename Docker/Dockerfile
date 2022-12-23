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
    iputils-ping \
    psmisc \
    iproute2 \
    libyaml-cpp-dev\
    unixodbc \
    python3-pip\
    vim\
    &&  apt-get clean \
    && rm -rf /var/lib/apt/lists/* 


RUN mkdir /var/run/sshd && service ssh start
RUN pip3 install pyyaml
RUN git clone https://github.com/jemalloc/jemalloc.git && cd jemalloc && ./autogen.sh && make -j 8 && make install && cd ..

# SSH login fix. Otherwise user is kicked off after login
RUN chmod 700 /root/ && sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd \
    && mkdir -p ~/.ssh && ssh-keygen -t rsa -N '' -f ~/.ssh/id_rsa && cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys \
    && ssh-keyscan -H 127.0.0.1 >> ~/.ssh/known_hosts && chmod 700 /root/.ssh/ && chmod 600 /root/.ssh/*

COPY ./OLTP/dast/requirements.txt /root/
COPY ./ /htap

RUN cd /root && pip install --upgrade "pip < 21.0" && pip install -r /root/requirements.txt
