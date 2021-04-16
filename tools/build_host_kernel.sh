#!/bin/bash
# To run this script, we need a kernel config file first
# run sudo make menuconfig if there is no existing config file
cd /usr/src/linux-4.10/
sudo make -j4
sudo make modules -j4
sudo make modules_install
sudo make install
