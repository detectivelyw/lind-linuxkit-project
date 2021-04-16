#!/bin/bash
echo > /sys/kernel/debug/tracing/trace
cat /sys/kernel/debug/tracing/trace
