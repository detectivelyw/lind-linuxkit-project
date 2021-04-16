#!/bin/bash
echo "start instrumenting the Linux kernel ..."
sudo python tool.py arch
sudo python tool.py block
sudo python tool.py crypto
sudo python tool.py drivers
sudo python tool.py fs
sudo python tool.py ipc
sudo python tool.py kernel
sudo python tool.py mm
sudo python tool.py net
sudo python tool.py security
echo "kernel instrumentation finished!"
