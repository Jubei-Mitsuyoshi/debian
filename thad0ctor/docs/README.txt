               ------------------------------------
               |   Thad0ctor's Word List Toolkit  |
               ------------------------------------
               |                                  |
               |          version #: v1.4         |
               |                                  |
               |       created by: thad0ctor      |
               |                                  |
	       |       created on: 24-Jul-12      |
               |                                  |
               |     created for: Backtrack 5R2   |
               |                                  |
               ------------------------------------

--------------------------|Background|-----------------------------
 
-Recently my interest in network security and pen-testing has been
 increasing and with that my so has curiosity about the tools and	
 functionality of the Backtrack Linux Distro. To best understand 
 Backtrack it is essential to have a solid grasp of bash scripting.
 That being said, in order to best learn the ins and outs of shell
 scripting and the critical functions that drive Backtrack I 
 decided to create this script to serve both as a learning 
 experience for me and as a valuable tool for others looking to
 secure their networks.

-This script was created due to a lot of hard work and free time
 while serving on a deployment in Afghanistan.  If you enjoy this
 script and want to support it do not hesitate to donate, thanks!

------------------------|Special Thanks|---------------------------

-This script would have never been possible without the examples
 inspiration and 3rd Party tools provided by:

	- Tape's Wordlist Manipulator and Datelist
	- Middle's Wordlister
	- Purehate's Wordlist Tools
	- slydgotcha and his word list manipulator

-All these a great scripts that come highly reccomended and are
 surely essentials for anyone trying to streamline their use of
 wordlists.  The authors have my highest praise and thanks!

-------------------------|Requirements|----------------------------

-This scripts requires Backtrack 5 Linux based on Ubuntu, although
	it has not been tested on other DISTROS it could work
	with the proper configuration

-In order to run this script you will need the following programs:

	-zenity
	-policygen
	-crunch
	-find
	-cat
	-sed
	-grep
	-pyrit (for the benchmarks)
	-aircack-ng (for the benchmarks)
	-pdftotext
	-cewl.rb
	-cupp.py
	-dictstat.py
	-maskgen.py
	-policygen.py
	-unzip
	-awk
	-tr
	-wc
	-paste
	-rev
	-gnome-terminal
	-nautilus
	-file
	-wget
	-xmlstarlet
	-script

-System wise I would recommend a minimum of a dual core setup with
	2 GB of RAM in addition to at least 50 GB of HDD space if
	you plan on making some word lists.

--------------------------|FAQ / Usage|----------------------------

Q: What is all this Both / GTK / CLI / PT stuff I see in the menus?
A: What you are seeing is info that describes the special features
	of the tools of the script.  GTK denotes that the function
	has GUI prompts while CLI means the function is command 
	line only.  Both means the function has both prompts or CLI
	features depending on how the script is configured.  PT 
	means that that feature of the script has the ability to
	passthrough its word list output to a cracking program so
	you do not have to create a large word list.  Whether or 
	not this happens depends on how the script is configured.

Q: How do I toggle between the CLI and GTK versions of the script/
A: Most of the script's menus have a function (T/t) that allows you
	to toggle between the two versions of the script but you 
	can also force the script to a certain version in the 
	configuration menu to prevent the prompt that appears every
	time you run the script.

Q: How do I use the passthrough attack feature?
A: Since v0.6 there are two ways you can use the attack passthrough
	feature for certain functions of the script.  If you prefer 
	to manually update the settings you can open up the script
	itself (LAUNCH_TOOLKIT.sh) and update lines 41 and 44 with
	the essid and capture file of the network you would like to
	attack.  Or, you can edit the settings directly from the 
	configuration menu while running the script.  Currently 
	this feature doesn't have a GTK feature to facilitate the
	configuration.

Q: What is the deal with the debug info?
A: The debug info (although not fully implemented yet) is designed
	to help troubleshoot potential user issues by allowing them
	to compare their log files and debug information to a set
	standard online to ensure they have the newest, un-adultered
	version of the script.

Q: Where can I find the latest version of this script and discuss
	any issues and or ideas pertaining to the script?
A: I do my best to update and post to my thread on the Backtrack 
	forums at: 
	www.backtrack-linux.org/forums/showthread.php?t=49222
	or at the script Sourceforge:
	https://sourceforge.net/projects/thad0ctorstools

--------------------------|Disclaimer|-----------------------------

-This script is not to be used for any illegal activities.  It was 
 designed with the premise of securing your own personal networks.
 If any of the tools on this script were able to generate passwords
 that you use I would highly suggest you create a new, more complex
 password to secure your networks.

-The author of this script is in no way responsible for any misuse
 of this script or damage resulting from the use of this script.

-------------------------|Contact Info|----------------------------

-If you have any suggestions, comments, bugs to report, questions
 or concerns about this script feel free to get in touch with me at
 thad0ctor at overclocked dot net.

-I'm working on setting up a blog in addition to a SVN repository 
 to provide automatic updates of this script in addition to a place 
 for users to come voice their opinions, check out other resources
 and even learn about other security topics.  Granted, I am not an
 expert at Linux or network security but I assure you, as I learn
 more so will you!

-------------------------------------------------------------------
