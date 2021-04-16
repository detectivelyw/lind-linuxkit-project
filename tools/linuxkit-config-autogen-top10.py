import sys

containers = ["ubuntu","alpine","docker","busybox","hello-world","httpd","memcached","redis","nginx","mariadb"]
container_tags = ["17.10","3.9","18","1","latest","latest","1.5.10-alpine","4.0.5-alpine","1.15.7-alpine","10.4"]

counter = 0
num = 1
for container in containers: 
  # print container_tags[counter]
  InputFileName1 = "linuxkit-auto-script-00-template.sh"
  with open(InputFileName1) as f:
    newText1=f.read().replace('template', container)
  f.close()
  
  OutputFileName1 = "/home/liyiwen/Documents/projects/tracks/popular-paths-scripts/linuxkit-auto-script-" + str(num) + "-" + container + ".sh"
  with open(OutputFileName1, "w+") as f:
    f.write(newText1)
  f.close()

  InputFileName2 = "linuxkit-instrumented-config-template.yml"
  with open(InputFileName2) as f:
    newText2=f.read().replace('template-tag', container_tags[counter])
    newText2=newText2.replace('template-script', OutputFileName1)
    newText2=newText2.replace('template', container)
  f.close()
  
  OutputFileName2 = "/home/liyiwen/Documents/projects/linuxkit-v0.2-src/" + container + ".instrumented.yml"
  with open(OutputFileName2, "w+") as f:
    f.write(newText2)
  f.close()

  InputFileName3 = "linuxkit-orig-config-template.yml"
  with open(InputFileName3) as f:
    newText3=f.read().replace('template-tag', container_tags[counter])
    newText3=newText3.replace('template-script', OutputFileName1)
    newText3=newText3.replace('template', container)
  f.close()
  
  OutputFileName3 = "/home/liyiwen/Documents/projects/linuxkit-v0.2-src/" + container + ".orig.yml"
  with open(OutputFileName3, "w+") as f:
    f.write(newText3)
  f.close()

  counter = counter + 1
  num = num + 1

OutputFileName4 = "/home/liyiwen/Documents/projects/tracks/tools/auto-build-vms.sh"
with open(OutputFileName4, "w+") as f:
  Text1 = "#!/bin/bash\nsudo chmod 777 ../popular-paths-scripts/linuxkit-auto-script-*\n"
  f.write(Text1)
  for container in containers: 
    Text2 = "cd $KITSRC\nsudo linuxkit build -disable-content-trust "+ container +".instrumented.yml\nsudo linuxkit build -disable-content-trust "+ container +".orig.yml\n"
    f.write(Text2)
f.close()

OutputFileName5 = "/home/liyiwen/Documents/projects/tracks/tools/auto-vms-run.sh"
with open(OutputFileName5, "w+") as f:
  Text1 = "#!/bin/bash\ncd $KITSRC\n"
  f.write(Text1)
  for container in containers: 
    Text2 = "echo | sudo tee /sys/kernel/debug/tracing/trace\nsudo time linuxkit run -mem 4096M "+ container +".orig 2>&1 | tee logs/"+ container +".orig.time\nsudo time linuxkit run -mem 4096M "+ container +".instrumented 2>&1 | tee logs/"+ container +".instrumented.time\nsudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/"+ container +".hypercall.log\n"
    f.write(Text2)
f.close()
