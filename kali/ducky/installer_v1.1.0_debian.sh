#!/bin/bash
# Simple-Ducky Payload Generator Installer
# Last Updated: 17 May 2013
# Author: Travis Weathers (skysploit) | skysploit@gmail.com

#######################################################
# Simple-Ducky Download
#######################################################
f_installer(){
clear
echo -e "\e[1;34m[*] Please wait while I download and install the Simple-Ducky Payload Generator\e[0m"
echo ""
sleep 4
wget https://simple-ducky-payload-generator.googlecode.com/files/simple-ducky-v1.1.0-debian.tar.gz
tar zxvf simple-ducky-v1.1.0-debian.tar.gz -C /usr/share
ln -s /usr/share/ducky/encoder/simple-ducky.sh /usr/bin/simple-ducky
rm simple-ducky-v1.1.0-debian.tar.gz
echo ""
echo -e "\e[1;32m[+] The Simple-Ducky installed sucessfully.\e[0m"
echo ""
sleep 4
clear

echo -e "\e[1;34m[*] Performing an APT Update prior to installing dependencies...\e[0m\n"
sleep 3
apt-get update
echo ""
echo -e "\e[1;32m[+] APT Update complete...\e[0m"
sleep 3
clear

echo -e "\e[1;34m[*] Please wait while I install some dependencies...\e[0m\n"
sleep 3
updatedb
mkdir /tmp/simple-ducky/
echo ""

	machine=$(cat /etc/issue)
	if [ "$machine" == "Kali GNU/Linux 1.0 \n \l" ]; then
		echo -e "\n\e[1;34m[*] I see that you are using Kali-Linux. This will only take a few moments...\e[0m"
		echo ""
		sleep 3
		f_kaliinstall
	else
		f_otherdebian
	fi

}

########################################################
# Kali Install
########################################################
f_kaliinstall(){
	reqs="pure-ftpd dfu-programmer burpsuite"
	for i in $reqs; do
		dpkg -s "$i" &> /tmp/simple-ducky/$i-install.txt
		isinstalled=$(cat /tmp/simple-ducky/$i-install.txt | grep -o "Status: install ok installed")
		if [ ! -e /usr/bin/$i ] && [ ! -e /usr/sbin/$i ] && [ ! -e /usr/local/sbin/$i ] && [ ! -e /usr/local/bin/$i ] && [ -z "$isinstalled" ]; then
				echo -e "\e[1;33m[-] It doesn't appear that $i is installed on your system. Installing it now...\e[0m"
				echo ""
			if [ ! -z $(apt-get install -y "$i" | grep -o "E: Couldn") ]; then
				echo -e "\e[1;31m[-] I had a hard time installing $i from the Kali-Linux repository.\e[0m"
				touch /tmp/simple-ducky/$i-fail
			else
				dpkg -s "$i" &> /tmp/simple-ducky/$i-install.txt
				isinstalled=$(cat /tmp/simple-ducky/$i-install.txt | grep -o "Status: install ok installed")				
				if [ ! -z "$isinstalled" ]; then
					update=1
					echo -e "\e[1;32m[+] Good news, $i installed without any issues.\e[0m"
					echo ""
					sleep 2
				else
					echo ""
					echo -e "\e[1;31m[!] It doesn't appear that I will be able to install $i right now.\e[0m"
					echo ""
					sleep 2
				fi
			fi
		else
			echo -e "\e[1;32m[+] $i is already installed on your system, moving on...\e[0m"
			echo ""
			sleep 2
		fi
	done
f_java

}

########################################################
# Other Debian Install
########################################################
f_otherdebian(){
	reqs="pure-ftpd file-roller dfu-programmer apache2 gnome-terminal burpsuite git wget netcat p7zip-full nautilus john"
	for i in $reqs; do
		dpkg -s "$i" &> /tmp/simple-ducky/$i-install.txt
		isinstalled=$(cat /tmp/simple-ducky/$i-install.txt | grep -o "Status: install ok installed")
		if [ ! -e /usr/bin/$i ] && [ ! -e /usr/sbin/$i ] && [ ! -e /usr/local/sbin/$i ] && [ ! -e /usr/local/bin/$i ] && [ -z "$isinstalled" ]; then
				echo -e "\e[1;33m[-] It doesn't appear that $i is installed on your system. Installing it now...\e[0m"
				echo ""
			if [ ! -z $(apt-get install -y "$i" | grep -o "E: Couldn") ]; then
				echo -e "\e[1;31m[-] I had a hard time installing $i from the Kali-Linux repository.\e[0m"
				touch /tmp/simple-ducky/$i-fail.txt
			else
				dpkg -s "$i" &> /tmp/simple-ducky/$i-install.txt
				isinstalled=$(cat /tmp/simple-ducky/$i-install.txt | grep -o "Status: install ok installed")				
				if [ ! -z "$isinstalled" ]; then
					update=1
					echo -e "\e[1;32m[+] Good news, $i installed without any issues.\e[0m"
					echo ""
					sleep 2
				else
					echo ""
					echo -e "\e[1;31m[!] It doesn't appear that I will be able to install $i right now.\e[0m"
					echo ""
					sleep 2
				fi
			fi
		else
			echo -e "\e[1;32m[+] $i is already installed on your system, moving on...\e[0m"
			echo ""
			sleep 2
		fi
	done

f_metasploit
}

########################################################
# Metasploit
########################################################	
f_metasploit(){
clear
echo -e "\e[1;34m[*] Checking to see if Metasploit is installed...\e[0m\n"
if [ ! -e /usr/bin/msfconsole ] && [ ! -e /usr/sbin/msfconsole ] && [ ! -e /usr/local/sbin/msfconsole ] && [ ! -e /usr/local/bin/msfconsole ]; then
	update=1
	echo -e "\n\e[1;34m[*] It doesn't appear that Metasploit is installed on your system. Installing it now...\e[0m"
	echo ""
	machine=$(uname -m)
	if [ "$machine" == "x86_64" ]; then
		wget http://downloads.metasploit.com/data/releases/metasploit-latest-linux-x64-installer.run -O /tmp/simple-ducky/metasploit-latest-linux-x64-installer.run
		echo -e "\n\e[1;33m[*] Launching the Metasploit installer. Select all the defaults and DONT launch the web UI...\e[0m"
		echo ""
		sleep 3
		chmod 755 /tmp/simple-ducky/metasploit-latest-linux-x64-installer.run
		/tmp/simple-ducky/metasploit-latest-linux-x64-installer.run
	else
		wget http://downloads.metasploit.com/data/releases/metasploit-latest-linux-installer.run -O /tmp/simple-ducky/metasploit-latest-linux-installer.run
		echo -e "\n\e[1;33m[*] Launching the Metasploit installer. Select all the defaults and DONT launch the web UI...\e[0m"
		echo ""
		sleep 3
		chmod 755 /tmp/simple-ducky/metasploit-latest-linux-installer.run
		/tmp/simple-ducky/metasploit-latest-linux-installer.run
	fi

	cd /usr/bin
	msfprogs="msfconsole msfupdate msfencode msfpayload"
	for z in $msfprogs; do
		if [ ! -e /usr/bin/$z ]; then
			ln -f -s /usr/local/bin/$z $z
		fi
	done
fi
echo -e "\e[1;32m[+] Good news, Metasploit installed without any issues.\e[0m"
echo ""
sleep 2

f_setoolkit
}

########################################################
# SE-Toolkit
########################################################	
f_setoolkit(){
clear
echo -e "\e[1;34m[+] Checking to see if the Social Engineering Toolkit is installed...\e[0m\n"
echo ""
sleep 4
	dpkg -s "set" &> /tmp/simple-ducky/set-install.txt
	setoolkit=$(cat /tmp/simple-ducky/set-install.txt | grep -o "Status: install ok installed")
	if [ "$setoolkit" == "Status: install ok installed" ]; then
	echo -e "\e[1;32m[+]  The SE-Toolkit is already installed on your system, moving on...\e[0m"
		echo ""
		sleep 4
	else			
		echo -e "\e[1;33m[-] It doesn't appear that the SE-Toolkit is installed on your system. Installing it now...\e[0m"
		echo -e ""
		sleep 3
		git clone https://github.com/trustedsec/social-engineer-toolkit/ /tmp/simple-ducky/set/
		chmod 755 /tmp/simple-ducky/set/setup.py
		python /tmp/simple-ducky/set/setup.py install
		echo ""
		echo -e "\e[1;32m[+] Good news, the SE-Toolkit installed without any issues.\e[0m"
		echo ""
		sleep 2
	fi


bsuite=$(cat /tmp/simple-ducky/burpsuite-install.txt | grep -o "is not installed" )
	if [ "$bsuite" == "is not installed" ]; then
		f_burpsuite
	else
		f_java
fi
}

########################################################
# Burpsuite
#######################################################
f_burpsuite(){
	clear
	echo -e "\e[1;34mTrying a different approach to install Burpsuite...\e[0m\n"
	echo ""
	sleep 3
	mkdir /usr/share/burpsuite/
	wget http://portswigger.net/burp/burpsuite_free_v1.5.jar -O /usr/share/burpsuite/burpsuite.jar
	chmod 755 /usr/share/burpsuite/burpsuite.jar
	ln -s /usr/share/burpsuite/burpsuite.jar /usr/bin/burpsuite.jar
	echo ""
	echo -e "\e[1;32m[+] Good news, Burpsuite installed without any issues.\e[0m"
	sleep 3
	clear
f_java
}

########################################################
# JavaInstall
########################################################
f_java(){
clear
echo -e "\e[1;34m[+] Checking your JDK version, I will update it if needed...\e[0m\n"
echo ""
sleep 4
	java -version &> /tmp/simple-ducky/java-version.txt
	javainstall=$(cat /tmp/simple-ducky/java-version.txt | grep -o "1.7.0")
	if [ "$javainstall" == "1.7.0" ]; then
		echo -e "\e[1;32m[+] It looks like your JDK is up to date, moving on..."
		sleep 4
	else			
		echo -e "\e[1;33m[+] It looks like we need to update JDK to version 1.7.0\e[0m"
		echo -e ""
		sleep 3
		apt-get install -y openjdk-7-jre-headless
		echo ""
		echo -e "\e[1;33m[*] When prompted select the option for: '...java-7-openjdk...'\e[0m"
		echo ""
		sleep 4
		update-alternatives --config java
		echo ""
		echo -e "\e[1;32m[+] Your new JDK version is...\e[0m"
		echo ""
		java -version
		sleep 5
		clear
	fi
	
f_ftpconfig
}

########################################################
# Alternate John Installer (Disabled)
########################################################
f_johninstall(){
	echo -e "\n\e[1;34m[*] It doesn't appear that John is installed on your system. Installing it now...\e[0m"
	echo ""
	machine=$(uname -m)
	if [ "$machine" == "x86_64" ]; then
		wget http://www.openwall.com/john/g/john-1.7.9-jumbo-7.tar.gz -O /tmp/simple-ducky/john-1.7.9-jumbo-7.tar.gz
		mkdir /usr/share/john/
		cd /tmp/simple-ducky/
		tar zxvf /tmp/simple-ducky/john-1.7.9-jumbo-7.tar.gz
		cd john-1.7.9-jumbo-7/
		cp * /usr/share/john/
		cd /usr/share/john/src/
		make
		make clean generic
		ln -s /usr/share/john/run/john /usr/bin/john
		touch /usr/bin/john.ini
		touch /usr/share/john/john.ini
		touch /root/john.ini
	else
		wget http://www.openwall.com/john/g/john-1.7.9-jumbo-7.tar.gz -O /tmp/simple-ducky/john-1.7.9-jumbo-7.tar.gz
		mkdir /usr/share/john/
		cd /tmp/simple-ducky/
		tar zxvf /tmp/simple-ducky/john-1.7.9-jumbo-7.tar.gz
		cd john-1.7.9-jumbo-7/
		cp * /usr/share/john/
		cd /usr/share/john/src/
		make
		make clean generic
		ln -s /usr/share/john/run/john /usr/bin/john
		touch /usr/bin/john.ini
		touch /usr/share/john/john.ini
		touch /root/john.ini
	fi
	echo ""
	echo -e "\e[1;32m[+] Good news, Burpsuite installed without any issues.\e[0m"
	sleep 3
	clear
	
f_ftpconfig
}

##################################################################
# FTP Configuration
##################################################################		
f_ftpconfig(){
clear
echo -e "\e[1;34mChecking to see if pure-ftpd has been configured. I will walk you through configuration if it is not...\e[0m\n"
echo ""
sleep 4
	cat /etc/group  &> /tmp/simple-ducky/ftp-group1.txt
	cat /etc/group  &> /tmp/simple-ducky/ftp-group2.txt
	ftpconfig1=$(cat /tmp/simple-ducky/ftp-group1.txt | grep -o "ftpgroup:x:1000")
	ftpconfig2=$(cat /tmp/simple-ducky/ftp-group2.txt | grep -o "ftpgroup:x:1001")
	if [ "$ftpconfig1" == "ftpgroup:x:1000" ]; then
		echo -e "\e[1;32m[+] I see that you have already configured pure-ftpd, moving on...\e[0m"
		echo ""
		sleep 4
	elif [ "$ftpconfig2" == "ftpgroup:x:1001" ]; then
		echo -e "\e[1;32m[+] I see that you have already configured pure-ftpd, moving on...\e[0m"
		echo ""
		sleep 4
	else
		echo -e "\e[1;33m[*] It doesn't appear that you have setup pure-ftpd... Let's start!\e[0m"
		echo ""
		read -p "[-] Who would you like the primary user to be? " ftpusername
		echo ""
		echo -e "\e[1;33m[-] Configuring pure-FTPD for: $ftpusername\e[0m"
		echo ""
		sleep 3
		groupadd ftpgroup
		useradd -g ftpgroup -d /dev/null -s /etc ftpuser
		echo -e "\e[1;33m[-] Please set the password for $ftpusername.\e[0m"
		pure-pw useradd $ftpusername -u ftpuser -d /ftphome
		pure-pw mkdb
		cd /etc/pure-ftpd/auth/
		ln -s ../conf/PureDB 60pdb
		echo ""
		echo -e "\e[1;33m[-] Creating your home directory, it will reside at /ftphome/\e[0m"
		echo ""
		sleep 3
		mkdir /ftphome
		chown -R ftpuser:ftpgroup /ftphome/
		echo -e "\e[1;33m[-] Starting the FTP server.\e[0m"
		echo ""
		sleep 2
		service pure-ftpd restart
		echo ""
		echo -e "\e[1;32m[+] Done! To test your new account, in a new terminal type: ftp 127.0.0.1\e[0m"
		echo ""
		read -p "Press any key to contiue" enter	
	fi
f_cleanupexit
}

#################################################################################
# Cleanup and exit
#################################################################################
f_cleanupexit(){
	clear
	echo ""
	echo -e "\e[1;32m[+] The installation process is complete!\e[0m"
	echo ""
	echo -e "\e[1;32m[+] Type: simple-ducky in the terminal to launch...\e[0m"
	echo ""
	read -p "Press any key to continue" continue
	rm -rf /tmp/simple-ducky/
	clear
	exit
}

#################################################################################
# Run as Root Query
#################################################################################
resize -s 35 115
cd /usr/share/ducky/encoder
if [ "$(id -u)" != "0" ]; then
	echo -e "\e[1;31m[!] This script must be run as root\e[0m" 1>&2
	exit 1
else
	f_installer
fi
