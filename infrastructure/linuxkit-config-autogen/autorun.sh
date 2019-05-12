#!/bin/bash
cd $KITSRC
sudo linuxkit build -disable-content-trust sl-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh sl 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust clefos-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh clefos 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust euleros-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh euleros 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust hipache-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh hipache 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust clearlinux-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh clearlinux 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust amazoncorretto-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh amazoncorretto 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust alt-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh alt 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust postfixadmin-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh postfixadmin 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust express-gateway-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh express-gateway 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust zookeeper-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh zookeeper 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust erlang-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh erlang 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust kapacitor-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh kapacitor 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust tomee-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh tomee 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust mongo-express-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh mongo-express 1 10
