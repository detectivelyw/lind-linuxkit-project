import sys

linuxkit_traces_dir = '/home/detectivelyw/Documents/projects/tracks/linuxkit-auto-experiment/gcov-parsed-data/'  

containers = ["busybox", "telegraf", "amazoncorretto", "consul", "sourcemage", "crate", "kong", "mediawiki", "mongo-express", "mysql", "piwik", "alt", "ros", "buildpack-deps", "hello-world", "geonetwork", "nats", "ubuntu-debootstrap", "celery", "neurodebian", "glassfish", "xwiki", "erlang", "cirros", "postgres", "teamspeak", "python", "flink", "opensuse", "nginx", "pypy", "alpine", "couchbase", "gazebo", "rabbitmq", "gradle", "orientdb", "eggdrop", "chronograf", "fsharp", "centos", "hipache", "zookeeper", "kibana", "rapidoid", "websphere-liberty", "redis", "euleros", "lightstreamer", "rocket.chat", "memcached", "kapacitor", "influxdb", "maven", "odoo", "notary", "swift", "logstash", "bash", "thrift", "haproxy", "drupal", "mono", "joomla", "adminer", "elasticsearch", "sonarqube", "vault", "convertigo", "php-zendserver", "amazonlinux", "rakudo-star", "yourls", "spiped", "traefik", "elixir", "irssi", "julia", "jruby", "haskell", "composer", "neo4j", "bonita", "perl", "plone", "eclipse-mosquitto", "httpd", "swipl", "mariadb", "java", "hello-seattle", "solr", "jetty", "rails", "clearlinux", "swarm", "redmine", "wordpress", "storm", "haxe", "ubuntu-upstart", "ghost", "tomee", "photon", "r-base", "known", "ruby", "nextcloud", "groovy", "openjdk", "iojs", "ibmjava", "rethinkdb", "mongo", "express-gateway", "golang", "mageia", "owncloud", "open-liberty", "cassandra", "gcc", "hola-mundo", "fedora", "matomo", "couchdb", "arangodb", "postfixadmin", "jenkins", "hylang", "node", "percona", "registry", "php", "debian", "znc", "clojure", "tomcat", "nats-streaming", "django", "sl", "docker", "backdrop"]

total_kernel_trace_set = set()

for container in containers: 
  for i in range(1, 11, 1):
    kernel_trace_file_name = linuxkit_traces_dir + "gcov-data-" + container + "-parsed-" + str(i) + ".txt"
    with open(kernel_trace_file_name) as kernel_trace_file:  
      trace_line = kernel_trace_file.readline()
      while trace_line:
        # print trace_line  
        total_kernel_trace_set.add(trace_line)
        trace_line = kernel_trace_file.readline()
    kernel_trace_file.close()
    # print trace_set
      
print len(total_kernel_trace_set)

OutputFileName = "total-pp.txt"
with open(OutputFileName, "w") as f:
  for line in total_kernel_trace_set: 
    f.write(line)
f.close()

