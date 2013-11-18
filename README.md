aaa..........debian related packages source and scripts

intro................this is turning into o big git with lots of related
scripts and debian packages

----------------------aaa-env------------------------------------

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
usage: aaa-setup-build-base

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


Install...............

there are 3 methods of installing these scripts.
1) run ./aaa-linkup in the aaa-env dir
( recomended if youare doing your own hacking
on these scripts since changes are echoed to
your git location)
2) run make install in the aaa-env dir
3) run aaa-install-all from the root git dir

Run------------------( PLEASE ADD SUDO IF YOU USE PRIVALAGES )


Default settings--------------
Distro = unstable
Build in tmps = yes
Mirror location = denmark

These and other settings are exposed in aaa-master-config,
change as desired, read comments included in file

cd to home directory

run aaa-setup-build-base
follow prompts, and read onscreen messages, which are extensive

run aaa-setup-build-env
follow prompts, and read onscreen messages, which are extensive




----------------------aaa-build-scripts----------------------------

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
  
aaa-build-pupvm
---- Bash script
usage: aaa-build-pupvm < -d distro >

  Currently this script takes one option
  and argument
  -d < distro >
  -d <kali|stable|unstable|testing|ubuntu>
  
    This script uses pbuilder and reprepro
  to build pup vm from git, it also creates
  modified packages to allow pup-vm to work
  in a native fassion on the given debian
  distro


aaa-linkup
---- Bash script
usage: ./aaa-linkup < -c >
	Currently this script takes one option
	
	-c remove all links and clean up
	
	when run from the current dir this
	script will create links from the
	current dir to the correct locations
	like make but with links

-----------IMPORTANT NOTES-----------------------
these build scripts depend on having installed and run the
scripts in aaa-env, plese see relevent section for install
and run info

----------------------------------------------------------


Install...............
there are 3 methods of installing these scripts.
1) run ./aaa-linkup in the aaa-env dir
( recomended if youare doing your own hacking
on these scripts since changes are echoed to
your git location)
2) run make install in the aaa-env dir
3) run aaa-install-all from the root git dir


----------------------aaa-admin-scripts----------------------------

list not available-- package in flux

-----------IMPORTANT NOTES-----------------------
these admin scripts depend on having installed and run the
scripts in aaa-env, plese see relevent section for install
and run info

----------------------------------------------------------


Install...............
there are 3 methods of installing these scripts.
1) run ./aaa-linkup in the aaa-env dir
( recomended if youare doing your own hacking
on these scripts since changes are echoed to
your git location)
2) run make install in the aaa-env dir
3) run aaa-install-all from the root git dir


----------------------Kali---------------------------------------------

various packages and various kali fixes, issues and package conflicts




  

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
