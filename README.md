aaa..........debian related packages source and scripts

intro................this is turning into o big git with lots of related
scripts and debian packages

----------------------aaa-env_0.1------------------------------------

aaa-common-functions
---- Bash script
usage: none

	common functions for all other aaa
	scripts, only sourced do not run

aaa-master-config
---- Bash script
usage: none

  this script/settings file is called
  by all the other scripts, all the settings
  for pbuilder amd all the other progs used
  are held here, variables can be tweaked here

aaa-setup-build-base
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
  
aaa-linkup
---- Bash script
usage: ./aaa-linkup < -c >
	Currently this script takes one option
	
	-c remove all links and clean up
	
	when run from the current dir this
	script will create links from the
	current dir to the correct locations
	like make but with links



----------------------aaa-build-scripts_0.1----------------------------

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

aaa-build-cinamon-pbuilder-git ******************* no longer maintained*************
---- Bash script
usage: aaa-build-cinamon-pbuilder-git < -d distro >

  Currently this script takes one option
  and argument
  -d < distro >
  -d <kali|stable|unstable|testing|ubuntu>
  
  This script uses pbuilder and reprepro
  to build cinnamon directly from git
   for the desired distro
  (including kali)
  all packaging follows debian guidelines
  
aaa-build-cinamon
---- Bash script
usage: aaa-build-cinamon-pbuilder-release < -d distro >

    Currently this script takes two options
  and one argument
  -d < distro >
  -d <kali|stable|unstable|testing|ubuntu>
  -g [build from git sources]
  
  This script uses pbuilder and reprepro
  to build cinnamon git or releases
   for the desired distro
  (including kali)
  all packaging follows debian guidelines

aaa-build
---- Bash script
usage: aaa-build < -d distro >

  Currently this script takes one option
  and argument
  -d < distro >
  -d <kali|stable|unstable|testing|ubuntu>
  
    This script uses pbuilder and reprepro
  to build the package in the current dir
  result is found at the location
  specified in aaa-master-configure
  as aaa_build_result


aaa-linkup
---- Bash script
usage: ./aaa-linkup < -c >
	Currently this script takes one option
	
	-c remove all links and clean up
	
	when run from the current dir this
	script will create links from the
	current dir to the correct locations
	like make but with links



----------------------aaa-admin-scripts_0.1----------------------------

list not available package in flux




----------------------pup-volume-monitor------------------------------

the latest sources for pup-volume-monitor packaged by me for debian

pup-volume-monitor is to be a total and lightweigt replacement for

udisks/udisks2 and gvfs, to use extra debian core packages need their
deps modifying to include pup-vm, a script to do the modifications is
available













----------------------freetype-infinality------------------------------


----------------------freetype-infinality------------------------------


----------------------freetype-infinality------------------------------


----------------------freetype-infinality------------------------------



----------------------freetype-infinality------------------------------



Package list ( sources mainly packaged and repackaged by me )

pup-volume-monitor
hopefully a complete replacement for udisks/2 and gvfs


winexe and cuda-toolkit
repackaged to fix a file confilict in kali



Scripts list...... note this readme is not updated often :)
note... i run a root only system, if you cnt su into your linux
box dont even attemt running these scripts, sudo nshould also work


KALI-JESSIE ( and possibly others )


aaa-build-scripts_0.1
new......................................................
a make file is now available and all the debian distros
are now supported ( kali jessie sid wheezy )
fixed compiz build script


.........................................................

Scripts and related configs for kali-jessie linux
AAA-Build-And-Associated-Scripts

Platform.... Debian ( wheezy,jessie,kali,sid)
Purpose..... Build forign debian packages and backport
for kali



Run in order



Run as desired


  

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
