#!/bin/bash
# usage: ./kill_linuxkit.sh [container_name]
#example: ./kill_linuxkit.sh nginx

cd $KITSRC
echo "start running the LinuxKit VM ..."
sudo linuxkit run -mem 4096M $1 > ./logs/boot.log &
sleep 100
echo "program has finished."
