#!/bin/bash
sudo python tool-count-nbb.py arch > count_nbb/arch.txt
sudo python tool-count-nbb.py fs > count_nbb/fs.txt
sudo python tool-count-nbb.py kernel > count_nbb/kernel.txt
sudo python tool-count-nbb.py mm > count_nbb/mm.txt
sudo python tool-count-nbb.py net > count_nbb/net.txt
sudo python tool-count-nbb.py drivers > count_nbb/drivers.txt
