#!/bin/sh
##aircrack-ng wpa attack##
function f_airwpa () {
pcapfile=$(zenity --file-selection --title "Capture file to use" --text "Please Select the capture file you would like to use.\n(e.x. .cap .pcap etc...)")
dicfile=$(zenity --file-selection --title "Dictionary file to use" --text "Please Select the dictionary capture file you would like to use.\n(e.x. .dic .lst .txt etc...)")
cpucore=$(zenity --list --checklist --height=350 --title "CPU Cores" --text "How many CPU cores would you like to use?\n(Select only 1 option)" --column "Select" --column "# of CPU Cores" FALSE 1 FALSE 2 FALSE 3 FALSE 4 FALSE 5 FALSE 6 FALSE 7 FALSE 8)
filename=$(zenity --entry --title "Password Filename" --text "What file would you like to save the cracked password to?\n(The extension .txt will be appended to your selection.)")
zenity --info --text "Please select the destination of $filename.txt"
destination=$(zenity --file-selection --directory --title "Password File Destination" --text "Please select the destintion of the cracked password file.")
zenity --question --no-wrap --title "Proceed?" --text "Would you like to attack the capture file: $pcapfile\nwith the dictionary: $dicfile now?"
if [ "$?" = "0" ]; then
	zenity --info --timeout=4 --text "About to start the attack!"
	aircrack-ng -w $dicfile -p $cpucore -l $destination/$filename.txt $pcapfile
else
	zenity --info --timeout=4 --text "Returning to the main menu..."
	f_menu
fi
}
##pyrit strip##
function f_strip () {
zenity --info --text "Select the capture file you would like to strip."
pcapfile=$(zenity --file-selection --title "Capture file to use" --text "Please Select the capture file you would like to use.\n(e.x. .cap .pcap etc...)")
filename=$(zenity --entry --title "Password Filename" --text "What file would you like to save the stripped capture file to?\n(The extension .pcap will be appended to your selection.)")
zenity --info --text "Please select the destination of $filename.pcap."
destination=$(zenity --file-selection --directory --title "Password File Destination" --text "Please select the destintion of the cracked password file.")
zenity --info --text "About to strip $pcapfile into\n $destination/$filename.pcap"
pyrit -r $pcapfile -o $destination/$filename.pcap strip
zenity --info --text "Finished stripping: $pcapfile\n into: $destination/$filename.pcap\n\nReturning to the main menu."
}
##cruncher passthrough##
function f_passthrough () {
echo
echo
echo
echo
echo
echo
echo
zenity --info --no-wrap --timeout=4 --text "Running the automated Crunch Pass Through script, you will be prompted with several popup questions."
zenity --question --title "Pattern or Standard" --text "Would you like specify a pattern?"
if (( $? == 0 )); then
	zenity --info --text "Select the capture file you would like to use."
	pcapfile=$(zenity --file-selection --title "Capture File" --text "What capture file would you like to use?")
	essid=$(zenity --entry --title "ESSID" --text "Enter the ESSID you would like to attack.")
	filename=$(zenity --entry --title "Password File" --text "Enter the filename you would like to save the cracked password to.\nThe file extension .txt will be appeneded to your entry")
	zenity --info --text "Select the directory you would like to save your password file to."
	destination=$(zenity --file-selection --directory --title "Destination of Password File.")
	max=$(zenity --scale --value=8 --min-value=1 --max-value=64 --step=1 --title "Character Length" --text "Select the character length for the pattern word list.\n(The minimum WPA length is 8 characters)")
	pattern=$(zenity --title "Pattern" --entry --text "Select the pattern you would like for your wordlist.\n@ = Lowercase Letters\n, = Uppercase Letters\n % = Numbers\n^ = Symbols\n(The pattern must be $max characters in length.)")
	zenity --question --title "Make Wordlist" --text "Would you like to crunch and pass through attack?"
	if (( $? == 0 )); then
		zenity --info --timeout=4 --text "About to attack the network: $essid."
		/usr/bin/crunch $max $max -t $pattern | aircrack-ng -l $destination/$filename.txt -w - -e "$essid" "$pcapfile"
		zenity --info --timeout=4 --text "Returning to the main menu."
	else
	f_passthrough
	fi
else
	zenity --info --text "Select the capture file you would like to use."
	pcapfile=$(zenity --file-selection --title "Capture File" --text "What capture file would you like to use?")
	essid=$(zenity --entry --title "ESSID" --text "Enter the ESSID you would like to attack.")
	filename=$(zenity --entry --title "Password File" --text "Enter the filename you would like to save the cracked password to.\nThe file extension .txt will be appeneded to your entry")
	zenity --info --text "Select the directory you would like to save your password file to."
	destination=$(zenity --file-selection --directory --title "Destination of Password File.")
	min=$(zenity --scale --value=8 --min-value=1 --max-value=64 --step=1 --title "Character Length" --text "Select the min. character length for the pattern word list.\n(The minimum WPA length is 8 characters)")
	max=$(zenity --scale --value=8 --min-value=1 --max-value=64 --step=1 --title "Character Length" --text "Select the max. character length for the pattern word list.\n(The maximum WPA length is 64 characters)")
	chars=$(zenity --title  "Charset" --entry --text "Select the characters you would like to use for the word list. (no spaces, space is forward slash)")
	zenity --question --title "Make Wordlist" --text "Would you like to Crunch and pass through attack?"
	if (( $? == 0 )); then
		zenity --info --timeout=4 --text "About to attack the network: $essid."
		/usr/bin/crunch $min $max $chars | aircrack-ng -l $destination/$filename.txt -w - -e "$essid" "$pcapfile"
		zenity --info --timeout=4 --text "Returning to the main menu."
	else
	f_passthrough
	fi
fi
}
