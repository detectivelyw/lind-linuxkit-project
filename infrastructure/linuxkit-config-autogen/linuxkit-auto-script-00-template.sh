#!/bin/sh
total_num=7
current_num=1

runc_cmd="runc --root /run/containerd/runc/services.linuxkit/ exec -t"
container_task="template"
container_obtain_data="ubuntu"
output_data_name="gcda-data-template.tar.gz"
host_ip="10.18.238.77"

cmd_task_1="sleep 10"
cmd_task_2="$runc_cmd $container_task ps -ef"

cmd_obtain_data_1="$runc_cmd $container_obtain_data cp -r /sys/kernel/debug/gcov/linux/ /home/"
cmd_obtain_data_2="$runc_cmd $container_obtain_data tar -zcvf /home/$output_data_name /home/linux/"
cmd_obtain_data_3="$runc_cmd $container_obtain_data apt update"
cmd_obtain_data_4="$runc_cmd $container_obtain_data apt -y install openssh-client"
cmd_obtain_data_5="$runc_cmd $container_obtain_data scp -o StrictHostKeyChecking=no /home/$output_data_name detectivelyw@$host_ip:~/Downloads/"

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

echo "start running task [$current_num/$total_num] ..."
echo "CMD_DATA_1: $cmd_obtain_data_1"
eval $cmd_obtain_data_1
echo "task [$current_num/$total_num] completed."
current_num=$((current_num+1))

echo "start running task [$current_num/$total_num] ..."
echo "CMD_DATA_2: $cmd_obtain_data_2"
eval $cmd_obtain_data_2
echo "task [$current_num/$total_num] completed."
current_num=$((current_num+1))

echo "start running task [$current_num/$total_num] ..."
echo "CMD_DATA_3: $cmd_obtain_data_3"
eval $cmd_obtain_data_3
echo "task [$current_num/$total_num] completed."
current_num=$((current_num+1))

echo "start running task [$current_num/$total_num] ..."
echo "CMD_DATA_4: $cmd_obtain_data_4"
eval $cmd_obtain_data_4
echo "task [$current_num/$total_num] completed."
current_num=$((current_num+1))

echo "start running task [$current_num/$total_num] ..."
echo "CMD_DATA_5: $cmd_obtain_data_5"
eval $cmd_obtain_data_5
echo "task [$current_num/$total_num] completed."
current_num=$((current_num+1))

