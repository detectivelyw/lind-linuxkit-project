#!/bin/bash

# usage: ./process-raw-data.sh [container-name] [start-num-data] [end-num-data]
# usage example: ./process-raw-data.sh ubuntu 10 20

program_main_path="/home/detectivelyw/Documents/projects/tracks/linuxkit-auto-experiment"
raw_data_path="$program_main_path/data"
tmp_data_path="$program_main_path/tmp"
processed_data_path="$program_main_path/processed-data"
raw_data_filename="gcda-data-$1"
gcda_data_path="$tmp_data_path/home/linux/"
linux_src_path="/home/detectivelyw/Documents/projects/tracks/linux-stable"

echo "First arg: $1"
echo "Second arg: $2"
echo "Third arg: $3"
num_current=$2
echo "We would like to process raw data of $1 container from data$2 to data$3"

while [ $num_current -le $3 ]
do
    echo "processing data$num_current [$num_current/$3] ..."
    raw_data_input="$raw_data_path/$raw_data_filename-$num_current.tar.gz"  

    if [ -f "$raw_data_input" ]
    then
        echo "now extracting raw data ..."
        tar_cmd="tar -zxvf $raw_data_input -C $tmp_data_path"
        echo $tar_cmd
        eval $tar_cmd
        echo "data extracting has finished successfully!"
        echo "now using lcov to process the raw data ..."
        lcov_output="$processed_data_path/gcov-data-$1-$num_current.txt"
        lcov_cmd="sudo lcov -d $gcda_data_path -b $linux_src_path -c -o $lcov_output"
        echo $lcov_cmd
        eval $lcov_cmd
        echo "lcov data processing has finished successfully!"
        rm_cmd="sudo rm -rf $tmp_data_path/home/"
        echo "now removing tmp files ..."
        echo $rm_cmd
        eval $rm_cmd
        echo "tmp files removed successfully!"        
    else
        echo "$raw_data_input not found."
        echo "***Warning*** missing raw data file!"
    fi
    echo "data$num_current [$num_current/$3] processing has finished successfully!"
    ((num_current++))
done
