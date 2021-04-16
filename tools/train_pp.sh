
# example" ./train_pp.sh nginx 5 100

# For the first iteration, make sure that we have the most updated kernel source compiled and used 
# cd $TOOL
# ./recompile-kernel.sh
# cd $KITSRC
# sudo linuxkit build -disable-content-trust $1.yml

# Now start our iteration
for i in $(seq $2 $3) 
do
  echo "start running training iteration $(($i-$2+1))/$(($3-$2+1)) ..."
  cd $TOOL
  ./kill_linuxkit.sh $1
  cd $TOOL
  sudo mv $KITSRC/logs/boot.log ./boot-log/
  sudo python process-boot-log.py
  sudo python process-blacklist.py
  sudo mv ./boot-log/boot.log ./boot-log-processed/boot.log.$i
  sudo mv ./blacklist/blacklist.txt ./blacklist-processed/blacklist.txt.$i
  ./recompile-kernel.sh
  cd $KITSRC
  sudo linuxkit build -disable-content-trust $1.yml
  echo "iteration $(($i-$2+1))/$(($3-$2+1)) has finished."
done
echo "training job has finished!"
