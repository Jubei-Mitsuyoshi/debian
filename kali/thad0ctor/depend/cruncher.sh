#!/bin/sh
function f_crunch () {
echo
echo
echo
echo
echo
echo
echo
echo "If you would prefer to run the GTK version of this script please enter: y"
echo "If you would like to run the command line version of this script enter: n"
echo "(The GTK+ version of this script requires Gnome and zenity)"
echo
sleep 1
echo "Press enter if you would like to go back to the main menu"
echo
echo "(y/n)"
echo
read gtk
if [ "$gtk" = "y" ]; then
		f_gtk
	elif [ "$gtk" = "n" ]; then
		f_cli
	else
	echo
	echo "Returning to the main menu."
	sleep 2
fi
}
function f_gtk () {
echo
echo
echo
echo
echo
echo
echo
zenity --info --timeout=4 --text "Running the automated Crunch script, you will be prompted with several popup questions."
zenity --question --title "Pattern or Standard" --text "Would you like specify a pattern?"
if (( $? == 0 )); then
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place the end product?")
	filename=$(zenity --entry --title "File Names" --text "What to name the new wordlist file?\n(The file will be saved with the .lst extension)")
	max=$(zenity --scale --value=8 --min-value=1 --max-value=64 --step=1 --title "Character Length" --text "Select the character length for the pattern word list.\n(The minimum WPA length is 8 characters)")
	pattern=$(zenity --title "Pattern" --entry --text "Select the pattern you would like for your wordlist.\n@ = Lowercase Letters\n, = Uppercase Letters\n % = Numbers\n^ = Symbols\n(The pattern must be $max characters in length.)")
	zenity --question --title "Make Wordlist" --text "Would you like to create the wordlist: $filename.lst"
	if (( $? == 0 )); then
		zenity --info --timeout=4 --text "About to generate your wordlist."
		/usr/bin/crunch $max $max -t $pattern -o $filename.lst
		zenity --info --timeout=4 --text "Moving $filename.lst to the directory you selected ($destination)."
		mv $filename.lst "$destination"
		zenity --info --timeout=4 --text "$filename.lst has been moved $destination"
		zenity --info --timeout=4 --text "Returning to the main menu."
	else
	f_crunch
	fi
else
	destination=$(zenity --file-selection --directory --title "File Destination" --text "Where would you like to place the end product?")
	filename=$(zenity --entry --title "File Names" --text "What to name the new wordlist file?\n(The file will be saved with the .lst extension)")
	min=$(zenity --scale --value=8 --min-value=1 --max-value=64 --step=1 --title "Character Length" --text "Select the character length for the pattern word list.\n(The maximum WPA length is 8 characters)")
	max=$(zenity --scale --value=8 --min-value=1 --max-value=64 --step=1 --title "Character Length" --text "Select the character length for the pattern word list.\n(The maximum WPA length is 64 characters)")
	chars=$(zenity --title  "Charset" --entry --text "Select the characters you would like to use for the word list. (no spaces, space is forward slash)")
	zenity --question --title "Make Wordlist" --text "Would you like to create the wordlist: $filename.lst"
	if (( $? == 0 )); then
		zenity --info --timeout=4 --text "About to generate your wordlist."
		/usr/bin/crunch $min $max $chars -o $filename.lst
		zenity --info --timeout=4 --text "Moving $filename.lst to the directory you selected ($destination)."
		mv $filename.lst "$destination"
		zenity --info --timeout=4 --text "$filename.lst has been moved $destination"
		zenity --info --timeout=4 --text "Returning to the main menu."
	else
	f_crunch
	fi
fi
}
function f_cli () {
echo work in progress
}
f_crunch



