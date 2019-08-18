#!/bin/bash
# usage: ./kill_linuxkit.sh [container_name]
#example: ./kill_linuxkit.sh nginx

cd $KITSRC
echo "start running the LinuxKit VM ..."
sudo linuxkit run $1 > ./logs/boot.log &
sleep 100
sudo kill -9 $(ps aux | grep "[q]emu" | awk {'print $2'} | head -n 1)
sudo kill -9 $(ps aux | grep "[q]emu" | awk {'print $2'} | head -n 1)
sed -i '$d' ./logs/boot.log
echo "program has finished."
