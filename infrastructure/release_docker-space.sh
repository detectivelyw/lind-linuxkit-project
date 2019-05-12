#!/bin/bash
echo "start releasing disk space taken by Docker..."
sudo apt-get remove docker docker-engine docker-ce docker.io
sudo rm -rf /var/lib/docker/
sudo apt-get install docker-ce=18.03.0~ce-0~ubuntu
echo "disk space releasing done."
