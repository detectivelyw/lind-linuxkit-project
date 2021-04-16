import sys

containers = ["couchbase", "crate", "percona", "rocket.chat", "jruby", "groovy", "nats-streaming", "couchdb", "chronograf", "eclipse-mosquitto", "buildpack-deps", "jetty", "bash", "joomla", "perl", "iojs", "solr", "amazonlinux", "adminer", "elixir", "pypy", "haxe", "rethinkdb", "flink", "teamspeak", "redmine", "composer", "xwiki", "websphere-liberty", "opensuse", "mono", "orientdb", "django", "mediawiki", "znc", "piwik", "ros", "swift", "rust", "oraclelinux", "arangodb", "ubuntu-debootstrap", "matomo", "rails", "clojure", "odoo", "ibmjava", "julia", "gcc", "nuxeo", "notary", "storm", "gazebo", "irssi", "php-zendserver", "plone", "celery", "bonita", "open-liberty", "cirros", "geonetwork", "haskell", "lightstreamer", "hello-seattle", "hylang", "swipl", "rakudo-star", "r-base", "neurodebian", "backdrop", "convertigo", "thrift", "fsharp", "eggdrop", "docker-dev", "ubuntu-upstart", "rapidoid", "photon", "glassfish", "spiped", "yourls", "mageia", "hola-mundo", "silverpeas", "sourcemage", "known", "crux", "sl", "clefos", "euleros", "hipache", "clearlinux", "amazoncorretto", "alt", "postfixadmin", "express-gateway", "zookeeper", "erlang", "kapacitor", "tomee", "mongo-express"]
container_tags = ["6.0.1", "3.2", "8", "0.74", "9", "3.0", "0.12.0-linux", "2", "1.7.8", "1.5", "18.04", "9.4", "5", "3.9", "5", "1.8", "7.7", "2", "4.7", "1.8", "2.7", "3.4", "2.3", "1.6", "3.6", "4.0", "1.8", "10", "19.0.0.2-kernel", "42.3", "5.18", "2.0.18", "1.10", "1.32", "1.7", "3.6", "indigo-ros-core", "4.2", "1.33", "7.6", "2.8", "14.04", "3.8", "5", "openjdk-8-lein", "12", "8", "1.1-stretch", "8", "10", "server-0.6.1-1", "1.2", "gzserver7", "1.2", "9.1", "5", "4", "7.8", "kernel", "0.4", "3.6", "8.6", "7", "linux", "0.13", "8.1.2", "2018.10", "3.5.2", "trusty", "1.10", "7.5", "0.12.0", "10", "1.8", "1.9", "14.04", "5.4.6", "3.0", "4.0", "1.6", "1.7", "6", "linux", "6.0.1", "0.62", "0.9", "3.4", "7", "7", "2.2", "0.3.1", "base", "8", "p8", "3.2", "1.15.0", "3.5", "21", "1.5", "8-jre-1.7.5-jaxrs", "0.49"]

counter = 0
num = 51
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
