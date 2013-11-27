#!/bin/bash
# pbuilder example script.
# Copyright 2003, 2007 Junichi Uekawa
#Distributed under GPL version 2 or later

#before running this script, make sure you have an up-to-date system
#with pbuilder update.

# This will install a package using APT inside the chroot and see if
# that fails. If it fails, it's wrong.

set -ex

# practically, killing is not enough, do a reboot here instead.
echo 'nobody@nowhere' > /etc/mailname
INSTALLTESTPID=$$
( sleep 1h ; kill $INSTALLTESTPID ) &
KILLPID=$!

apt-get install -y "${APTGETOPT[@]}" "$@" < /dev/null

kill $KILLPID

