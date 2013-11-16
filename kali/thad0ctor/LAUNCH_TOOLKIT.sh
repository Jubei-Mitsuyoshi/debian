#!/bin/bash
##!!!DO NOT MODIFY OR ADD ANY LINES FROM HERE UNTIL LINE 53!!!##
##!!!     BETTER YET, DON'T MODIFY ANYTHING PERIOD ;)         ##
##version##
scriptversion=Kali
function f_config () {
  ###############################################
  ##thad0ctor's backtrack 5 tooklit config file##
  ###############################################
  #                                             #
  #  Settings in this file will be applied on   #
  #  the script's startup.                      #
  #                                             #
  #  For instance, if you would prefer to skip  #
  #  the GTK check you can customize that       #
  #  setting in this config file.               #
  #                                             #
  ###############################################
## force set the GTK (Zenity Prompts) (Gnome Only) ##
## "y"= Yes "n"= No (CLI) ""= Prompt on execution  ##
##  Default setting = "x" (Prompt on script start)  ##
GTK="x"
##    force auto-update the script on execution    ##
##   "y" = auto-update "n" = No = manual update    ##
##  Default setting = "y" (script will auto-update)##
autoupdate="n"
##    force the script to start on the big menu    ##
##                "y"= Yes "n"= No                 ##
##   Default setting = "n" (Go to the main menu)   ##
bigmenu="n"
##    force the script output to a certain color   ##
##                "y"= Yes "n"= No                 ##
##        Default setting = "7" (Cyan)             ##
COLOR="7"
##       set the default passthrough program       ##
##  "0" = none "1" = aircrack-ng  "2"= pyrit       ##
##                 Default = "0"                   ##
passthrough="0"
##   set the default passthrough access point      ##
##               Default = "essid"                 ##
passthroughap="essid"
##     set the default passthrough cap file        ##
##           Default = "test.cap"                  ##
passthroughcap="test.pcap"
folder="$( cd "$( dirname "$0" )" && pwd )"
##    checks if the script has been run before     ##
firstrun="y"
##      would you like to display the banner       ##
##                 Default = "y"                   ##
banner="y"
##  display most recent rss news in main menu      ##
##                 Default = "y"                   ##
rssnews="y"
}
###Sourceing Dependencies###
source depend/wifi.sh
source depend/manipulation.sh
source depend/creation.sh
source depend/dependency.sh
source depend/combination.sh
source depend/router.sh
source depend/analyze.sh
source depend/wordify.sh
##!!!DO NOT MODIFY OR ADD ANY LINES FROM HERE UP!!!##
##echo break##
function f_echobreak () {
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
	echo
}
function f_quickcheck () {
#f_echobreak
echo
echo "              Checking for internet connectivity / updates..."
echo
rm current.txt &> /dev/null
wget --tries=3 --timeout=15 https://dl.dropbox.com/u/40006489/BT5_Downloads/current.txt &> /dev/null
newestversion=$(cat current.txt | awk -F " " '{print$1}')
if [ ! -s "current.txt" ]; then
	internet="n"
	rm current.txt &> /dev/null
else
	internet="y"
	rm current.txt &> /dev/null
fi
}
function f_menuinstall () {
	f_menuinstaller
	echo "Returning to the main menu..."
	echo
	sleep 3
	f_menu
}
function f_menuinstaller () {
	f_echobreak
	new=$(echo /usr/share/thad0ctor)
	dir=$(echo "$new" | sed 's/LAUNCH_TOOLKIT.sh//')
	echo "Adding thad0ctor's Kali toolkit to your Applications > Other menu"
	echo
	sed -i -e '/'"Exec"'/d' "./depend/install/thad0ctor.desktop"
	echo 'Exec=gnome-terminal -x bash -c "cd '"'$dir'"'; bash LAUNCH_TOOLKIT.sh"' >> "./depend/install/thad0ctor.desktop"
	cp "./depend/install/thad0ctor.desktop" "/usr/share/applications"
	if [ -e "/usr/share/applications/thad0ctor.desktop" ]; then
		echo "The application link exists in /usr/share/applications"
		echo
		sleep 2
		echo "Continuing..."
		echo
		sleep 3
	else
		echo "The application link does not exist in /usr/share/applications"
		echo
		sleep 2
		echo "Something went wrong, returning to the main menu..."
		echo
		sleep 3
		f_menu
	fi
	echo
	echo "Copying the application icon to /usr/share/icons"
	echo
	cp "./depend/install/thad0ctorstoolkiticon.png" "/usr/share/icons"
	if [ -e "/usr/share/icons/thad0ctorstoolkiticon.png" ]; then
		echo "The application icon exists in /usr/share/icons"
		echo
		sleep 2
		echo "Continuing..."
		echo
		sleep 3
	else
		echo "The application link does not exist in /usr/share/applications"
		echo
		sleep 2
		echo "Something went wrong, returning to the main menu..."
		echo
		sleep 3
		f_menu
	fi
	echo "Restarting gnome-panel to reflect the changes..."
	echo
	sleep 4
	killall gnome-panel
	echo "A link to this script can now be found under Applications > Other"
	echo
	sleep 5
}
##display rss feed##
function f_rss () {
RSS=https://sourceforge.net/p/thad0ctorstools/blog/feed
f_echobreak
echo "      Loading thad0ctor's Backtrack 5 mod Kali toolkit development RSS 2.0 feed..."
echo
wget ${RSS} --no-check-certificate -O - 2>/dev/null | \
xmlstarlet sel -t -m "/rss/channel/item" \
  -n -o '           ' -v "title" -o '    ( ' -v "pubDate" -o ')' -n -n -v "link" -n -n -v "description" -n  -n -o '--------------------------------------------------------------------------------' -n | sed 's/-0000//'
echo
echo "                 Hit enter to return to the information menu"
echo
read rssinput
if [ "$rssinput" = "" ]; then
	f_info
else
	f_info
fi
}
##bash install##
function f_bashinstaller () {
f_echobreak
echo "        In order to install the script to your .bashrc all"
echo "         apostrophes must be removed from the script path."
echo
sleep 3
echo "      Also, all version information will be stripped from the"
echo "     script directory to preseve the proper file path for .bashrc"
echo
sleep 10
echo "      Would you like the script to do this for you? (y/n)"
echo
read installer
echo
if [ "$installer" = "y" ]; then
	new=$(echo /usr/share/thad0ctor/ | sed 's/'"'"'//')
	new1=$(echo $new | sed 's/ v'"$scriptversion"'//')
	dir=$(echo $new1 | sed 's/LAUNCH_TOOLKIT.sh//')
	mkdir "$dir"
	echo "     Adding thad0ctor's word list toolkit to your .bashrc"
	echo alias thad0ctor="'"'cd ''"'$dir'"' '&& bash LAUNCH_TOOLKIT.sh'"'" >> /root/.bashrc
	source /root/.bashrc
	echo
	sleep 2
	echo "        Moving the script files to the new directory"
	sleep 3
	mv * "$dir"
	sleep 2
	echo "     Added thad0ctor's word list toolkit to your .bashrc"
	echo
	sleep 2
	echo "     You can now run the script directly from the terminal"
	echo "              by using the command: 'thad0ctor'"
	echo " (you must reinstall with this function if you move the script)"
	echo
	sleep 10
	echo "                        New directory:" 
	echo "$dir"
	echo
	echo "       Navigating to the new directory with nautilus..."
	echo
	sleep 3
	nautilus "$dir"
	echo
	sleep 10
	echo "Would you like to install a shortcut to your Applications > Other menu? (y/n)"
	echo
	read menu
	if [ "$menu" = "y" ] || [ "$menu" = "Y" ]; then
		f_menuinstaller
	elif [ "$menu" = "n" ] || [ "$menu" = "N" ]; then
		echo "       You chose not to install a shortcut at this time"
		echo
		sleep 3
		echo "   You can always do this later from the configuration menu..."
		echo
		sleep 3
	else
		echo "        You did not enter a correct answer, skipping..."
		echo
		sleep 3
		echo "  You can always do this later from the configuration menu..."
		echo
		sleep 3
	fi
	echo "    Exiting the script, start over from the new directory or"
	echo "            by typing 'thad0ctor' in the terminal"
	echo
	sleep 15
	exit
elif [ "$installer" = "n" ]; then
	echo "       You chose not to install the script at this time"
	echo
	sleep 2
	echo "With out installing you will not be able to run the script with 'thad0ctor'"
	echo
	sleep 3
	echo "                 Returning to the main menu..."
	echo
	sleep 5
	f_menu
else
	echo "You did not input a correct answer!"
	echo
	sleep 2
	echo "Returning to the main menu..."
	echo
	sleep 3
	f_menu
fi
}
##run on first run##
function f_firstrun () {
	if [ "$firstrun" = "y" ]; then
		f_echobreak
		echo "This is the first time you have run thad0ctor's word list toolkit v$scriptversion"
		echo "or you elected to run the dependency check upon starting the script."
		echo
		sleep 3
		echo "Checking for dependencies..."
		echo
		sleep 3
		f_depend
	else
		echo
	fi
}
##terminal colors##
function f_color () {
GREY=$(tput sgr0 && tput bold)
RED=$(tput setaf 1 && tput bold)
GREEN=$(tput setaf 2 && tput bold)
YELLOW=$(tput setaf 3 && tput bold)
LBLUE=$(tput setaf 4 && tput bold)
PURPLE=$(tput setaf 5 && tput bold)
BLUE=$(tput setaf 6 && tput bold)
NORMAL=$(tput sgr0)
if [ "$COLOR" = "1" ];then
	##GREY##
	ECHOCOLOR=$(tput sgr0 && tput bold)
elif [ "$COLOR" = "2" ];then
	##RED##
	ECHOCOLOR=$(tput setaf 1 && tput bold)
elif [ "$COLOR" = "3" ];then
	##GREEN##
	ECHOCOLOR=$(tput setaf 2 && tput bold)
elif [ "$COLOR" = "4" ];then
	##YELLOW##
	ECHOCOLOR=$(tput setaf 3 && tput bold)
elif [ "$COLOR" = "5" ];then
	##CYAN##
	ECHOCOLOR=$(tput setaf 4 && tput bold)
elif [ "$COLOR" = "6" ];then
	##PURPLE##
	ECHOCOLOR=$(tput setaf 5 && tput bold)
elif [ "$COLOR" = "7" ];then
	##BLUE##
	ECHOCOLOR=$(tput setaf 6 && tput bold)
elif [ "$COLOR" = "8" ];then
	##DEFAULT##
	ECHOCOLOR=$(tput sgr0)
fi
}
##restart##
function f_restart () {
	gnome-terminal -e "bash LAUNCH_TOOLKIT.sh"; exit
}
##toggle between GTK / CLI##
function f_toggle () {
	f_echobreak
	sed -i -e 's/n//g' depend/tempsettings.sh
	sed -i -e 's/y//g' depend/tempsettings.sh
	if [ "$GTK" = "y" ]; then
		GTK=n
		sed -i -e 's/^/n/' depend/tempsettings.sh
	elif [ "$GTK" = "n" ]; then
		GTK=y
		sed -i -e 's/^/y/' depend/tempsettings.sh
	fi
}
##updater##
function f_updater () {
	echo
	echo "About to update thad0ctor's word list toolkit."
	echo
	sleep 2
	echo "Downloading the latest version from source forge..."
	echo
	rm -r thad0ctorstools-code
	git clone git://git.code.sf.net/p/thad0ctorstools/code thad0ctorstools-code
	bash depend/UPDATER.sh
	echo $ECHOCOLOR""
	echo
	echo "Returning to the main menu..."
	echo
	sleep 3
	f_menu
}
##internet check##
function f_wwwcheck () {
	f_echobreak
	echo "Updating thad0ctor's word list toolkit..."
	echo
	sleep 1
	echo "Verifying internet connectivity."
	echo
	wget -q --tries=10 --timeout=15 http://www.google.com -O /tmp/index.google &> /dev/null
	if [ ! -s /tmp/index.google ]; then
		echo "The internet is currently unavailable."
		echo
		sleep 1
		echo "Please try again later."
		echo
		sleep 1
		echo "Returning to the main menu..."
		echo
		sleep 4
		rm /tmp/index.google
		f_menu
	else
		echo "Successfully connected to the internet."
		echo
		sleep 2
		rm /tmp/index.google
		f_updater
	fi
}
##config color##
function f_config_color () {
f_echobreak
echo $GREY"1) USE GREY TEXT"
echo $RED"2) USE RED TEXT"
echo $GREEN"3) USE GREEN TEXT"
echo $YELLOW"4) USE YELLOW TEXT"
echo $LBLUE"5) USE BLUE TEXT"
echo $PURPLE"6) USE PURPLE TEXT"
echo $BLUE"7) USE CYAN TEXT"
echo $NORMAL"8) USE THE DEFAULT TEXT"
echo $ECHOCOLOR""
echo
echo "Enter what text you would like to use."
echo
read CLR
sed -i -e '34s/^\(.\{7\}\)'$COLOR'/\1'$CLR'/' "LAUNCH_TOOLKIT.sh"
echo
echo "Starting the script over to reflect changes..."
echo
sleep 3
f_restart
}
##force big menu##
function f_bigmenu () {
f_echobreak
echo "1) Force the old style menu (all tools)"
echo "2) Force the new menu style (compartamentalized)"
echo
read selection
if [ "$selection" = "1" ]; then
	sed -i -e '30s/^\(.\{9\}\)'$bigmenu'/\1y/' "LAUNCH_TOOLKIT.sh"
	echo
	echo "The old style menu (all items) is now activated"
	echo
	sleep 2
	echo "Starting the script over to reflect changes..."
	echo
	sleep 3
	f_restart
elif [ "$selection" = "2" ]; then
	sed -i -e '30s/^\(.\{9\}\)'$bigmenu'/\1n/' "LAUNCH_TOOLKIT.sh"
	echo
	echo "The new style menu (compartamentalized) is now activated"
	echo
	sleep 2
	echo "Starting the script over to reflect changes..."
	echo
	sleep 3
	f_restart
else
	echo "You did not enter a correct answer."
	echo
	echo "Starting over..."
	echo
	sleep 3
	f_bigmenu
fi
}
##config update##
function f_config_update () {
	f_echobreak
	echo "1) Turn on Auto-Update"
	echo "2) Turn off Auto-Update"
	echo
	echo "  Please select an auto-update configuration."
	echo "(Checks are conducted upon starting the script.)"
	echo
	read update
	echo
	if [ "$update" = "1" ]; then
		sed -i -e '26s/^\(.\{12\}\)'$autoupdate'/\1y/' "LAUNCH_TOOLKIT.sh"
		echo
		echo "Auto-Update is now enabled"
		echo
		sleep 2
		echo "Restarting the script to reflect your changes..."
		echo
		sleep 2
		f_restart
	elif [ "$update" = "2" ]; then
		sed -i -e '26s/^\(.\{12\}\)'$autoupdate'/\1n/' "LAUNCH_TOOLKIT.sh"
		echo
		echo "Auto-Update is now disabled"
		echo
		sleep 2
		echo "Restarting the script to reflect your changes..."
		echo
		sleep 2
		f_restart
	else
		echo "You did not select a correct answer."
		echo
		echo "Returning to the main menu..."
		echo
		sleep 2
		f_menu
	fi
}
##config update##
function f_config_rss () {
	f_echobreak
	echo "1) Display the RSS news banner on the main menu"
	echo "2) Do not display the RSS news banner on the main menu"
	echo
	echo "Please select if you would like to display the banner or start up."
	echo
	read update
	echo
	if [ "$update" = "1" ]; then
		sed -i -e '53s/^\(.\{9\}\)'$rssnews'/\1y/' "LAUNCH_TOOLKIT.sh"
		echo
		echo "The RSS banner will now be displayed on the main menu"
		echo
		sleep 2
		f_configure
	elif [ "$update" = "2" ]; then
		sed -i -e '53s/^\(.\{9\}\)'$banner'/\1n/' "LAUNCH_TOOLKIT.sh"
		echo
		echo "The banner will no longer be displayed on the main menu"
		echo
		sleep 2
		f_configure
	else
		echo "You did not input a correct answer!"
		echo
		sleep 2
		echo "Starting over..."
		echo
		sleep 3
		f_config_rss
	fi
}
##config update##
function f_config_banner () {
	f_echobreak
	echo "1) Display the banner"
	echo "2) Do not display the banner"
	echo
	echo "  Please select if you would like to display the banner or start up."
	echo
	read update
	echo
	if [ "$update" = "1" ]; then
		sed -i -e '26s/^\(.\{8\}\)'$banner'/\1y/' "LAUNCH_TOOLKIT.sh"
		echo
		echo "The banner will now be displayed upon script startup"
		echo
		sleep 2
		f_configure
	elif [ "$update" = "2" ]; then
		sed -i -e '49s/^\(.\{8\}\)'$banner'/\1n/' "LAUNCH_TOOLKIT.sh"
		echo
		echo "The banner will no longer be displayed upon script startup"
		echo
		sleep 2
		f_configure
	else
		echo "You did not input a correct answer!"
		echo
		sleep 2
		echo "Starting over..."
		echo
		sleep 3
		f_config_banner
	fi
}
##config GTK##
function f_config_GTK () {
f_config
f_echobreak
echo "y) Force GTK to yes"
echo "n) Force GTK to no"
echo "x) Prompt on startup"
echo
echo "Select what you would like the GTK forced to."
echo "  (You can always toggle back and forth.)"
echo
read CONFIGGTK
echo
if [ "$CONFIGGTK" = "y" ] || [ "$CONFIGGTK" = "n" ] || [ "$CONFIGGTK" = "x" ]; then
	sed -i -e '22s/^\(.\{5\}\)'$GTK'/\1'$CONFIGGTK'/' "LAUNCH_TOOLKIT.sh"
	echo "The startup GTK setting has been changed to reflect: $CONFIGTK"
	echo
	sleep 2
else
	echo "You did not input a correct selection!"
	echo
	sleep 2
	echo
	echo "Starting over..."
	echo
	sleep 3
	f_config_GTK
fi
echo
echo "Starting the script over to reflect changes..."
echo
sleep 3
f_restart
}
###config passthrough##
function f_configpassthrough () {
	f_echobreak
	echo "1) Do not passthrough word lists"
	echo "2) Passthrough word lists to Aircrack-NG"
	echo "3) Passthrough word lists to Pyrit"
	echo "4) Enter the default passthrough essid"
	echo "5) Enter the default passthrough cap file"
	echo "X) Return to the config menu."
	echo
	read selection
	echo
	if [ "$selection" = "1" ]; then
		sed -i -e '38s/^\(.\{13\}\)'$passthrough'/\10/' "LAUNCH_TOOLKIT.sh"
		echo "You chose to disable the passthrough feature"
		echo
		sleep 2
		echo "Restarting the script to reflect your changes..."
		echo
		sleep 3
		f_restart
	elif [ "$selection" = "2" ]; then
		sed -i -e '38s/^\(.\{13\}\)'$passthrough'/\11/' "LAUNCH_TOOLKIT.sh"
		echo "You chose the aircrack-ng passthrough feature"
		echo
		sleep 2
		echo "Restarting the script to reflect your changes..."
		echo
		sleep 3
		f_restart
	elif [ "$selection" = "3" ]; then
		sed -i -e '38s/^\(.\{13\}\)'$passthrough'/\12/' "LAUNCH_TOOLKIT.sh"
		echo "You chose the pyrit passthrough feature"
		echo
		sleep 2
		echo "Restarting the script to reflect your changes..."
		echo
		sleep 3
		f_restart
	elif [ "$selection" = "4" ]; then
		echo
		echo "Enter the name (essid) of the network to attack."
		read accesspoint
		echo
		accesspointt="$accesspoint"
		sed -i -e '41s/^\(.\{15\}\)'"$passthroughap"'/\1'"$accesspointt"'/' "LAUNCH_TOOLKIT.sh"
		echo "You chose to attack $accesspoint for the passthrough attack"
		echo
		sleep 2
		echo "Restarting the script to reflect your changes..."
		echo
		sleep 5
		f_restart
	elif [ "$selection" = "5" ]; then
		echo
		echo "Enter the capture file you would like to use."
		echo "(e.x. /root/Desktop/capture.pcap)"
		read capture
		echo
		#sed -i -e '44s/^\(.\{16\}\)'"$passthroughcap"'/\1'"$capture"'/' "LAUNCH_TOOLKIT.sh"
		sed -i -e '44s#'"$passthroughcap"'#'"$capture"'#' "LAUNCH_TOOLKIT.sh"
		echo "You chose to use $passthroughcap for the passthrough attack"
		echo
		sleep 2
		echo "Restarting the script to reflect your changes..."
		echo
		sleep 5
		f_restart
	elif [ "$selection" = "X" ] || [ "$selection" = "x" ]; then
		f_configure
	else
		echo
		echo "You did not input a correct selection!"
		echo
		sleep 2
		echo "Starting over..."
		echo
		sleep 3
		f_configpassthrough
	fi
}
##1337ify##
function f_config1337ify () {
	f_echobreak
	echo "About to edit depend/1337ify.sh to customize the 1337ify settings."
	echo
	sleep 2
	echo "To continue in the CLI enter (y) otherwise enter (n) for GEDIT (y/n)"
	echo
	read taco
	echo
	if [ "$taco" = "y" ]; then
		echo
		echo "Opening depend/1337ify.sh in the command line with nano..."
		echo
		sleep 2
		echo "Press CTRL + X and then Y when you are done."
		echo
		sleep 3
		nano -w depend/1337ify.sh
		echo
		echo "Done editing depend/1337ify.sh"
		echo
		sleep 2
		echo
		echo "Returning to the main menu..."
		echo
		sleep 3
		f_menu	
	elif [ "$taco" = "n" ]; then
		echo
		echo "Opening depend/1337ify.sh with GEDIT..."
		echo
		sleep 3
		gedit depend/1337ify.sh
		echo
		echo "Done editing depend/1337ify.sh"
		echo
		sleep 2
		echo
		echo "Returning to the main menu..."
		echo
		sleep 3
		f_menu	
	else
		echo
		echo "You did not input a correct answer."
		echo
		sleep 2
		echo "Starting over..."
		echo
		sleep 3
		f_config1337ify
	fi
}
##configure##
function f_configure () {
	f_echobreak
	echo " 1)  Change Console Output Color"
	echo " 2)  Toggle on/off Auto-Update"
	echo " 3)  Force GTK On/Off On Script Start "
	echo " 4)  Force Old Style Menu (All Tools)"
	echo " 5)  Change the attack passthrough options"
	echo " 6)  Change the 1337ify conversion options"
	echo " 7)  Force dependency check on next run"
	echo " 8)  Force dependency check and install"
	echo " 9)  Toggle the start up banner on/off"
	echo "10)  Install this script to run from the terminal with 'thad0ctor'"
	echo "11)  Install a menu entry to run this script from Applications > Other"
	echo "12)  Toggle the RSS news banner on the main menu."
	echo " D)  Display the current configurations."
	echo " X)  Return to the main menu."
	echo
	read selection
	if [ "$selection" = "1" ]; then
		f_config_color
	elif [ "$selection" = "2" ]; then
		f_config_update
	elif [ "$selection" = "3" ]; then
		f_config_GTK
	elif [ "$selection" = "4" ]; then
		f_bigmenu
	elif [ "$selection" = "5" ]; then
		f_configpassthrough
	elif [ "$selection" = "6" ]; then
		f_config1337ify
	elif [ "$selection" = "7" ]; then
		f_echobreak
		echo "Forcing dependency check on next run..."
		echo
		sleep 4
		sed -i -e '47s/^\(.\{10\}\)'$firstrun'/\1y/' "LAUNCH_TOOLKIT.sh"
		f_echobreak
		echo "Returning to the configuration menu..."
		echo
		sleep 3
		f_configure
	elif [ "$selection" = "8" ]; then
		f_echobreak
		f_depend
	elif [ "$selection" = "9" ]; then
		f_echobreak
		f_config_banner
	elif [ "$selection" = "10" ]; then
		f_bashinstaller
	elif [ "$selection" = "11" ]; then
		f_menuinstall
	elif [ "$selection" = "12" ]; then
		f_config_rss
	elif [ "$selection" = "X" ] || [ "$selection" = "x" ]; then
		f_echobreak
		f_menu
	elif [ "$selection" = "D" ] || [ "$selection" = "d" ]; then
		f_displayconfig
	else
		echo
	fi
}
##known issues##
function f_knownissues () {
f_echobreak
while :
do
cat docs/KNOWNISSUES.txt
echo
echo "                 Enter X to return to the info menu: "
read choice

case $choice in

X|x) f_info ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##changes new##
function f_changesnew () {
f_echobreak
echo "                          -----------------"
echo "                          |Changes to v$scriptversion|"
echo "                          -----------------"
echo
while :
do
cat docs/UPDATES.txt
echo
echo "                 Enter X to return to the info menu: "
read choice

case $choice in

X|x) f_info ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##change log##
function f_changelog () {
f_echobreak
while :
do
cat docs/CHANGELOG.txt
echo
echo "                 Enter X to return to the info menu: "
read choice

case $choice in

X|x) f_info ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##script updater##
function f_autoupdate () {
	f_echobreak
	sleep 2
	if [ "$autoupdate" = "y" ]; then
		echo
		if [ "$internet" = "y" ]; then
			f_updater
		else
			f_wwwcheck
			f_menu
		fi
	else
		echo
	fi
}
##script info##
function f_debug () {
f_echobreak
size1=$(stat --printf="%s" "$0")
size=$(( size1 / 1024 ))
modifieddate=$(stat --printf="%y" "$0" | awk -F " " '{print$1}')
modifiedtime=$(stat --printf="%y" "$0" | awk -F " " '{print$2}' | cut -d "." -f1)
linecount=$(wc -l "$0" | awk -F " "  '{print$1}')
md5hash=$(md5sum "$0" | awk -F " "  '{print$1}')
scriptname=$(stat --printf="%n" "$0")
depend=$(ls -s depend)
dependsize=$(ls -s depend | grep total | awk -F " " '{print$2}')
rdparty=$(ls -s 3rdparty)
rdpartysize=$(ls -s 3rdparty| grep total | awk -F " " '{print$2}')
main=$(ls -s)
mainsize=$(ls -s | grep total | awk -F " " '{print$2}')
toolsize=$(( $mainsize + $dependsize + $rdpartysize ))
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo "-------------------|thad0ctor's backtrack 5 toolkit info|---------------------"
echo "|                             by: thad0ctor                                   |"
     "|                         modified for Kali by Devil_D                        |"
echo "------------------------------------------------------------------------------"
echo
echo "Script Path: $0"
echo "Last Modified: $modifieddate at $modifiedtime"
echo
#echo "Lines Of Code: $linecount ;)"
echo "Process ID: $$"
echo "Script MD5SUM: $md5hash"
echo "Script Size: $size kb"
echo
echo "---Script Files Debug---"
echo " (All sizes are in kb.)"
echo
echo "Main Folder:"
echo "$main"
echo
echo "Main Folder Size: $mainsize kb"
echo
echo "Dependencies:"
echo "$depend"
echo
echo "Dependencies Size: $dependsize kb"
echo
echo "3rd Party Tools:"
echo "$rdparty"
echo
echo "3rd Party Size: $rdpartysize kb"
echo
echo "Total Toolkit Size: $toolsize kb"
echo
echo
echo "              Press enter to return to the information menu."
echo
read enter
if [ "$enter" = "" ]; then
	f_info
else
	f_debug
fi
}
function f_info () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo "-------------------|thad0ctor's backtrack 5 toolkit info|---------------------"
echo "|                             by: thad0ctor                                  |"
echo "|                     modified by Devil_D for run in kali                    |"
echo "|                                                                            |"
echo "| 1. Update the script (requires internet access)                            |"
echo "| 2. Known Issues                                                            |"
echo "| 3. Change Log                                                              |"
echo "|      3a.  Just show the changes to v$scriptversion	                   |"          
echo "| 4. Read Me                                                                 |"
echo "| 5. Show the script debug info                                              |"
echo "| 6. Display the Sourceforge Development RSS Feed (requires internet)        |"
echo "| X. Return to the main menu                                                 |"
echo "|                                                                            |"
echo "------------------------------------------------------------------------------"
while :

do

cat << !
!
echo
echo "                      Select an option from the menu: "
read choice
echo
case $choice in

1) f_wwwcheck ;;

2) f_knownissues ;;

3) f_changelog ;;

3a) f_changesnew ;;

4) f_readme ;;

5) f_debug ;;

6) f_rss ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu combination##
function f_menu_combination () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"------------------|thad0ctor's word list combination tools|--------------------"
echo $ECHOCOLOR"------------|Created by thad0ctor and modified for Kali by Devil_D|------------"
while :

do

cat << !
|                                                                             |
| 1.  Combine word lists from a directory into one file                 (Both)|
| 2.  Combine specific word lists from one folder into one file          (GTK)|
| 3.  Gather word lists from across your system into one folder         (Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_combine ;;

2) f_combiner ;;

3) f_gather ;;

T|t) f_toggle && f_menu_combination ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu router##
function f_menu_router () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"-----------------|thad0ctor's router/device word list  tools|------------------"
while :

do

cat << !
|                                                                             |
| 1.  Create a word list for default belkin54g router passwords     (PT)(Both)|
| 2.  Create a word list for default Verizon Wireless Mifi Devices      (Both)|
| 3.  Create a word list for default KooBrick router passwords      (PT)(Both)|
| 4.  Create a word list for default Plusnet router passwords       (PT)(Both)|
| 5.  Create a word list for default SKY router passwords           (PT)(Both)|
| 6.  Create a word list for default Thomson router passwords       (PT)(Both)|
| 7.  Create a word list for default ATT 2WIRE router passwords     (PT)(Both)|
| 8.  Create a word list for default Comcast Aeris router passwords (PT)(Both)|
| 9.  Create a word list for default Huawei Mifi Device passwords   (PT)(Both)|
| 10. Create a word list for default ATT Mifi device password	    (PT)(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Creation Menu                                                           |
!
echo "|                           ------------------------                          |"
echo "|---------------------------| GTK Enabled (y/n): $GTK |--------------------------|"
echo "|         PT (Passthrough) = $passthrough (O = Off | 1 = Aircrack | 2 = Pyrit)           |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_belkin ;;

2) f_vzw ;;

3) f_koobrick ;;

4) f_plusnet ;;

5) f_sky ;;

6) f_thomson ;;

7) f_2wire ;;

8) f_aeris ;;

9) f_huawei ;;

10) f_att ;;

T|t) f_toggle && f_menu_router ;;

X|x) f_menu_creation ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu creation##
function f_menu_number () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"----------------|thad0ctor's number word list creation tools|------------------"
while :

do

cat << !
|                                                                             |
| 1.  Create phone # word list(s) for all active US area codes      (PT)(Both)|
| 2.  Create phone # word list(s) for specific area codes               (Both)|
| 3.  Create a social security # word list for all possible SSNs    (PT)(Both)|
| 4.  Create a social security # word list for specific regions          (WIP)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Creation Menu                                                           |
!
echo "|                           ------------------------                          |"
echo "|---------------------------| GTK Enabled (y/n): $GTK |--------------------------|"
echo "|         PT (Passthrough) = $passthrough (O = Off | 1 = Aircrack | 2 = Pyrit)           |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_phone1 ;;

2) f_phone ;;

3) f_social ;;

4) f_region ;;

T|t) f_toggle && f_menu_number ;;

X|x) f_menu_creation ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu creation##
function f_menu_text () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"------------------|thad0ctor's text word list creation tools|------------------"
while :

do

cat << !
|                                                                             |
| 1.  Create a word list from a website with CEWL (requires internet)   (Both)|
| 2.  Create a word list from a PDF file  				(Both)|
| 3.  Create a word list by manipulating a certain word			(Both)|
| 4.  Create a word list from directory and file names.			(Both)|
| 5.  Create a word list from an ebook file				(Both)|
| 6.  Batch convert multiple PDFs / Ebooks into a word list		(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Creation Menu                                                           |
!
echo "|                           ------------------------                          |"
echo "|---------------------------| GTK Enabled (y/n): $GTK |--------------------------|"
echo "|         PT (Passthrough) = $passthrough (O = Off | 1 = Aircrack | 2 = Pyrit)           |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_cewl ;;

2) f_pdf ;;

3) f_mangleword ;;

4) f_filedir ;;

5) f_ebook ;;

6) f_batchconvert ;;

T|t) f_toggle && f_menu_text ;;

X|x) f_menu_creation ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu custom##
function f_menu_custom () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"-----------------|thad0ctor's custom word list creation tools|-----------------"
while :

do

cat << !
|                                                                             |
| 1.  Create a word list with an Crunch interactive prompt system        (GTK)|
| 2.  Create a word list with the Common User Password Profiler (CUPP)   (CLI)|
| 	2a. Replace the CUPP config file with a more thorough config     (CLI)|
| 	2b. Restore the CUPP config file with the default config         (CLI)|
| 3.  Create a word list for a specific password policy                  (CLI)|
| 4.  Create word lists for a range of dates with Tape's Datelist       (Both)|
	4a. Create a wordified date word list from scratch w/ Tape's     (CLI)|
	4b. Wordify an existing word list from Tape's Datelist	         (CLI)|
| 5.  Pre-calculate the hashes of a word list for a Cowpatty Attack     (Both)|
| 6.  Create a word list with a customizeable 'random' pattern		(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Creation Menu                                                           |
!
echo "|                           ------------------------                          |"
echo "|---------------------------| GTK Enabled (y/n): $GTK |--------------------------|"
echo "|         PT (Passthrough) = $passthrough (O = Off | 1 = Aircrack | 2 = Pyrit)           |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_cruncher ;;

2) f_cupp ;;

2a) f_cuppreplacer ;;

2b) f_cupprestore ;;

3) f_policy ;;

4) f_tapes ;;

4a) f_wordifyscratch ;;

4b) f_wordifyexisting ;;

5) f_genpmk ;;

6) f_random ;;

T|t) f_toggle && f_menu_custom ;;

X|x) f_menu_creation ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu creation_all##
function f_menu_creation_all () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"--------------------|thad0ctor's word list creation tools|---------------------"
while :

do

cat << !
|                                                                             |
| 1.  Create phone # word list(s) for all active US area codes      (PT)(Both)|
| 2.  Create phone # word list(s) for specific area codes               (Both)|
| 3.  Create a social security # word list for all possible SSNs    (PT)(Both)|
| 4.  Create a social security # word list for specific regions          (WIP)|
| 5.  Create a word list for default belkin54g router passwords     (PT)(Both)|
| 6.  Create a word list for default Verizon Wireless Mifi Devices      (Both)|
| 7.  Create a word list with an Crunch interactive prompt system        (GTK)|
| 8.  Create a word list with the Common User Password Profiler (CUPP)   (CLI)|
| 	8a. Replace the CUPP config file with a more thorough config     (CLI)|
|       8b. Restore the CUPP config file with the default config         (CLI)|
| 9.  Create a word list for a specific password policy                  (CLI)|
| 10. Create word lists for a range of dates with Tape's Datelist       (Both)|
|	10a. Create a wordified date word list from scratch w/ Tape's    (CLI)|
|	10b. Wordify an existing word list from Tape's Datelist	         (CLI)|
| 11. Pre-calculate the hashes of a word list for a Cowpatty Attack     (Both)|
| 12. Create a word list from a website with CEWL (requires internet)   (Both)|
| 13. Create a word list for default KooBrick router passwords      (PT)(Both)|
| 14. Create a word list for default Plusnet router passwords       (PT)(Both)|
| 15. Create a word list for default SKY router passwords           (PT)(Both)|
| 16. Create a word list for default Thomson router passwords       (PT)(Both)|
| 17. Create a word list for default ATT 2WIRE router passwords     (PT)(Both)|
| 18. Create a word list for default Comcast Aeris router passwords (PT)(Both)|
| 19. Create a word list for default Huawei Mifi Device passwords   (PT)(Both)|
| 20. Create a word list from a PDF file  				(Both)|
| 21. Create a word list by manipulating a certain word			(Both)|
| 22. Create a word list from directory and file names.			(Both)|
| 23. Create a word list with a customizeable 'random' pattern		(Both)|
| 24. Create a word list for default ATT Mifi device password	    (PT)(Both)|
| 25. Create a word list from an ebook file				(Both)|
| 26. Batch convert multiple PDFs / Ebooks into a word list		(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|---------------------------| GTK Enabled (y/n): $GTK |--------------------------|"
echo "|         PT (Passthrough) = $passthrough (O = Off | 1 = Aircrack | 2 = Pyrit)           |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_phone1 ;;

2) f_phone ;;

3) f_social ;;

4) f_region ;;

5) f_belkin ;;

6) f_vzw ;;

7) f_cruncher ;;

8) f_cupp ;;

8a) f_cuppreplacer ;;

8b) f_cupprestore ;;

9) f_policy ;;

10) f_tapes ;;

10a) f_wordifyscratch ;;

10b) f_wordifyexisting ;;

11) f_genpmk ;;

12) f_cewl ;;

13) f_koobrick ;;

14) f_plusnet ;;

15) f_sky ;;

16) f_thomson ;;

17) f_2wire ;;

18) f_aeris ;;

19) f_huawei ;;

20) f_pdf ;;

21) f_mangleword ;;

22) f_filedir ;;

23) f_random ;;

24) f_att ;;

25) f_ebook ;;

26) f_batchconvert ;;

T|t) f_toggle && f_menu_creation_all ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu creation##
function f_menu_creation () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"--------------------|thad0ctor's word list creation tools|---------------------"
while :

do

cat << !
|                                                                             |
| 1.  Create word lists for default passwords of routers/devices    (PT)(Both)|
| 2.  Create number based word lists (i.e.: Phone,SSN,etc.)         (PT)(Both)|
| 3.  Create text based word lists (i.e.: from PDF,files,etc.)      (PT)(Both)|
| 4.  Create custom lists (i.e.: profile,crunch,random,dates,etc.)  (PT)(Both)|
| 5.  Show all the word list creation tools		            (PT)(Both)|		
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|---------------------------| GTK Enabled (y/n): $GTK |--------------------------|"
echo "|         PT (Passthrough) = $passthrough (O = Off | 1 = Aircrack | 2 = Pyrit)           |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_menu_router ;;

2) f_menu_number ;;

3) f_menu_text ;;

4) f_menu_custom ;;

5) f_menu_creation_all ;;

T|t) f_toggle && f_menu_creation ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu case##
function f_menu_case () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"----------------|thad0ctor's word list case manipulation tools|----------------"
while :

do

cat << !
|                                                                             |
| 1.  Convert a word list to all uppercase characters                   (Both)|
| 2.  Convert a word list to all lowercase characters                   (Both)|
| 3.  Invert the case of characters in a word list			(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Manipulation Menu                                                       |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_uppercase ;;

2) f_lowercase ;;

3) f_invert ;;

T|t) f_toggle && f_menu_case ;;

X|x) f_menu_manipulation ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu manipulation##
function f_menu_clean () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"--------------------|thad0ctor's word list clean up tools|--------------------"
while :

do

cat << !
|                                                                             |
| 1.  Trim a word list to a minimum and maximum character length        (Both)|
| 2.  Sort word lists into alphabetical order w/o non-printable chars   (Both)|
| 3.  Remove double entries from a sorted word list		        (Both)|
| 4.  Split a word list into more manageable pieces                     (Both)|
| 5.  Convert a non-ASCII word list to ASCII				(Both)|
| 6.  Remove the non-ASCII characters from a word list                  (Both)|
| 7.  Remove blank and commented lines from a word list			(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Manipulation Menu                                                       |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_minmax ;;

2) f_sort ;;

3) f_uniq ;;

4) f_split ;;

5) f_ascii ;;

6) f_remascii ;;

7) f_remove ;;

T|t) f_toggle && f_menu_clean ;;

X|x) f_menu_manipulation ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu insert##
function f_menu_insert () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"-------------------|thad0ctor's word list insertion tools|---------------------"
while :

do

cat << !
|                                                                             |
| 1.  Prefix a word list with a string of characters                    (Both)|
| 2.  Append a word list with a string of characters                    (Both)|
| 3.  Insert a string of characters at a position in a word list        (Both)|
| 4.  Add a sequence of numbers to the lines of a word list		(Both)|
| 5.  Add a sequence of characters to the lines of a word list		(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Manipulation Menu                                                       |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

7) f_prefix ;;

8) f_append ;;

9) f_insert ;;

22) f_randomnum ;;

23) f_randomchar ;;

T|t) f_toggle && f_menu_insert ;;

X|x) f_menu_manipulation ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu manipulation##
function f_menu_manip () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"------------------|thad0ctor's word list manipulation tools|-------------------"
while :

do

cat << !
|                                                                             |
| 1.  Manipulate a word list with Tape's Word List Manipulator v3.0      (CLI)|
| 2.  Mangle a word list with John the Ripper's rules                   (Both)|
| 3.  1337ify a word list with a custom settings                        (Both)|
| 4.  Remove/replace character(s) in a word list.                       (Both)|
| 5.  Reverse the order of the characters in a word list 	 	(Both)|
| 6.  Mirror the words of a word list					(Both)|
| 7.  Remove lines from a word list that have a certain string (pattern)(Both)|
| 8.  Double up the lines of a word list				(Both)|
| 9.  Convert the numbers of a word list into words			(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Manipulation Menu                                                       |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_wlm ;;

2) f_john ;;

3) f_1337ify ;;

4) f_replace ;;

5) f_reverse ;;

6) f_mirror ;;

7) f_pattern ;;

8) f_dup ;;

9) f_numberconv ;;

T|t) f_toggle && f_menu_manip ;;

X|x) f_menu_manipulation ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu manipulation all##
function f_menu_manipulation_all () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"------------------|thad0ctor's word list manipulation tools|-------------------"
while :

do

cat << !
|                                                                             |
| 1.  Trim a word list to a minimum and maximum character length        (Both)|
| 2.  Manipulate a word list with Tape's Word List Manipulator v3.0      (CLI)|
| 3.  Sort word lists into alphabetical order w/o non-printable chars   (Both)|
| 4.  Remove double up entries from a sorted word list		        (Both)|
| 5.  Mangle a word list with John the Ripper's rules                   (Both)|
| 6.  Split a word list into more manageable pieces                     (Both)|
| 7.  Prefix a word list with a string of characters                    (Both)|
| 8.  Append a word list with a string of characters                    (Both)|
| 9.  Insert a string of characters at a position in a word list        (Both)|
| 10. 1337ify a word list with a custom settings                        (Both)|
| 11. Convert a word list to all uppercase characters                   (Both)|
| 12. Convert a word list to all lowercase characters                   (Both)|
| 13. Remove/replace character(s) in a word list.                       (Both)|
| 14. Invert the case of characters in a word list			(Both)|
| 15. Convert a non-ASCII word list to ASCII				(Both)|
| 16. Remove the non-ASCII characters from a word list                  (Both)|
| 17. Reverse the order of the characters in a word list 	 	(Both)|
| 18. Mirror the words of a word list					(Both)|
| 19. Remove blank and commented lines from a word list			(Both)|
| 20. Remove lines from a word list that have a certain string (pattern)(Both)|
| 21. Double up the lines of a word list				(Both)|
| 22. Add a sequence of numbers to the lines of a word list		(Both)|
| 23. Add a sequence of characters to the lines of a word list		(Both)|
| 24. Convert the numbers of a word list into words			(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_minmax ;;

2) f_wlm ;;

3) f_sort ;;

4) f_uniq ;;

5) f_john ;;

6) f_split ;;

7) f_prefix ;;

8) f_append ;;

9) f_insert ;;

10) f_1337ify ;;

11) f_uppercase ;;

12) f_lowercase ;;

13) f_replace ;;

14) f_invert ;;

15) f_ascii ;;

16) f_remascii ;;

17) f_reverse ;;

18) f_mirror ;;

19) f_remove ;;

20) f_pattern ;;

21) f_dup ;;

22) f_randomnum ;;

23) f_randomchar ;;

24) f_numberconv ;;

T|t) f_toggle && f_menu_manipulation_all ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu manipulation##
function f_menu_manipulation () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"------------------|thad0ctor's word list manipulation tools|-------------------"
while :

do

cat << !
|                                                                             |
| 1.  Manipulate the case of characters of a word list			(Both)|
| 2.  Tools to clean up / split word lists				(Both)|
| 3.  Tools to insert characters to lines for a word list		(Both)|
| 4.  Various word list manipulation tools				(Both)|
| 5.  Show all the manipulation tools					(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_menu_case ;;

2) f_menu_clean ;;

3) f_menu_insert ;;

4) f_menu_manip ;;

5) f_menu_manipulation_all ;;

T|t) f_toggle && f_menu_manipulation ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##read me##
function f_readme () {
f_echobreak
while :
do
cat docs/README.txt
echo
echo "                 Enter X to return to the info menu: "
read choice

case $choice in

X|x) f_info ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu analyzation##
function f_menu_analyzation () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"------------------|thad0ctor's word list analyzation tools|--------------------"
while :

do

cat << !
|                                                                             |
| 1.  Count the number of lines in a word list                          (Both)|
| 2.  Calculate the time it will take to run through a word list        (Both)|
| 3.  Analyze a word list with dictstat                                 (Both)|
| 4.  Analyze a word list with pipal                                    (Both)|
| 5.  Search a word list for a string of text				(Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_counter ;;

2) f_bench ;;

3) f_dictstat ;;

4) f_pipal ;;

5) f_search ;;

T|t) f_toggle && f_menu_analyzation ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu all##
function f_menu_all_word () {
f_echobreak
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"-------------|thad0ctor's backtrack 5 toolkit wordlist tool list|--------------"
while :

do

cat << !
|                                                                             |
| 1.  Create phone # word list(s) for all active US area codes          (Both)|
| 2.  Create phone # word list(s) for specific area codes           (PT)(Both)|
| 3.  Create a social security # word list for all possible SSNs    (PT)(Both)|
| 4.  Create a social security # word list for specific regions          (WIP)|
| 5.  Create a word list for default belkin54g router passwords     (PT)(Both)|
| 6.  Create a word list for default Verizon Wireless Mifi Devices      (Both)|
| 7.  Create a word list with an Crunch interactive prompt system        (GTK)|
| 8.  Create a word list with the Common User Password Profiler (CUPP)   (CLI)|
| 	8a. Replace the CUPP config file with a more thorough config     (CLI)|
| 	8b. Restore the CUPP config file with the default config         (CLI)|
| 9.  Create a word list for a specific password policy                  (CLI)|
| 10. Create word lists for a range of dates with Tape's Datelist       (Both)|
	10a. Create a wordified date word list from scratch w/ Tape's    (CLI)|
	10b. Wordify an existing word list from Tape's Datelist	         (CLI)|
| 11. Combine word lists from a directory into one file                 (Both)|
| 12. Combine specific word lists from one folder into one file          (GTK)|
| 13. Gather word lists from across your system into one folder         (Both)|
| 14. Trim a word list to a minimum and maximum character length        (Both)|
| 15. Count the number of lines in a word list                          (Both)|
| 16. Manipulate a word list with Tape's Word List Manipulator v3.0      (CLI)|
| 17. Calculate the time it will take to run through a word list        (Both)|
| 18. Pre-calculate the hashes of a word list for a Cowpatty Attack     (Both)|
| 19. Analyze a word list with a set of word list analyzation tools      (CLI)|
| 20. Sort word lists into alphabetical order w/o non-printable chars   (Both)|
| 21. Remove double up entries from a sorted word list	                (Both)|
| 22. Create a word list from a website with CEWL (requires internet)   (Both)|
| 23. Create a word list for default KooBrick router passwords      (PT)(Both)|
| 24. Create a word list for default Plusnet router passwords       (PT)(Both)|
| 25. Create a word list for default SKY router passwords           (PT)(Both)|
| 26. Create a word list for default Thomson router passwords       (PT)(Both)|
| 27. Create a word list for default ATT 2WIRE router passwords     (PT)(Both)|
| 28. Create a word list for default Comcast Aeris router passwords (PT)(Both)|
| 29. Create a word list for default Huawei Mifi Device passwords   (PT)(Both)|
| 30. Mangle a word list with John the Ripper's rules                   (Both)|
| 31. Split a word list into more manageable pieces                     (Both)|
| 32. Prefix a word list with a string of characters                    (Both)|
| 33. Append a word list with a string of characters                    (Both)|
| 34. Insert a string of characters at a position in a word list        (Both)|
| 35. 1337ify a word list with a custom settings                        (Both)|
| 36. Convert a word list to all uppercase characters                   (Both)|
| 37. Convert a word list to all lowercase characters                   (Both)|
| 38. Remove/replace character(s) in a word list.                       (Both)|
| 39. Invert the case of characters in a word list		        (Both)|
| 40. Create a word list from a PDF file				(Both)|
| 41. Convert a non-ASCII word list to ASCII				(Both)|
| 42. Remove the non-ASCII characters from a word list                  (Both)|
| 43. Reverse the order of the characters in a word list         	(Both)|
| 44. Mirror the words of a word list					(Both)|
| 45. Create a word list by manipulating a certain word			(Both)|
| 46. Remove blank and commented lines from a word list			(Both)|
| 47. Remove lines from a word list that have a certain string (pattern)(Both)|
| 48. Search a word list for a string of text				(Both)|
| 49. Create a word list from directory and file names.			(Both)|
| 50. Double up the lines of a word list				(Both)|
| 51. Add a sequence of numbers to the lines of a word list		(Both)|
| 52. Add a sequence of characters to the lines of a word list		(Both)|
| 53. Create a word list with a customizeable 'random' pattern		(Both)|
| 54. Convert the numbers in a word list into words			(Both)|
| 55. Create a word list for default ATT Mifi device password	    (PT)(Both)|
| 56. Create a word list from an ebook file				(Both)|
| 57. Batch convert multiple PDFs / Ebooks into a word list		(Both)|
| C.  Configure / Install this script & shortcuts / Check dependencies        |
| I.  Info / Read Me / Upgrade / Change Log / Debug Info                      |
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|---------------------------| GTK Enabled (y/n): $GTK |--------------------------|"
echo "|         PT (Passthrough) = $passthrough (O = Off | 1 = Aircrack | 2 = Pyrit)           |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_phone1 ;;

2) f_phone ;;

3) f_social ;;

4) f_region ;;

5) f_belkin ;;

6) f_vzw ;;

7) f_cruncher ;;

8) f_cupp ;;

8a) f_cuppreplacer ;;

8b) f_cupprestore ;;

9) f_policy ;;

10) f_tapes ;;

10a) f_wordifyscratch ;;

10b) f_wordifyexisting ;;

11) f_combine ;;

12) f_combiner ;;

13) f_gather ;;

14) f_minmax ;;

15) f_counter ;;

16) f_wlm ;;

17) f_bench ;;

18) f_genpmk ;;

19) f_menu_analyzation ;;

20) f_sort ;;

21) f_unique ;;

22) f_cewl ;;

23) f_koobrick ;;

24) f_plusnet ;;

25) f_sky ;;

26) f_thomson ;;

27) f_2wire ;;

28) f_aeris ;;

29) f_huawei ;;

30) f_john ;;

31) f_split ;;

32) f_prefix ;;

33) f_append ;;

34) f_insert ;;

35) f_1337ify ;;

36) f_uppercase ;;

37) f_lowercase ;;

38) f_replace ;;

39) f_invert ;;

40) f_pdf ;;

41) f_ascii ;;

42) f_remascii ;;

43) f_reverse ;;

44) f_mirror ;;

45) f_mangleword ;;

46) f_remove ;; 

47) f_pattern ;;

48) f_search ;;

49) f_filedir ;;

50) f_dup ;;

51) f_randomnum ;;

52) f_randomchar ;;

53) f_random ;;

54) f_numberconv ;;

55) f_att ;;

56) f_ebook ;;

57) f_batchconvert ;;

C|c) f_configure ;;

I|i) f_info ;;

T|t) f_toggle && f_menu_all_word ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu all##
function f_menu_all () {
f_echobreak
sed -i -e '47s/^\(.\{10\}\)'$firstrun'/\1n/' "LAUNCH_TOOLKIT.sh"
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"-----------------|thad0ctor's backtrack 5 toolkit tool list|-------------------"
while :

do

cat << !
|                                                                             |
| 1.  Create phone # word list(s) for all active US area codes          (Both)|
| 2.  Create phone # word list(s) for specific area codes           (PT)(Both)|
| 3.  Create a social security # word list for all possible SSNs    (PT)(Both)|
| 4.  Create a social security # word list for specific regions          (WIP)|
| 5.  Create a word list for default belkin54g router passwords     (PT)(Both)|
| 6.  Create a word list for default Verizon Wireless Mifi Devices      (Both)|
| 7.  Create a word list with an Crunch interactive prompt system        (GTK)|
| 8.  Create a word list with the Common User Password Profiler (CUPP)   (CLI)|
| 	8a. Replace the CUPP config file with a more thorough config     (CLI)|
| 	8b. Restore the CUPP config file with the default config         (CLI)|
| 9.  Create a word list for a specific password policy                  (CLI)|
| 10. Create word lists for a range of dates with Tape's Datelist       (Both)|
	10a. Create a wordified date word list from scratch w/ Tape's    (CLI)|
	10b. Wordify an existing word list from Tape's Datelist	         (CLI)|
| 11. Combine word lists from a directory into one file                 (Both)|
| 12. Combine specific word lists from one folder into one file          (GTK)|
| 13. Gather word lists from across your system into one folder         (Both)|
| 14. Trim a word list to a minimum and maximum character length        (Both)|
| 15. Count the number of lines in a word list                          (Both)|
| 16. Manipulate a word list with Tape's Word List Manipulator v3.0      (CLI)|
| 17. Calculate the time it will take to run through a word list        (Both)|
| 18. Pre-calculate the hashes of a word list for a Cowpatty Attack     (Both)|
| 19. Analyze a word list with a set of word list analyzation tools     (Both)|
| 20. Sort word lists into alphabetical order w/o non-printable chars   (Both)|
| 21. Remove double up entries from a sorted word list	                (Both)|
| 22. Create a word list from a website with CEWL (requires internet)   (Both)|
| 23. Create a word list for default KooBrick router passwords      (PT)(Both)|
| 24. Create a word list for default Plusnet router passwords       (PT)(Both)|
| 25. Create a word list for default SKY router passwords           (PT)(Both)|
| 26. Create a word list for default Thomson router passwords       (PT)(Both)|
| 27. Create a word list for default ATT 2WIRE router passwords     (PT)(Both)|
| 28. Create a word list for default Comcast Aeris router passwords (PT)(Both)|
| 29. Create a word list for default Huawei Mifi Device passwords   (PT)(Both)|
| 30. Mangle a word list with John the Ripper's rules                   (Both)|
| 31. Split a word list into more manageable pieces                     (Both)|
| 32. Prefix a word list with a string of characters                    (Both)|
| 33. Append a word list with a string of characters                    (Both)|
| 34. Insert a string of characters at a position in a word list        (Both)|
| 35. 1337ify a word list with a custom settings                        (Both)|
| 36. Convert a word list to all uppercase characters                   (Both)|
| 37. Convert a word list to all lowercase characters                   (Both)|
| 38. Remove/replace character(s) in a word list.                       (Both)|
| 39. Invert the case of characters in a word list		        (Both)|
| 40. Create a word list from a PDF file				(Both)|
| 41. Convert a non-ASCII word list to ASCII				(Both)|
| 42. Remove the non-ASCII characters from a word list                  (Both)|
| 43. Reverse the order of the characters in a word list         	(Both)|
| 44. Mirror the words of a word list					(Both)|
| 45. Create a word list by manipulating a certain word			(Both)|
| 46. Remove blank and commented lines from a word list			(Both)|
| 47. Remove lines from a word list that have a certain string (pattern)(Both)|
| 48. Search a word list for a string of text				(Both)|
| 49. Create a word list from directory and file names.			(Both)|
| 50. Double up the lines of a word list				(Both)|
| 51. Add a sequence of numbers to the lines of a word list		(Both)|
| 52. Add a sequence of characters to the lines of a word list		(Both)|
| 53. Create a word list with a customizeable 'random' pattern		(Both)|
| 54. Convert the numbers in a word list into words			(Both)|
| 55. Create a word list for default ATT Mifi device password	    (PT)(Both)|
| 56. Create a word list from an ebook file				(Both)|
| 57. Batch convert multiple PDFs / Ebooks into a word list		(Both)|
| 58. Attack a WPA2 password with a .cap file, Aircrack-NG & a wordlist  (GTK)|
| 59. Attack a WPA2 password using the Crunch passthrough technique      (GTK)|
| 60. Strip only the useful packets out of a capture file with Pyrit     (GTK)|
| C.  Configure / Install this script & shortcuts / Check dependencies        |
| I.  Info / Read Me / Upgrade / Change Log / Debug Info                      |
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|---------------------------| GTK Enabled (y/n): $GTK |--------------------------|"
echo "|         PT (Passthrough) = $passthrough (O = Off | 1 = Aircrack | 2 = Pyrit)           |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_phone1 ;;

2) f_phone ;;

3) f_social ;;

4) f_region ;;

5) f_belkin ;;

6) f_vzw ;;

7) f_cruncher ;;

8) f_cupp ;;

8a) f_cuppreplacer ;;

8b) f_cupprestore ;;

9) f_policy ;;

10) f_tapes ;;

10a) f_wordifyscratch ;;

10b) f_wordifyexisting ;;

11) f_combine ;;

12) f_combiner ;;

13) f_gather ;;

14) f_minmax ;;

15) f_counter ;;

16) f_wlm ;;

17) f_bench ;;

18) f_genpmk ;;

19) f_menu_analyzation ;;

20) f_sort ;;

21) f_unique ;;

22) f_cewl ;;

23) f_koobrick ;;

24) f_plusnet ;;

25) f_sky ;;

26) f_thomson ;;

27) f_2wire ;;

28) f_aeris ;;

29) f_huawei ;;

30) f_john ;;

31) f_split ;;

32) f_prefix ;;

33) f_append ;;

34) f_insert ;;

35) f_1337ify ;;

36) f_uppercase ;;

37) f_lowercase ;;

38) f_replace ;;

39) f_invert ;;

40) f_pdf ;;

41) f_ascii ;;

42) f_remascii ;;

43) f_reverse ;;

44) f_mirror ;;

45) f_mangleword ;;

46) f_remove ;; 

47) f_pattern ;;

48) f_search ;;

49) f_filedir ;;

50) f_dup ;;

51) f_randomnum ;;

52) f_randomchar ;;

53) f_random ;;

54) f_numberconv ;;

55) f_att ;;

56) f_ebook ;;

57) f_batchconvert ;;

58) f_airwpa ;;

59) f_passthrough ;;

60) f_strip ;;

C|c) f_configure ;;

I|i) f_info ;;

T|t) f_toggle && f_menu_all ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##header recent news##
function f_rssnews () {
RSS=https://sourceforge.net/p/thad0ctorstools/blog/feed
wget ${RSS} --no-check-certificate -O - 2>/dev/null | xmlstarlet sel -t -m "/rss/channel/item" -n -o '    [Recent News]           ' -v "title" | sed 's/-0000//' | head -n 2 | sed '/^$/d'
}
##menu header##
function f_menuheader () {
echo '                   __  __	       ______       __
                  / /_/ /_  ____ _____/ / __ \_____/ /_____  _____
                 / __/ __ \/ __ `/ __  / / / / ___/ __/ __ \/ ___/
                / /_/ / / / /_/ / /_/ / /_/ / /__/ /_/ /_/ / /   
                \__/_/ /_/\__,_/\__,_/\____/\___/\__/\____/_/  Kali  '
echo 
echo "    [$today]                [Version: "$scriptversion"]                [$timenow]"
if [ "$internet" = "y" ]; then
if [ "$mainmenu" = "yes" ]; then
	if [ "$scriptversion" \< "$newestversion" ]; then
		echo $RED"                (an update is available: enter U to update now!)" $ECHOCOLOR""
	elif [ "$scriptversion" \> "$newestversion" ]; then
		echo $GREEN"(Devil_D you scored a development version..way to go!ONLY this Ver work in Kali)" $ECHOCOLOR""
	else
		echo $GREEN"                     (your version is currently up to date)" $ECHOCOLOR""
	fi
	if [ "$rssnews" = "y" ]; then
		f_rssnews
	else
		rssnews=n
	fi
else
	mainmenu="n"
fi
else
	internet="n"
	echo $RED"               (you do not currently have an internet connection)" $ECHOCOLOR""
fi
}
##menu wifi##
function f_menu_wifi () {
f_echobreak
while :

do
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"-----------------------|thad0ctor's WPA Attack Toolkit|------------------------"
cat << !
|                                                                             |
| 1.  Attack a WPA2 password with a .cap file, Aircrack-NG & a wordlist  (GTK)|
| 2.  Attack a WPA2 password using the Crunch passthrough technique      (GTK)|
| 3.  Strip only the useful packets out of a capture file with Pyrit     (GTK)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                 Select a operation from the menu: "

read choice

case $choice in

1) f_airwpa ;;

2) f_passthrough ;;

3) f_strip ;;

T|t) f_toggle && f_menu_wifi ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ;;

esac

done
}
##menu wordlist##
function f_menu_word () {
f_echobreak
while :

do
mainmenu="no"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"-------------------|thad0ctor's word list creation toolkit|--------------------"
cat << !
|                                                                             |
| 1.  Word list creation tools (i.e. Phone #'s, SSN's, Dates, & More)   (Both)|
| 2.  Word list combination tools                                       (Both)| 
| 3.  Word list manipulation tools                                      (Both)|
| 4.  Word list analyzation tools                                       (Both)|
| 5.  View all this script's tools                                      (Both)|
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Main Menu                                                               |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: " && read choice
echo
case $choice in

1) f_menu_creation ;;

2) f_menu_combination ;;

3) f_menu_manipulation ;;

4) f_menu_analyzation ;;

5) f_menu_all_word ;;

T|t) f_toggle && f_menu_word ;;

U|u) f_wwwcheck ;;

X|x) f_menu ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
##menu main##
function f_menu () {
f_echobreak
sed -i -e '47s/^\(.\{10\}\)'$firstrun'/\1n/' "LAUNCH_TOOLKIT.sh"
if [ "$GTK" = "x" ]; then
	f_ask
else
	echo
fi
if [ "$bigmenu" = "y" ]; then
	f_menu_all
else
	echo
fi
while :

do
mainmenu="yes"
today=$(date +"%Y-%m-%d")
timenow=$(date +"%r")
f_menuheader
echo $ECHOCOLOR"-----------------------|thad0ctor's backtrack 5 toolkit|-----------------------"
echo $ECHOCOLOR"------------|Created by thad0ctor and modified for Kali by Devil_D|------------"
cat << !
|                                                                             |
| 1.  Wordlist tools (Creation, Manipulation, Combination, Analyzation) (Both)|
| 2.  Wifi tools (strip pcap files, wifi attack tools, etc.)             (GTK)| 
| 3.  View all this script's word list tools                            (Both)|
| C.  Configure / Install script & shortcuts / Check dependencies             |
| I.  Info / Readme / Upgrade / Changelog / Debug Info / Dev. RSS Feed        |
| T.  Toggle between the GTK and CLI versions of this script                  |
| X.  Quit                                                                    |
!
echo "|                           ------------------------                          |"
echo "|                           | GTK Enabled (y/n): $GTK |                          |"
echo "-------------------------------------------------------------------------------"
echo "                        Select an option from the menu: "
read choice
echo
case $choice in

1) f_menu_word ;;

2) f_menu_wifi ;;

3) f_menu_all ;;

C|c) f_configure ;;

I|i) f_info ;;

T|t) f_toggle && f_menu ;;

U|u) f_wwwcheck ;;

X|x) exit ;;

*) echo "\"$choice\" is not valid "; sleep 2 ; f_echobreak ;;

esac

done
}
function f_banner () {
	if [ "$banner" = "y" ]; then
	bash depend/banner.sh
	else
		echo
	fi
}
function f_ask () {
	f_echobreak
	if [ "$GTK" = "x" ];then
		f_asker
	elif [ "$GTK" = "y" ];then
		if [ "$bigmenu" = "y" ]; then
			f_menu_all
		else
			f_menu
		fi
	elif [ "$GTK" = "n" ];then
		if [ "$bigmenu" = "y" ]; then
			f_menu_all
		else
			f_menu
		fi
	else
		f_asker
	fi
}	
##Begin Menu & Loop Forever
function f_asker () {
f_echobreak
if [ "$GTK" != "n" ] || [ "$GTK" != "y" ]; then
	echo "     If you would prefer to run the GTK+ version of this script enter: y"
	echo "     If you would like to run this script from the command line enter: n"
	echo
	echo "         (The GTK+ version of this script requires Gnome and Zenity)"
	echo "           (You can disable this prompt in the configuration menu)"
	echo "             (You may toggle between the two version at any time)"
	echo
	echo "                                    (y/n)"
	echo
	read GTK
	sed -i -e 's/n//g' depend/tempsettings.sh
	sed -i -e 's/y//g' depend/tempsettings.sh
	if [ "$GTK" = "y" ] || [ "$GTK" = "Y" ]; then
		sed -i -e 's/^/y/' depend/tempsettings.sh
		if [ "$bigmenu" = "y" ]; then
			f_menu_all
		else
			f_menu
		fi
	elif [ "$GTK" = "n" ] || [ "$GTK" = "N" ]; then
		sed -i -e 's/^/n/' depend/tempsettings.sh
		if [ "$bigmenu" = "y" ]; then
			f_menu_all
		else
			f_menu
		fi
	else
		echo "You did not input a correct answer (y/n)!"
		echo
		sleep 2
		echo
		echo "Let's try that again."
		echo
		sleep 1
		f_ask
	fi
	f_menu
else
	f_menu
fi
}
function f_start () {
	f_config
	f_color
	echo $ECHOCOLOR""
	f_firstrun
	f_echobreak
	f_banner
	#if [ "$internet" = "y" ]; then
		#f_rssnews
	#else
		#internet="n"
	#fi
	f_quickcheck
	f_autoupdate
	f_ask
}
f_start
