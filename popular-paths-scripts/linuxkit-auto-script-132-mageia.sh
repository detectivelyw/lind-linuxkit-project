#!/bin/sh
total_num=2
current_num=1

runc_cmd="runc --root /run/containerd/runc/services.linuxkit/ exec -t"
container_task="mageia"

cmd_task_1="sleep 10"
cmd_task_2="$runc_cmd $container_task ps -ef"

echo "start running task [$current_num/$total_num] ..."
echo "CMD_TASK_1: $cmd_task_1"
eval $cmd_task_1
echo "task [$current_num/$total_num] completed."
current_num=$((current_num+1))

echo "start running task [$current_num/$total_num] ..."
echo "CMD_TASK_2: $cmd_task_2"
eval $cmd_task_2
echo "task [$current_num/$total_num] completed."
current_num=$((current_num+1))

