#!/bin/bash
echo "start creating new git branch for kernel instrumentation..."
cd /home/detectivelyw/Documents/projects/tracks/linux-stable/
git checkout my-4.14.24
git branch -D v4.14.24-test
git checkout -b v4.14.24-test

echo "adding kernel global variable..."
rm init/main.c
cp ../tools/get-basic-blocks/kernel_global_variable_instrumentation/v4.14.24/init/main.c ./init/
git add -A
git commit -m "added kernel global variable in init/main.c"

echo "preparation finished successfully!"

