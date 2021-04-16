#!/bin/bash
sudo chmod 777 ../popular-paths-scripts/linuxkit-auto-script-*
cd $KITSRC
sudo linuxkit build -disable-content-trust ubuntu.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust alpine.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust docker.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust busybox.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust hello-world.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust httpd.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust memcached.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust redis.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust nginx.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust mariadb.yml
