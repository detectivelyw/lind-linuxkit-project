#!/bin/bash
cd $KITSRC
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M couchbase.orig 2>&1 | tee logs/couchbase.orig.time
sudo time linuxkit run -mem 4096M couchbase.instrumented 2>&1 | tee logs/couchbase.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/couchbase.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M crate.orig 2>&1 | tee logs/crate.orig.time
sudo time linuxkit run -mem 4096M crate.instrumented 2>&1 | tee logs/crate.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/crate.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M percona.orig 2>&1 | tee logs/percona.orig.time
sudo time linuxkit run -mem 4096M percona.instrumented 2>&1 | tee logs/percona.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/percona.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M rocket.chat.orig 2>&1 | tee logs/rocket.chat.orig.time
sudo time linuxkit run -mem 4096M rocket.chat.instrumented 2>&1 | tee logs/rocket.chat.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/rocket.chat.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M jruby.orig 2>&1 | tee logs/jruby.orig.time
sudo time linuxkit run -mem 4096M jruby.instrumented 2>&1 | tee logs/jruby.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/jruby.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M groovy.orig 2>&1 | tee logs/groovy.orig.time
sudo time linuxkit run -mem 4096M groovy.instrumented 2>&1 | tee logs/groovy.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/groovy.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M nats-streaming.orig 2>&1 | tee logs/nats-streaming.orig.time
sudo time linuxkit run -mem 4096M nats-streaming.instrumented 2>&1 | tee logs/nats-streaming.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/nats-streaming.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M couchdb.orig 2>&1 | tee logs/couchdb.orig.time
sudo time linuxkit run -mem 4096M couchdb.instrumented 2>&1 | tee logs/couchdb.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/couchdb.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M chronograf.orig 2>&1 | tee logs/chronograf.orig.time
sudo time linuxkit run -mem 4096M chronograf.instrumented 2>&1 | tee logs/chronograf.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/chronograf.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M eclipse-mosquitto.orig 2>&1 | tee logs/eclipse-mosquitto.orig.time
sudo time linuxkit run -mem 4096M eclipse-mosquitto.instrumented 2>&1 | tee logs/eclipse-mosquitto.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/eclipse-mosquitto.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M buildpack-deps.orig 2>&1 | tee logs/buildpack-deps.orig.time
sudo time linuxkit run -mem 4096M buildpack-deps.instrumented 2>&1 | tee logs/buildpack-deps.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/buildpack-deps.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M jetty.orig 2>&1 | tee logs/jetty.orig.time
sudo time linuxkit run -mem 4096M jetty.instrumented 2>&1 | tee logs/jetty.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/jetty.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M bash.orig 2>&1 | tee logs/bash.orig.time
sudo time linuxkit run -mem 4096M bash.instrumented 2>&1 | tee logs/bash.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/bash.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M joomla.orig 2>&1 | tee logs/joomla.orig.time
sudo time linuxkit run -mem 4096M joomla.instrumented 2>&1 | tee logs/joomla.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/joomla.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M perl.orig 2>&1 | tee logs/perl.orig.time
sudo time linuxkit run -mem 4096M perl.instrumented 2>&1 | tee logs/perl.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/perl.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M iojs.orig 2>&1 | tee logs/iojs.orig.time
sudo time linuxkit run -mem 4096M iojs.instrumented 2>&1 | tee logs/iojs.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/iojs.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M solr.orig 2>&1 | tee logs/solr.orig.time
sudo time linuxkit run -mem 4096M solr.instrumented 2>&1 | tee logs/solr.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/solr.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M amazonlinux.orig 2>&1 | tee logs/amazonlinux.orig.time
sudo time linuxkit run -mem 4096M amazonlinux.instrumented 2>&1 | tee logs/amazonlinux.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/amazonlinux.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M adminer.orig 2>&1 | tee logs/adminer.orig.time
sudo time linuxkit run -mem 4096M adminer.instrumented 2>&1 | tee logs/adminer.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/adminer.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M elixir.orig 2>&1 | tee logs/elixir.orig.time
sudo time linuxkit run -mem 4096M elixir.instrumented 2>&1 | tee logs/elixir.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/elixir.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M pypy.orig 2>&1 | tee logs/pypy.orig.time
sudo time linuxkit run -mem 4096M pypy.instrumented 2>&1 | tee logs/pypy.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/pypy.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M haxe.orig 2>&1 | tee logs/haxe.orig.time
sudo time linuxkit run -mem 4096M haxe.instrumented 2>&1 | tee logs/haxe.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/haxe.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M rethinkdb.orig 2>&1 | tee logs/rethinkdb.orig.time
sudo time linuxkit run -mem 4096M rethinkdb.instrumented 2>&1 | tee logs/rethinkdb.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/rethinkdb.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M flink.orig 2>&1 | tee logs/flink.orig.time
sudo time linuxkit run -mem 4096M flink.instrumented 2>&1 | tee logs/flink.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/flink.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M teamspeak.orig 2>&1 | tee logs/teamspeak.orig.time
sudo time linuxkit run -mem 4096M teamspeak.instrumented 2>&1 | tee logs/teamspeak.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/teamspeak.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M redmine.orig 2>&1 | tee logs/redmine.orig.time
sudo time linuxkit run -mem 4096M redmine.instrumented 2>&1 | tee logs/redmine.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/redmine.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M composer.orig 2>&1 | tee logs/composer.orig.time
sudo time linuxkit run -mem 4096M composer.instrumented 2>&1 | tee logs/composer.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/composer.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M xwiki.orig 2>&1 | tee logs/xwiki.orig.time
sudo time linuxkit run -mem 4096M xwiki.instrumented 2>&1 | tee logs/xwiki.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/xwiki.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M websphere-liberty.orig 2>&1 | tee logs/websphere-liberty.orig.time
sudo time linuxkit run -mem 4096M websphere-liberty.instrumented 2>&1 | tee logs/websphere-liberty.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/websphere-liberty.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M opensuse.orig 2>&1 | tee logs/opensuse.orig.time
sudo time linuxkit run -mem 4096M opensuse.instrumented 2>&1 | tee logs/opensuse.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/opensuse.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M mono.orig 2>&1 | tee logs/mono.orig.time
sudo time linuxkit run -mem 4096M mono.instrumented 2>&1 | tee logs/mono.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/mono.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M orientdb.orig 2>&1 | tee logs/orientdb.orig.time
sudo time linuxkit run -mem 4096M orientdb.instrumented 2>&1 | tee logs/orientdb.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/orientdb.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M django.orig 2>&1 | tee logs/django.orig.time
sudo time linuxkit run -mem 4096M django.instrumented 2>&1 | tee logs/django.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/django.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M mediawiki.orig 2>&1 | tee logs/mediawiki.orig.time
sudo time linuxkit run -mem 4096M mediawiki.instrumented 2>&1 | tee logs/mediawiki.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/mediawiki.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M znc.orig 2>&1 | tee logs/znc.orig.time
sudo time linuxkit run -mem 4096M znc.instrumented 2>&1 | tee logs/znc.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/znc.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M piwik.orig 2>&1 | tee logs/piwik.orig.time
sudo time linuxkit run -mem 4096M piwik.instrumented 2>&1 | tee logs/piwik.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/piwik.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M ros.orig 2>&1 | tee logs/ros.orig.time
sudo time linuxkit run -mem 4096M ros.instrumented 2>&1 | tee logs/ros.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/ros.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M swift.orig 2>&1 | tee logs/swift.orig.time
sudo time linuxkit run -mem 4096M swift.instrumented 2>&1 | tee logs/swift.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/swift.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M rust.orig 2>&1 | tee logs/rust.orig.time
sudo time linuxkit run -mem 4096M rust.instrumented 2>&1 | tee logs/rust.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/rust.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M oraclelinux.orig 2>&1 | tee logs/oraclelinux.orig.time
sudo time linuxkit run -mem 4096M oraclelinux.instrumented 2>&1 | tee logs/oraclelinux.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/oraclelinux.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M arangodb.orig 2>&1 | tee logs/arangodb.orig.time
sudo time linuxkit run -mem 4096M arangodb.instrumented 2>&1 | tee logs/arangodb.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/arangodb.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M ubuntu-debootstrap.orig 2>&1 | tee logs/ubuntu-debootstrap.orig.time
sudo time linuxkit run -mem 4096M ubuntu-debootstrap.instrumented 2>&1 | tee logs/ubuntu-debootstrap.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/ubuntu-debootstrap.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M matomo.orig 2>&1 | tee logs/matomo.orig.time
sudo time linuxkit run -mem 4096M matomo.instrumented 2>&1 | tee logs/matomo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/matomo.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M rails.orig 2>&1 | tee logs/rails.orig.time
sudo time linuxkit run -mem 4096M rails.instrumented 2>&1 | tee logs/rails.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/rails.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M clojure.orig 2>&1 | tee logs/clojure.orig.time
sudo time linuxkit run -mem 4096M clojure.instrumented 2>&1 | tee logs/clojure.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/clojure.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M odoo.orig 2>&1 | tee logs/odoo.orig.time
sudo time linuxkit run -mem 4096M odoo.instrumented 2>&1 | tee logs/odoo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/odoo.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M ibmjava.orig 2>&1 | tee logs/ibmjava.orig.time
sudo time linuxkit run -mem 4096M ibmjava.instrumented 2>&1 | tee logs/ibmjava.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/ibmjava.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M julia.orig 2>&1 | tee logs/julia.orig.time
sudo time linuxkit run -mem 4096M julia.instrumented 2>&1 | tee logs/julia.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/julia.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M gcc.orig 2>&1 | tee logs/gcc.orig.time
sudo time linuxkit run -mem 4096M gcc.instrumented 2>&1 | tee logs/gcc.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/gcc.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M nuxeo.orig 2>&1 | tee logs/nuxeo.orig.time
sudo time linuxkit run -mem 4096M nuxeo.instrumented 2>&1 | tee logs/nuxeo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/nuxeo.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M notary.orig 2>&1 | tee logs/notary.orig.time
sudo time linuxkit run -mem 4096M notary.instrumented 2>&1 | tee logs/notary.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/notary.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M storm.orig 2>&1 | tee logs/storm.orig.time
sudo time linuxkit run -mem 4096M storm.instrumented 2>&1 | tee logs/storm.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/storm.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M gazebo.orig 2>&1 | tee logs/gazebo.orig.time
sudo time linuxkit run -mem 4096M gazebo.instrumented 2>&1 | tee logs/gazebo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/gazebo.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M irssi.orig 2>&1 | tee logs/irssi.orig.time
sudo time linuxkit run -mem 4096M irssi.instrumented 2>&1 | tee logs/irssi.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/irssi.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M php-zendserver.orig 2>&1 | tee logs/php-zendserver.orig.time
sudo time linuxkit run -mem 4096M php-zendserver.instrumented 2>&1 | tee logs/php-zendserver.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/php-zendserver.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M plone.orig 2>&1 | tee logs/plone.orig.time
sudo time linuxkit run -mem 4096M plone.instrumented 2>&1 | tee logs/plone.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/plone.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M celery.orig 2>&1 | tee logs/celery.orig.time
sudo time linuxkit run -mem 4096M celery.instrumented 2>&1 | tee logs/celery.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/celery.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M bonita.orig 2>&1 | tee logs/bonita.orig.time
sudo time linuxkit run -mem 4096M bonita.instrumented 2>&1 | tee logs/bonita.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/bonita.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M open-liberty.orig 2>&1 | tee logs/open-liberty.orig.time
sudo time linuxkit run -mem 4096M open-liberty.instrumented 2>&1 | tee logs/open-liberty.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/open-liberty.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M cirros.orig 2>&1 | tee logs/cirros.orig.time
sudo time linuxkit run -mem 4096M cirros.instrumented 2>&1 | tee logs/cirros.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/cirros.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M geonetwork.orig 2>&1 | tee logs/geonetwork.orig.time
sudo time linuxkit run -mem 4096M geonetwork.instrumented 2>&1 | tee logs/geonetwork.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/geonetwork.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M haskell.orig 2>&1 | tee logs/haskell.orig.time
sudo time linuxkit run -mem 4096M haskell.instrumented 2>&1 | tee logs/haskell.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/haskell.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M lightstreamer.orig 2>&1 | tee logs/lightstreamer.orig.time
sudo time linuxkit run -mem 4096M lightstreamer.instrumented 2>&1 | tee logs/lightstreamer.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/lightstreamer.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M hello-seattle.orig 2>&1 | tee logs/hello-seattle.orig.time
sudo time linuxkit run -mem 4096M hello-seattle.instrumented 2>&1 | tee logs/hello-seattle.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/hello-seattle.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M hylang.orig 2>&1 | tee logs/hylang.orig.time
sudo time linuxkit run -mem 4096M hylang.instrumented 2>&1 | tee logs/hylang.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/hylang.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M swipl.orig 2>&1 | tee logs/swipl.orig.time
sudo time linuxkit run -mem 4096M swipl.instrumented 2>&1 | tee logs/swipl.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/swipl.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M rakudo-star.orig 2>&1 | tee logs/rakudo-star.orig.time
sudo time linuxkit run -mem 4096M rakudo-star.instrumented 2>&1 | tee logs/rakudo-star.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/rakudo-star.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M r-base.orig 2>&1 | tee logs/r-base.orig.time
sudo time linuxkit run -mem 4096M r-base.instrumented 2>&1 | tee logs/r-base.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/r-base.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M neurodebian.orig 2>&1 | tee logs/neurodebian.orig.time
sudo time linuxkit run -mem 4096M neurodebian.instrumented 2>&1 | tee logs/neurodebian.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/neurodebian.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M backdrop.orig 2>&1 | tee logs/backdrop.orig.time
sudo time linuxkit run -mem 4096M backdrop.instrumented 2>&1 | tee logs/backdrop.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/backdrop.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M convertigo.orig 2>&1 | tee logs/convertigo.orig.time
sudo time linuxkit run -mem 4096M convertigo.instrumented 2>&1 | tee logs/convertigo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/convertigo.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M thrift.orig 2>&1 | tee logs/thrift.orig.time
sudo time linuxkit run -mem 4096M thrift.instrumented 2>&1 | tee logs/thrift.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/thrift.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M fsharp.orig 2>&1 | tee logs/fsharp.orig.time
sudo time linuxkit run -mem 4096M fsharp.instrumented 2>&1 | tee logs/fsharp.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/fsharp.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M eggdrop.orig 2>&1 | tee logs/eggdrop.orig.time
sudo time linuxkit run -mem 4096M eggdrop.instrumented 2>&1 | tee logs/eggdrop.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/eggdrop.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M docker-dev.orig 2>&1 | tee logs/docker-dev.orig.time
sudo time linuxkit run -mem 4096M docker-dev.instrumented 2>&1 | tee logs/docker-dev.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/docker-dev.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M ubuntu-upstart.orig 2>&1 | tee logs/ubuntu-upstart.orig.time
sudo time linuxkit run -mem 4096M ubuntu-upstart.instrumented 2>&1 | tee logs/ubuntu-upstart.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/ubuntu-upstart.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M rapidoid.orig 2>&1 | tee logs/rapidoid.orig.time
sudo time linuxkit run -mem 4096M rapidoid.instrumented 2>&1 | tee logs/rapidoid.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/rapidoid.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M photon.orig 2>&1 | tee logs/photon.orig.time
sudo time linuxkit run -mem 4096M photon.instrumented 2>&1 | tee logs/photon.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/photon.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M glassfish.orig 2>&1 | tee logs/glassfish.orig.time
sudo time linuxkit run -mem 4096M glassfish.instrumented 2>&1 | tee logs/glassfish.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/glassfish.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M spiped.orig 2>&1 | tee logs/spiped.orig.time
sudo time linuxkit run -mem 4096M spiped.instrumented 2>&1 | tee logs/spiped.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/spiped.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M yourls.orig 2>&1 | tee logs/yourls.orig.time
sudo time linuxkit run -mem 4096M yourls.instrumented 2>&1 | tee logs/yourls.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/yourls.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M mageia.orig 2>&1 | tee logs/mageia.orig.time
sudo time linuxkit run -mem 4096M mageia.instrumented 2>&1 | tee logs/mageia.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/mageia.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M hola-mundo.orig 2>&1 | tee logs/hola-mundo.orig.time
sudo time linuxkit run -mem 4096M hola-mundo.instrumented 2>&1 | tee logs/hola-mundo.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/hola-mundo.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M silverpeas.orig 2>&1 | tee logs/silverpeas.orig.time
sudo time linuxkit run -mem 4096M silverpeas.instrumented 2>&1 | tee logs/silverpeas.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/silverpeas.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M sourcemage.orig 2>&1 | tee logs/sourcemage.orig.time
sudo time linuxkit run -mem 4096M sourcemage.instrumented 2>&1 | tee logs/sourcemage.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/sourcemage.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M known.orig 2>&1 | tee logs/known.orig.time
sudo time linuxkit run -mem 4096M known.instrumented 2>&1 | tee logs/known.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/known.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M crux.orig 2>&1 | tee logs/crux.orig.time
sudo time linuxkit run -mem 4096M crux.instrumented 2>&1 | tee logs/crux.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/crux.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M sl.orig 2>&1 | tee logs/sl.orig.time
sudo time linuxkit run -mem 4096M sl.instrumented 2>&1 | tee logs/sl.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/sl.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M clefos.orig 2>&1 | tee logs/clefos.orig.time
sudo time linuxkit run -mem 4096M clefos.instrumented 2>&1 | tee logs/clefos.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/clefos.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M euleros.orig 2>&1 | tee logs/euleros.orig.time
sudo time linuxkit run -mem 4096M euleros.instrumented 2>&1 | tee logs/euleros.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/euleros.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M hipache.orig 2>&1 | tee logs/hipache.orig.time
sudo time linuxkit run -mem 4096M hipache.instrumented 2>&1 | tee logs/hipache.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/hipache.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M clearlinux.orig 2>&1 | tee logs/clearlinux.orig.time
sudo time linuxkit run -mem 4096M clearlinux.instrumented 2>&1 | tee logs/clearlinux.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/clearlinux.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M amazoncorretto.orig 2>&1 | tee logs/amazoncorretto.orig.time
sudo time linuxkit run -mem 4096M amazoncorretto.instrumented 2>&1 | tee logs/amazoncorretto.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/amazoncorretto.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M alt.orig 2>&1 | tee logs/alt.orig.time
sudo time linuxkit run -mem 4096M alt.instrumented 2>&1 | tee logs/alt.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/alt.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M postfixadmin.orig 2>&1 | tee logs/postfixadmin.orig.time
sudo time linuxkit run -mem 4096M postfixadmin.instrumented 2>&1 | tee logs/postfixadmin.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/postfixadmin.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M express-gateway.orig 2>&1 | tee logs/express-gateway.orig.time
sudo time linuxkit run -mem 4096M express-gateway.instrumented 2>&1 | tee logs/express-gateway.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/express-gateway.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M zookeeper.orig 2>&1 | tee logs/zookeeper.orig.time
sudo time linuxkit run -mem 4096M zookeeper.instrumented 2>&1 | tee logs/zookeeper.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/zookeeper.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M erlang.orig 2>&1 | tee logs/erlang.orig.time
sudo time linuxkit run -mem 4096M erlang.instrumented 2>&1 | tee logs/erlang.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/erlang.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M kapacitor.orig 2>&1 | tee logs/kapacitor.orig.time
sudo time linuxkit run -mem 4096M kapacitor.instrumented 2>&1 | tee logs/kapacitor.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/kapacitor.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M tomee.orig 2>&1 | tee logs/tomee.orig.time
sudo time linuxkit run -mem 4096M tomee.instrumented 2>&1 | tee logs/tomee.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/tomee.hypercall.log
echo | sudo tee /sys/kernel/debug/tracing/trace
sudo time linuxkit run -mem 4096M mongo-express.orig 2>&1 | tee logs/mongo-express.orig.time
sudo time linuxkit run -mem 4096M mongo-express.instrumented 2>&1 | tee logs/mongo-express.instrumented.time
sudo cat /sys/kernel/debug/tracing/trace > $TOOL/hypercall_logs/mongo-express.hypercall.log
