#!/bin/bash
sudo chmod 777 ../popular-paths-scripts/linuxkit-auto-script-*
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

