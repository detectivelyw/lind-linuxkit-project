#!/bin/bash
echo | sudo tee /sys/kernel/debug/tracing/trace
cd $KITSRC
sudo time linuxkit run -mem 4096M ubuntu.instrumented 2>&1 | tee logs/ubuntu.instrumented.time.test
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/test.txt
