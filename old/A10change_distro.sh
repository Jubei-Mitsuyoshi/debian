#!/bin/sh

cd $HOME

distro=$(lsb_release -sr)
echo $distro

cd $(ls -d */)

debchange --append --no-force-save-on-release --force-distribution --distribution "$distro" "automated distribution change just affects repo management"
