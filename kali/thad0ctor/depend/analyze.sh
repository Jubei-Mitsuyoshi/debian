#!/bin/sh
##count lines GTK##
function f_counterGTK () {
	zenity --info --timeout=4 --text "This script will count the number of lines in a word list file."
	countlist=$(zenity --file-selection --title "Wordlist to count" --text "Select the word list file you would like to analyze.")
	count=$(wc -l "$countlist" | awk -F " "  '{print$1}')
	zenity --info --no-wrap --text "The word list: $countlist\n has $count lines"
	f_menu
}
##count lines##
function f_counter () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_counterGTK
	else
		echo "This script will count the number of lines in a word list file."
		echo
		sleep 1
		echo "Enter the full path the the wordlist file you would like to analyze."
		echo
		read countlist
		count=$(wc -l "$countlist" | awk -F " "  '{print$1}')
		echo
		echo "The word list: $countlist has $count lines"
		sleep 10
		echo "Hit enter to return to the main menu"
		echo
		read gotomenu
		if [ "$gotomenu" = "" ]; then
			f_menu
		else
			f_menu
		fi	
	fi
}
##benchmarker GTK##
function f_benchGTK () {
	zenity --info --no-wrap --text "Please select the word list you would like to use for the calculation."
	wordfile=$(zenity --file-selection --title "Word List to Calculate.")
	zenity --info --timeout=4 --no-wrap --title "Calculating..." --text "Calculating the word count for $wordfile.\nThis may take a while depeding on its size..."
	count=$(wc -l $wordfile | awk -F " " '{print$1}')
	program=$(zenity --list --checklist --title "Program to Benchmark" --text "Select the program you would like to use to benchmark:\n$wordfile\n\n(Select Only One)" --column "Select" --column "Program" FALSE "Pyrit" FALSE "Aircrack-NG")
	if [ "$program" = "Pyrit" ]; then
		zenity --info --timeout=4 --text "About to run the Pyrit benchmark."
		kps=$(pyrit benchmark | grep Computed | awk -F " " '{print$2}' | cut -d "." -f1)
		let totals=$count/$kps
		let totalm=$totals/60
		let totalh=$totalm/60
		zenity --info --no-wrap --text "$wordfile ($count lines)\n will take $totalh hours (i.e. $totalm minutes) to run through\n with Pyrit at $kps keys per second."
		f_menu
	elif [ "$program" = "Aircrack-NG" ]; then
		zenity --info --timeout=4 --text "About to run the Aircrack-NG benchmark."
		kps=$(aircrack-ng -S | awk -F " " '{print$1}')
		let totals=$count/$kps
		let totalm=$totals/60
		let totalh=$totalm/60
		zenity --info --no-wrap --text "$wordfile ($count lines)\n will take $totalh hours (i.e. $totalm minutes) to run through\n with Aircrack-NG at $kps keys per second."
		f_menu
	else
		zenity --error --text "You did not select a correct answer.\nStarting Over..."
		f_bench
	fi
}
##benchmarker##
function f_bench () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_benchGTK
	else
		f_echobreak
		echo "Please enter the full path to the word list you would like to use."
		echo "(e.x. /root/Desktop/wordlist.lst)"
		echo
		read wordfile
		echo
		echo "Calculating the word count for $wordfile."
		echo "This may take a while depeding on its size..."
		count=$(wc -l $wordfile | awk -F " " '{print$1}')
		f_echobreak
		echo "1) Pyrit"
		echo "2) Aircrack-NG"
		echo
		echo "Select the program you would like to calculate the time to run through the list:"
		echo $wordfile
		echo
		read program
		if [ "$program" = "1" ]; then
			f_echobreak
			echo "About to run the Pyrit benchmark"
			echo
			kps=$(pyrit benchmark | grep Computed | awk -F " " '{print$2}' | cut -d "." -f1)
			let totals=$count/$kps
			let totalm=$totals/60
			let totalh=$totalm/60
			echo
			echo "$wordfile ($count lines) will take $totalh hours"
			echo "(i.e. $totalm minutes) to run through with Pyrit at $kps keys per second."
			echo
			sleep 10
			echo "Hit enter to return to the main menu"
			echo
			read gotomenu
			if [ "$gotomenu" = "" ]; then
				f_menu
			else
				f_menu
			fi
		elif [ "$program" = "2" ]; then
			f_echobreak
			echo "About to run the Aircrack-NG benchmark"
			echo
			kps=$(aircrack-ng -S | awk -F " " '{print$1}')
			let totals=$count/$kps
			let totalm=$totals/60
			let totalh=$totalm/60
			echo
			echo "$wordfile ($count lines) will take $totalh hours"
			echo "(i.e. $totalm minutes) to run through with Aircrack-NG at $kps keys per second."
			echo
			sleep 10
			echo "Hit enter to return to the main menu"
			echo
			read gotomenu
			if [ "$gotomenu" = "" ]; then
				f_menu
			else
				f_menu
			fi
		else
			echo "You did not enter the correct answer, starting over..."
			sleep 4
			f_bench
		fi
	fi
}
##dictstat##
function f_dictstat () {
	f_echobreak
	if [ "$GTK" = "y" ]; then
		f_dictstatgtk
	else
	echo "About to analyze a word list with dictstat."
	echo "This may take a while..."
	echo
	sleep 1
	echo "Enter the full name and path of the wordlist you would like analyze."
	echo "(e.x. /root/Desktop/Wordlist.lst)"
	echo
	read filename
	echo
	echo "Would you like to dictstat analyze: $filename? (y/n)"
	echo
	read proceed
	if [ "$proceed" = "y" ]; then
		LOG1="$filename"_report_dictstat1.txt
		LOG="$filename"_report_dictstat.txt
		python /pentest/passwords/pack/dictstat.py "$filename" 2>&1 | tee $LOG1
		sed '/Psyco/d' $LOG1 > $LOG
		rm $LOG1
		echo
		echo "The output report has been saved in the word list directory under: "$filename"_report_dictstat.txt"
		echo
		echo " Would you like to analyze another list? (y/n)"
		echo "(Answering n will return you to the main menu.)"
		echo
		read contin
		if [ "$contin" = "y" ]; then
			f_dictstat
		else
			f_menu
		fi
	else
		echo
		echo "You chose not to analyze $filename."
		echo
		sleep 2
		echo "Returning to the main menu..."
		echo
		sleep 2
		f_menu
	fi
fi
}
##dictstat gtk##
function f_dictstatgtk () {
	f_echobreak
	zenity --info --timeout=5 --text "About to analyze a word list with dictstat.\nThis may take a while..."
	filename=$(zenity --file-selection --title "Word list to analyze")
	zenity --question --title "Analyze Wordlist" --text "Would you like to dictstat analyze: $filename? (y/n)"
	if (( $? == 0 )); then
		LOG1="$filename"_report_dictstat1.txt
		LOG="$filename"_report_dictstat.txt
		python /pentest/passwords/pack/dictstat.py "$filename" 2>&1 | tee $LOG1
		sed '/Psyco/d' $LOG1 > $LOG
		rm $LOG1
		echo
		zenity --info --timeout=5 --text "The output report has been saved in the word list directory under: "$filename"_report_dictstat.txt"
		zenity --question --title "Analyze Wordlist" --text "Would you like to analyze another list? (y/n)\n(Answering n will return you to the main menu.)"
		if (( $? == 0 )); then
			f_dictstat
		else
			f_menu
		fi
	else
		zenity --info --timeout=5 --text "You chose not to analyze $filename."
		zenity --info --timeout=5 --text "Returning to the main menu..."
		f_menu
	fi
}
##pipal gtk##
function f_pipalgtk () {
	f_echobreak
	zenity --info --timeout=5 --text "About to analyze a word list with pipal.\nThis will take quite a while..."
	filename=$(zenity --file-selection --title "Word list to analyze")
	zenity --info --text "Enter the top X results you would like to review.\n(i.e. 25, The default (blank) is set to 10)"
	top=$(zenity --entry --title "# of results")
	zenity --question --title "Analyze Wordlist" --text "Would you like to pipal analyze: $filename? (y/n)"
	if (( $? == 0 )); then
		LOG1="$filename"_report_pipal1.txt
		LOG="$filename"_report_pipal.txt
		if [ "$top" = "" ]; then
			bash -c "cd /pentest/passwords/pipal/ && ./pipal.rb "$filename"" 2>&1 | tee $LOG1
		else
			bash -c "cd /pentest/passwords/pipal/ && ./pipal.rb -t $top "$filename"" 2>&1 | tee $LOG1
		fi
		sed '/Processing/d' $LOG1 > $LOG
		sed '/Generating stats/d' $LOG > $LOG1
		sed '/Please wait/d' $LOG1 > $LOG
		sed '1d' $LOG > $LOG1
		sed '1d' $LOG1 > $LOG
		rm $LOG1
		zenity --info --timeout=5 --text "The output report has been saved in the word list directory under: "$filename"_report_pipal.txt"
		zenity --question --title "Analyze Wordlist" --text "Would you like to analyze another list? (y/n)\n(Answering n will return you to the main menu.)"
		if (( $? == 0 )); then
			f_pipal
		else
			f_menu
		fi
	else
		zenity --info --timeout=5 --text "You chose not to analyze $filename."
		zenity --info --timeout=5 --text "Returning to the main menu..."
		f_menu
	fi
}
##pipal##
function f_pipal () {
	if [ "$GTK" = "y" ]; then
		f_pipalgtk
	else
	f_echobreak
	echo "About to analyze a word list with pipal."
	echo "This will take quite a while..."
	echo
	sleep 2
	echo "Enter the full name and path of the wordlist you would like analyze."
	echo "(e.x. /root/Desktop/Wordlist.lst)"
	echo
	read filename
	echo
	echo "Enter the top X results you would like to review."
	echo " (i.e. 25, The default (blank) is set to 10)"
	echo
	read top
	echo
	echo
	echo "Would you like to pipal analyze: $filename? (y/n)"
	echo
	read proceed
	if [ "$proceed" = "y" ]; then
		LOG1="$filename"_report_pipal1.txt
		LOG="$filename"_report_pipal.txt
		if [ "$top" = "" ]; then
			bash -c "cd /pentest/passwords/pipal/ && ./pipal.rb "$filename"" 2>&1 | tee $LOG1
		else
			bash -c "cd /pentest/passwords/pipal/ && ./pipal.rb -t $top "$filename"" 2>&1 | tee $LOG1
		fi
		sed '/Processing/d' $LOG1 > $LOG
		sed '/Generating stats/d' $LOG > $LOG1
		sed '/Please wait/d' $LOG1 > $LOG
		sed '1d' $LOG > $LOG1
		sed '1d' $LOG1 > $LOG
		rm $LOG1
		echo
		echo "The output report has been saved in the word list directory under: "$filename"_report_pipal.txt"
		echo
		sleep 1
		echo " Would you like to analyze another list? (y/n)"
		echo "(Answering n will return you to the main menu.)"
		echo
		read contin
		if [ "$contin" = "y" ]; then
			f_pipal
		else
			f_menu
		fi
	else
		echo
		echo "You chose not to analyze $filename."
		echo
		sleep 2
		echo "Returning to the main menu..."
		echo
		sleep 2
		f_menu
	fi
fi
}
##search for a string##
function f_search () {
if [ "$GTK" = "y" ]; then
	f_searchgtk
else
	echo "About to search a word list for a string of text."
	echo
	sleep 2
	echo "Please select the word list you would like to search through."
	echo "(e.x. /root/Desktop/wordlist.lst)"
	echo
	read inputlist
	echo
	if [ -e "$inputlist" ]; then
		echo "$inputlist exists"
		echo
		sleep 2
		echo "Continuing..."
		sleep 1
	else
		echo "$inputlist does not exist..."
		echo
		sleep 2
		echo "Starting over..."
		echo
		sleep 3
		f_search
	fi
	echo
	echo "Please enter the string of text to search for."
	echo
	sleep 1
	read string
	echo
	echo "Working..."
	echo
	sleep 2
	foundstrings=$(cat $inputlist | grep -n "$string")
	foundstring=$(cat $inputlist | grep "$string")
	foundnum=$(cat $inputlist | sed -n '/'"$string"'/=')
	if [ "$foundstring" = "" ]; then
		echo "The string: $string was not found in $inputlist"
		echo
		sleep 5
		echo "Returning to the main menu..."
		echo
		sleep 5
		f_menu
	elif [ "$foundstring" = "$string" ]; then
		echo "The string: $string was found in $inputlist"
		echo "at line: $foundnum"
		echo
		sleep 5
		echo "Hit enter to return to the main menu"
		echo
		read gotomenu
		if [ "$gotomenu" = "" ]; then
			f_menu
		else
			f_menu
		fi
	else
		echo "The string: $string was found in $inputlist "
		echo "as a substring of:"
		echo
		echo "Line : String"
		echo "$foundstrings"
		echo
		sleep 5
		echo "Hit enter to return to the main menu"
		echo
		read gotomenu
		if [ "$gotomenu" = "" ]; then
			f_menu
		else
			f_menu
		fi
	fi
fi
}
##search for a string gtk##
function f_searchgtk () {
	zenity --info --timeout=5 --text "About to search a word list for a string of text."
	zenity --info --timeout=5 --text "Please select the word list you would like to search through."
	inputlist=$(zenity --file-selection --title "Word list to analyze")
	if [ -e "$inputlist" ]; then
		zenity --info --timeout=5 --text "$inputlist exists\nContinuing..."
	else
		zenity --info --timeout=5 --text "$inputlist does not exist..."
		zenity --info --timeout=5 --text "Starting over..."
		f_search
	fi
	string=$(zenity --entry --title "String to find" --text "Please enter the string of text to search for.")
	zenity --info --timeout=3 --text "Working..."
	foundstrings=$(cat $inputlist | grep -n "$string")
	foundstring=$(cat $inputlist | grep "$string")
	foundnum=$(cat $inputlist | sed -n '/'"$string"'/=')
	if [ "$foundstring" = "" ]; then
		zenity --info --text "The string: $string was not found in $inputlist"
		zenity --info --timeout=4 --text "Returning to the main menu..."
		f_menu
	elif [ "$foundstring" = "$string" ]; then
		zenity --info --text "The string: $string was found in\n $inputlist at line: $foundnum"
		zenity --info --timeout=4 --text "Returning to the main menu..."
		f_menu
	else
		zenity --info --text "The string: $string was found in $inputlist \nas a substring of:\n\nLine : String\n$foundstrings"
		zenity --info --timeout=4 --text "Returning to the main menu..."
		f_menu
	fi
}
