#!/bin/bash
sudo chmod 777 ../popular-paths-scripts/linuxkit-auto-script-*
cd $KITSRC
sudo linuxkit build -disable-content-trust couchbase.instrumented.yml
sudo linuxkit build -disable-content-trust couchbase.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust crate.instrumented.yml
sudo linuxkit build -disable-content-trust crate.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust percona.instrumented.yml
sudo linuxkit build -disable-content-trust percona.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust rocket.chat.instrumented.yml
sudo linuxkit build -disable-content-trust rocket.chat.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust jruby.instrumented.yml
sudo linuxkit build -disable-content-trust jruby.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust groovy.instrumented.yml
sudo linuxkit build -disable-content-trust groovy.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust nats-streaming.instrumented.yml
sudo linuxkit build -disable-content-trust nats-streaming.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust couchdb.instrumented.yml
sudo linuxkit build -disable-content-trust couchdb.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust chronograf.instrumented.yml
sudo linuxkit build -disable-content-trust chronograf.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust eclipse-mosquitto.instrumented.yml
sudo linuxkit build -disable-content-trust eclipse-mosquitto.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust buildpack-deps.instrumented.yml
sudo linuxkit build -disable-content-trust buildpack-deps.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust jetty.instrumented.yml
sudo linuxkit build -disable-content-trust jetty.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust bash.instrumented.yml
sudo linuxkit build -disable-content-trust bash.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust joomla.instrumented.yml
sudo linuxkit build -disable-content-trust joomla.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust perl.instrumented.yml
sudo linuxkit build -disable-content-trust perl.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust iojs.instrumented.yml
sudo linuxkit build -disable-content-trust iojs.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust solr.instrumented.yml
sudo linuxkit build -disable-content-trust solr.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust amazonlinux.instrumented.yml
sudo linuxkit build -disable-content-trust amazonlinux.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust adminer.instrumented.yml
sudo linuxkit build -disable-content-trust adminer.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust elixir.instrumented.yml
sudo linuxkit build -disable-content-trust elixir.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust pypy.instrumented.yml
sudo linuxkit build -disable-content-trust pypy.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust haxe.instrumented.yml
sudo linuxkit build -disable-content-trust haxe.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust rethinkdb.instrumented.yml
sudo linuxkit build -disable-content-trust rethinkdb.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust flink.instrumented.yml
sudo linuxkit build -disable-content-trust flink.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust teamspeak.instrumented.yml
sudo linuxkit build -disable-content-trust teamspeak.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust redmine.instrumented.yml
sudo linuxkit build -disable-content-trust redmine.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust composer.instrumented.yml
sudo linuxkit build -disable-content-trust composer.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust xwiki.instrumented.yml
sudo linuxkit build -disable-content-trust xwiki.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust websphere-liberty.instrumented.yml
sudo linuxkit build -disable-content-trust websphere-liberty.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust opensuse.instrumented.yml
sudo linuxkit build -disable-content-trust opensuse.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust mono.instrumented.yml
sudo linuxkit build -disable-content-trust mono.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust orientdb.instrumented.yml
sudo linuxkit build -disable-content-trust orientdb.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust django.instrumented.yml
sudo linuxkit build -disable-content-trust django.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust mediawiki.instrumented.yml
sudo linuxkit build -disable-content-trust mediawiki.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust znc.instrumented.yml
sudo linuxkit build -disable-content-trust znc.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust piwik.instrumented.yml
sudo linuxkit build -disable-content-trust piwik.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust ros.instrumented.yml
sudo linuxkit build -disable-content-trust ros.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust swift.instrumented.yml
sudo linuxkit build -disable-content-trust swift.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust rust.instrumented.yml
sudo linuxkit build -disable-content-trust rust.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust oraclelinux.instrumented.yml
sudo linuxkit build -disable-content-trust oraclelinux.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust arangodb.instrumented.yml
sudo linuxkit build -disable-content-trust arangodb.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust ubuntu-debootstrap.instrumented.yml
sudo linuxkit build -disable-content-trust ubuntu-debootstrap.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust matomo.instrumented.yml
sudo linuxkit build -disable-content-trust matomo.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust rails.instrumented.yml
sudo linuxkit build -disable-content-trust rails.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust clojure.instrumented.yml
sudo linuxkit build -disable-content-trust clojure.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust odoo.instrumented.yml
sudo linuxkit build -disable-content-trust odoo.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust ibmjava.instrumented.yml
sudo linuxkit build -disable-content-trust ibmjava.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust julia.instrumented.yml
sudo linuxkit build -disable-content-trust julia.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust gcc.instrumented.yml
sudo linuxkit build -disable-content-trust gcc.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust nuxeo.instrumented.yml
sudo linuxkit build -disable-content-trust nuxeo.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust notary.instrumented.yml
sudo linuxkit build -disable-content-trust notary.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust storm.instrumented.yml
sudo linuxkit build -disable-content-trust storm.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust gazebo.instrumented.yml
sudo linuxkit build -disable-content-trust gazebo.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust irssi.instrumented.yml
sudo linuxkit build -disable-content-trust irssi.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust php-zendserver.instrumented.yml
sudo linuxkit build -disable-content-trust php-zendserver.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust plone.instrumented.yml
sudo linuxkit build -disable-content-trust plone.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust celery.instrumented.yml
sudo linuxkit build -disable-content-trust celery.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust bonita.instrumented.yml
sudo linuxkit build -disable-content-trust bonita.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust open-liberty.instrumented.yml
sudo linuxkit build -disable-content-trust open-liberty.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust cirros.instrumented.yml
sudo linuxkit build -disable-content-trust cirros.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust geonetwork.instrumented.yml
sudo linuxkit build -disable-content-trust geonetwork.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust haskell.instrumented.yml
sudo linuxkit build -disable-content-trust haskell.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust lightstreamer.instrumented.yml
sudo linuxkit build -disable-content-trust lightstreamer.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust hello-seattle.instrumented.yml
sudo linuxkit build -disable-content-trust hello-seattle.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust hylang.instrumented.yml
sudo linuxkit build -disable-content-trust hylang.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust swipl.instrumented.yml
sudo linuxkit build -disable-content-trust swipl.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust rakudo-star.instrumented.yml
sudo linuxkit build -disable-content-trust rakudo-star.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust r-base.instrumented.yml
sudo linuxkit build -disable-content-trust r-base.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust neurodebian.instrumented.yml
sudo linuxkit build -disable-content-trust neurodebian.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust backdrop.instrumented.yml
sudo linuxkit build -disable-content-trust backdrop.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust convertigo.instrumented.yml
sudo linuxkit build -disable-content-trust convertigo.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust thrift.instrumented.yml
sudo linuxkit build -disable-content-trust thrift.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust fsharp.instrumented.yml
sudo linuxkit build -disable-content-trust fsharp.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust eggdrop.instrumented.yml
sudo linuxkit build -disable-content-trust eggdrop.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust docker-dev.instrumented.yml
sudo linuxkit build -disable-content-trust docker-dev.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust ubuntu-upstart.instrumented.yml
sudo linuxkit build -disable-content-trust ubuntu-upstart.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust rapidoid.instrumented.yml
sudo linuxkit build -disable-content-trust rapidoid.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust photon.instrumented.yml
sudo linuxkit build -disable-content-trust photon.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust glassfish.instrumented.yml
sudo linuxkit build -disable-content-trust glassfish.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust spiped.instrumented.yml
sudo linuxkit build -disable-content-trust spiped.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust yourls.instrumented.yml
sudo linuxkit build -disable-content-trust yourls.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust mageia.instrumented.yml
sudo linuxkit build -disable-content-trust mageia.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust hola-mundo.instrumented.yml
sudo linuxkit build -disable-content-trust hola-mundo.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust silverpeas.instrumented.yml
sudo linuxkit build -disable-content-trust silverpeas.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust sourcemage.instrumented.yml
sudo linuxkit build -disable-content-trust sourcemage.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust known.instrumented.yml
sudo linuxkit build -disable-content-trust known.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust crux.instrumented.yml
sudo linuxkit build -disable-content-trust crux.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust sl.instrumented.yml
sudo linuxkit build -disable-content-trust sl.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust clefos.instrumented.yml
sudo linuxkit build -disable-content-trust clefos.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust euleros.instrumented.yml
sudo linuxkit build -disable-content-trust euleros.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust hipache.instrumented.yml
sudo linuxkit build -disable-content-trust hipache.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust clearlinux.instrumented.yml
sudo linuxkit build -disable-content-trust clearlinux.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust amazoncorretto.instrumented.yml
sudo linuxkit build -disable-content-trust amazoncorretto.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust alt.instrumented.yml
sudo linuxkit build -disable-content-trust alt.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust postfixadmin.instrumented.yml
sudo linuxkit build -disable-content-trust postfixadmin.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust express-gateway.instrumented.yml
sudo linuxkit build -disable-content-trust express-gateway.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust zookeeper.instrumented.yml
sudo linuxkit build -disable-content-trust zookeeper.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust erlang.instrumented.yml
sudo linuxkit build -disable-content-trust erlang.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust kapacitor.instrumented.yml
sudo linuxkit build -disable-content-trust kapacitor.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust tomee.instrumented.yml
sudo linuxkit build -disable-content-trust tomee.orig.yml
cd $KITSRC
sudo linuxkit build -disable-content-trust mongo-express.instrumented.yml
sudo linuxkit build -disable-content-trust mongo-express.orig.yml
