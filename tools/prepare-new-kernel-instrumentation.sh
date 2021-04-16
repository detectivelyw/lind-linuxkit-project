#!/bin/bash
linux_kernel_src_path="/home/liyiwen/Documents/projects/tracks/linux-stable/"
global_variable_instrumentation_path="/home/liyiwen/Documents/projects/tracks/tools/global_variable_instrumentation/v4.14.24/init/main.c"

echo "start creating new git branch for kernel instrumentation..."
cd $linux_kernel_src_path
sudo git checkout my-v4.14.24
sudo git branch -D v4.14.24-test
sudo git checkout -b v4.14.24-test

echo "adding kernel global variable..."
sudo rm init/main.c
sudo cp $global_variable_instrumentation_path ./init/
sudo git add -A
sudo git commit -m "added kernel global variable in init/main.c"

echo "preparation finished successfully!"

