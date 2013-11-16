#!/bin/sh
function f_update () {
	newvers=$(cat thad0ctorstools-code/LAUNCH_TOOLKIT.sh | grep "scriptversion=" | awk -F " " '{print$1}' | sed 's/version=//')
	direc=$0
	scriptname=$(stat --printf="%n" "$0" )
	folder="$( cd "$( dirname "$0" )" && pwd )"
	version=$(cat LAUNCH_TOOLKIT.sh | grep "scriptversion=" | awk -F " " '{print$1}' | sed 's/version=//')
	if [ "$newvers" \> "$version" ]; then
		echo
		echo "About to update..."
		echo
		sleep 3
		mkdir "$version"_backup
		mv 3rdparty "$version"_backup
		mv depend "$version"_backup
		mv docs "$version"_backup
		mv ".LAUNCH TOOLKIT WITH LOGGING.sh" "$version"_backup
		mv ".LAUNCH_TOOLKIT_WITH_LOGGING.sh" "$version"_backup
		mv "UPDATER.sh" "$version"_backup
		mv LAUNCH_TOOLKIT.sh "$version"_backup
		mv *.txt "$version"_backup
		echo "Done backing up the old version..."
		echo
		sleep 2
		echo "Moving the updated version to:"
		echo "$folder"
		echo
		sleep 2
		mv thad0ctorstools-code/* "$folder"
		if [ -e "$folder"/LAUNCH_TOOLKIT.sh ]; then
			echo "The LAUNCH_TOOLKIT.sh exists in: "
			echo "$folder"
			curversion=$(cat LAUNCH_TOOLKIT.sh | grep "version=" | awk -F " " '{print$1}' | sed 's/version=//' | sed 's/"//' | sed 's/new$(cat//')
			echo
			echo "Current version: $curversion"
			echo
			sleep 3
			if [ "$newvers" = "$curversion" ]; then
				echo "The script has been updated to v$curversion and has been moved to:"
				echo "$folder"
				echo
				sleep 8
			else
				echo "The updated version has not been moved to:"
				echo "$folder"
				echo
				sleep 2
				echo "I suggest you manually download the newest version"
				sleep 10
			fi
		else
			echo "The updated version has not been moved to:"
			echo "$folder"
			echo
			sleep 2
			echo "I suggest you manually download the newest version"
			sleep 10
		fi
		rm -r thad0ctorstools-code
		echo "Starting the script over..."
		echo
		sleep 3
		bash LAUNCH_TOOLKIT.sh
	else
		echo
		echo "Your version is up to date."
		sleep 4
		rm -r thad0ctorstools-code
		bash LAUNCH_TOOLIT.sh
	fi
}
f_update
