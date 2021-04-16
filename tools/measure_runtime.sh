sudo mkdir $TOOL/time_measurement/$1
echo "start running Docker container on uninstrumented kernel..."
for i in $(seq $2 $3)
do
  echo "start running iteration $i"
  cd $TOOL
  ./kill_linuxkit.sh $1.orig
  sudo mv $KITSRC/logs/boot.log $TOOL/time_measurement/$1/un.log.$i
done

echo "start running Docker container on instrumented kernel..."
for j in $(seq $2 $3)
do
  echo "start running iteration $j"
  cd $TOOL
  ./kill_linuxkit.sh $1
  sudo mv $KITSRC/logs/boot.log $TOOL/time_measurement/$1/instrumented.log.$j
done
