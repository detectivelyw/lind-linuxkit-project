#!/bin/bash
sudo chmod 777 ../popular-paths-scripts/linuxkit-auto-script-*
cd $KITSRC
sudo linuxkit build -disable-content-trust ubuntu.instrumented.yml
sudo linuxkit build -disable-content-trust ubuntu.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust alpine.instrumented.yml
sudo linuxkit build -disable-content-trust alpine.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust docker.instrumented.yml
sudo linuxkit build -disable-content-trust docker.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust busybox.instrumented.yml
sudo linuxkit build -disable-content-trust busybox.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust hello-world.instrumented.yml
sudo linuxkit build -disable-content-trust hello-world.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust httpd.instrumented.yml
sudo linuxkit build -disable-content-trust httpd.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust memcached.instrumented.yml
sudo linuxkit build -disable-content-trust memcached.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust redis.instrumented.yml
sudo linuxkit build -disable-content-trust redis.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust nginx.instrumented.yml
sudo linuxkit build -disable-content-trust nginx.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust mariadb.instrumented.yml
sudo linuxkit build -disable-content-trust mariadb.orig.yml
