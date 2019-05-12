#!/bin/bash
echo "replacing kernel source files with our instrumented files..."
python ../replace-files/replace-files.py
cd /home/detectivelyw/Documents/projects/tracks/linux-stable/
git add -A
git commit -m "added our instrumented source files."
rm $KITSRC/kernel/patches-4.14.x/*
echo "generating kernel patches..."
git format-patch -o $KITSRC/kernel/patches-4.14.x/ v4.14.24..HEAD
cd $KITSRC/kernel
sudo docker system prune --force
echo "now start recompiling the linux kernel..."
sudo make build_4.14.x

