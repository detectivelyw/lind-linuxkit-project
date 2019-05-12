#!/bin/bash
sudo chmod 777 ../../popular-paths-scripts/linuxkit-auto-script-*
cd $KITSRC
sudo linuxkit build -disable-content-trust couchbase-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh couchbase 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust crate-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh crate 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust percona-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh percona 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust rocket.chat-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh rocket.chat 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust jruby-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh jruby 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust groovy-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh groovy 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust nats-streaming-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh nats-streaming 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust couchdb-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh couchdb 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust chronograf-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh chronograf 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust eclipse-mosquitto-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh eclipse-mosquitto 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust buildpack-deps-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh buildpack-deps 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust jetty-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh jetty 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust bash-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh bash 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust joomla-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh joomla 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust perl-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh perl 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust iojs-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh iojs 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust solr-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh solr 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust amazonlinux-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh amazonlinux 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust adminer-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh adminer 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust elixir-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh elixir 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust pypy-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh pypy 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust haxe-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh haxe 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust rethinkdb-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh rethinkdb 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust flink-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh flink 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust teamspeak-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh teamspeak 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust redmine-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh redmine 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust composer-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh composer 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust xwiki-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh xwiki 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust websphere-liberty-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh websphere-liberty 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust opensuse-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh opensuse 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust mono-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh mono 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust orientdb-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh orientdb 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust django-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh django 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust mediawiki-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh mediawiki 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust znc-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh znc 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust piwik-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh piwik 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust ros-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh ros 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust swift-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh swift 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust rust-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh rust 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust oraclelinux-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh oraclelinux 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust arangodb-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh arangodb 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust ubuntu-debootstrap-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh ubuntu-debootstrap 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust matomo-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh matomo 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust rails-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh rails 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust clojure-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh clojure 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust odoo-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh odoo 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust ibmjava-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh ibmjava 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust julia-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh julia 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust gcc-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh gcc 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust nuxeo-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh nuxeo 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust notary-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh notary 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust storm-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh storm 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust gazebo-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh gazebo 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust irssi-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh irssi 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust php-zendserver-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh php-zendserver 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust plone-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh plone 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust celery-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh celery 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust bonita-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh bonita 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust open-liberty-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh open-liberty 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust cirros-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh cirros 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust geonetwork-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh geonetwork 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust haskell-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh haskell 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust lightstreamer-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh lightstreamer 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust hello-seattle-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh hello-seattle 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust hylang-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh hylang 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust swipl-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh swipl 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust rakudo-star-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh rakudo-star 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust r-base-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh r-base 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust neurodebian-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh neurodebian 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust backdrop-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh backdrop 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust convertigo-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh convertigo 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust thrift-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh thrift 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust fsharp-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh fsharp 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust eggdrop-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh eggdrop 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust docker-dev-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh docker-dev 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust ubuntu-upstart-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh ubuntu-upstart 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust rapidoid-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh rapidoid 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust photon-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh photon 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust glassfish-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh glassfish 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust spiped-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh spiped 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust yourls-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh yourls 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust mageia-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh mageia 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust hola-mundo-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh hola-mundo 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust silverpeas-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh silverpeas 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust sourcemage-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh sourcemage 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust known-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh known 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust crux-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh crux 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust sl-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh sl 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust clefos-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh clefos 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust euleros-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh euleros 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust hipache-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh hipache 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust clearlinux-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh clearlinux 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust amazoncorretto-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh amazoncorretto 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust alt-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh alt 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust postfixadmin-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh postfixadmin 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust express-gateway-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh express-gateway 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust zookeeper-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh zookeeper 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust erlang-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh erlang 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust kapacitor-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh kapacitor 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust tomee-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh tomee 1 10
cd $KITSRC
sudo linuxkit build -disable-content-trust mongo-express-gcov.yml
cd ../linuxkit-auto-experiment/
./run-linuxkit-auto.sh mongo-express 1 10
