KALI-JESSIE


Scripts and related configs for kali-jessie linux

Built packages for Debian testing and 
backported for Kali Linux

Package list
Cinnamon-Git ( >2.02 ) and associated components
Compiz ( all ) 0.9.10+13.10.20131011
Emerald 0.9.5




AAA-Build-And-Associated-Scripts

Platform.... Debian ( wheezy,jessie,kali,sid)
Purpose..... Build forign debian packages and backport
for kali

aaa-common-functions
---- Bash script
usage: none

	common functions for all other aaa
	scripts

aaa-master-config
---- Bash script
usage: none

  this script/settings file is called
  by all the other scripts, all the settings
  for pbuilder amd all the other progs used
  are held here, variables can be tweaked here

Run in order

aaa-setup-tmpfs
---- Bash script
usage: aaa-setup-tmpfs

  Currently this script takes no
  options or arguments, when called it
  will setup a custom tmpfs dir at 
  $aaa_tmpfs_location
  the size can be configured in 
  /usr/share/aaa/aaa-master-config
  It will also setup your personal
  gpg key for auto package signing
  if you desire

aaa-setup-build-env
---- Bash script
usage: aaa-setup-build-env

  Currently this script takes no options
  it writes /etc/pbuilderrc as a default
  when HOME/.pbuilderrc is not present,
  It also sets up a local repo controlled
  by reprepro finally it writes the local
  repo entries to /etc/apt/sources.d
  and creates a default base immage

Run as desired

aaa-build-compiz-emerald
---- Bash script
usage: aaa-build-compiz-emerald < -d distro >

  Currently this script takes one option
  and argument
  -d < distro >
  -d <kali|stable|unstable|testing|ubuntu>
  
  This script uses pbuilder and reprepro
  to build compiz-0.9.10+13.10.20131011
  and emerald-0.9.5
  from ubuntu saucy for the desired distro
  (including kali)
  all packaging follows debian guidelines

aaa-build-cinamon-pbuilder
---- Bash script
usage: aaa-build-cinamon-pbuilder < -d distro >

  Currently this script takes one option
  and argument
  -d < distro >
  -d <kali|stable|unstable|testing|ubuntu>
  
  This script uses pbuilder and reprepro
  to build cinnamon directly from git
   for the desired distro
  (including kali)
  all packaging follows debian guidelines
  

legacy scripts
--not activly developed, just here for history

aaa-kali-pkg-check....... no arguments............ 
checks all installed packages for alternate kali versions ( those with kali in the version string ) then interactivly lets you downgrade to the correct version

aaa-get-build-deps....... optional argument <package name> .... <package name> ....... 
checks for and interactivly install build deps for LOCAL ie on hd, debian package,
so you can install all deps for example git versions of deb packages. Since its designed for kali/unstable mix its very conservative about
package installation with lots of checks.


aaa-build-cinnamon.........no arguments....... this one downloads all the cinnamon components from git, checks for and installs build deps, builds all the packages and installs them in order,
it can be used to install or update cinnamon git, currently icinnamon-git is at 1.9 and F***in beautiful, it is what i am writing on now. ( on a kali-jessie box obviously )
It also applies a patch i developed for nemo, that removes the annoying " you are running as root" warning at the bottom
