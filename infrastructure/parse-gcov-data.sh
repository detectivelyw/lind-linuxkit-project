#!/bin/bash

# usage: ./parse-gcov-data.sh [container-name] [start-num] [end-number]
# usage example: ./parse-gcov-data.sh ubuntu 5 20

input="gcov-data-$1"
output="gcov-data-$1-parsed"
program_main_path="/home/detectivelyw/Documents/projects/tracks/linuxkit-auto-experiment"
processed_data_path="$program_main_path/processed-data"
gcov_parsed_data_path="$program_main_path/gcov-parsed-data"
parse_program="process-gcov-results-v3.py"

echo "First arg: $1"
echo "Second arg: $2"
echo "Third arg: $3"

num_current=$2

while [ $num_current -le $3 ]
do
    echo "working on iteration num $num_current ..."
    current_filename="$input-$num_current.txt"
    output_filename="$output-$num_current.txt"

    parse_cmd="python $parse_program -i $processed_data_path/$current_filename -o $gcov_parsed_data_path/$output_filename"
    echo $parse_cmd
    eval $parse_cmd
    echo "iteration num $num_current has been processed!"
    ((num_current++))
done
