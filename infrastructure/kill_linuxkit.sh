#!/bin/bash
cd ~/Documents/projects/linuxkit-v0.2-src/
echo "start running the LinuxKit VM ..."
sudo linuxkit run nginx-test > ./logs/boot.log &
sleep 40
sudo kill -9 $(ps aux | grep "[q]emu" | awk {'print $2'} | head -n 1)
sudo kill -9 $(ps aux | grep "[q]emu" | awk {'print $2'} | head -n 1)
sed -i '$d' ./logs/boot.log
echo "program has finished."
