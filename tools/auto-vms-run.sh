#!/bin/bash
cd $KITSRC
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M arangodb.orig 2>&1 | tee logs/arangodb.orig.time
sudo time linuxkit run -mem 4096M arangodb.instrumented 2>&1 | tee logs/arangodb.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/arangodb.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M ubuntu-debootstrap.orig 2>&1 | tee logs/ubuntu-debootstrap.orig.time
sudo time linuxkit run -mem 4096M ubuntu-debootstrap.instrumented 2>&1 | tee logs/ubuntu-debootstrap.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/ubuntu-debootstrap.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M matomo.orig 2>&1 | tee logs/matomo.orig.time
sudo time linuxkit run -mem 4096M matomo.instrumented 2>&1 | tee logs/matomo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/matomo.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M rails.orig 2>&1 | tee logs/rails.orig.time
sudo time linuxkit run -mem 4096M rails.instrumented 2>&1 | tee logs/rails.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/rails.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M clojure.orig 2>&1 | tee logs/clojure.orig.time
sudo time linuxkit run -mem 4096M clojure.instrumented 2>&1 | tee logs/clojure.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/clojure.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M odoo.orig 2>&1 | tee logs/odoo.orig.time
sudo time linuxkit run -mem 4096M odoo.instrumented 2>&1 | tee logs/odoo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/odoo.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M ibmjava.orig 2>&1 | tee logs/ibmjava.orig.time
sudo time linuxkit run -mem 4096M ibmjava.instrumented 2>&1 | tee logs/ibmjava.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/ibmjava.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M julia.orig 2>&1 | tee logs/julia.orig.time
sudo time linuxkit run -mem 4096M julia.instrumented 2>&1 | tee logs/julia.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/julia.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M gcc.orig 2>&1 | tee logs/gcc.orig.time
sudo time linuxkit run -mem 4096M gcc.instrumented 2>&1 | tee logs/gcc.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/gcc.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M nuxeo.orig 2>&1 | tee logs/nuxeo.orig.time
sudo time linuxkit run -mem 4096M nuxeo.instrumented 2>&1 | tee logs/nuxeo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/nuxeo.hypercall.log
