
sudo docker volume create --driver local --opt type=nfs --opt o=addr=10.22.1.1,rw --opt device=:/opt/share_cluster/edc-cluster edc-nfs
