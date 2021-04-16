#!/bin/bash
cd $KITSRC
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M ubuntu.orig 2>&1 | tee logs/ubuntu.orig.time
sudo time linuxkit run -mem 4096M ubuntu.instrumented 2>&1 | tee logs/ubuntu.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/ubuntu.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M alpine.orig 2>&1 | tee logs/alpine.orig.time
sudo time linuxkit run -mem 4096M alpine.instrumented 2>&1 | tee logs/alpine.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/alpine.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M docker.orig 2>&1 | tee logs/docker.orig.time
sudo time linuxkit run -mem 4096M docker.instrumented 2>&1 | tee logs/docker.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/docker.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M busybox.orig 2>&1 | tee logs/busybox.orig.time
sudo time linuxkit run -mem 4096M busybox.instrumented 2>&1 | tee logs/busybox.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/busybox.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M hello-world.orig 2>&1 | tee logs/hello-world.orig.time
sudo time linuxkit run -mem 4096M hello-world.instrumented 2>&1 | tee logs/hello-world.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/hello-world.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M httpd.orig 2>&1 | tee logs/httpd.orig.time
sudo time linuxkit run -mem 4096M httpd.instrumented 2>&1 | tee logs/httpd.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/httpd.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M memcached.orig 2>&1 | tee logs/memcached.orig.time
sudo time linuxkit run -mem 4096M memcached.instrumented 2>&1 | tee logs/memcached.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/memcached.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M redis.orig 2>&1 | tee logs/redis.orig.time
sudo time linuxkit run -mem 4096M redis.instrumented 2>&1 | tee logs/redis.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/redis.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M nginx.orig 2>&1 | tee logs/nginx.orig.time
sudo time linuxkit run -mem 4096M nginx.instrumented 2>&1 | tee logs/nginx.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/nginx.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M mariadb.orig 2>&1 | tee logs/mariadb.orig.time
sudo time linuxkit run -mem 4096M mariadb.instrumented 2>&1 | tee logs/mariadb.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/mariadb.hypercall.log
