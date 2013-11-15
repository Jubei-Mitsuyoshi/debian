#!/bin/sh
##wordify scratch""
function f_wordifyscratch () {
	format=""
	f_wordifycheck
}
##wordify GTK"
function f_wordifycheckGTK () {
	zenity --info --timeout=4 --text "Wordify GTK is a work in progress.\n(Some features are in CLI format.)"
	zenity --info --timeout=4 --text "Forcing GTK to 'n' to complete the wordification..."
	GTK="n"
	zenity --info --timeout=4 --text "GTK is now set to: $GTK"
	zenity --info --timeout=4 --text "Continuing with the wordification of $filename.lst"
	f_wordifycheck
	zenity --info --timeout=4 --text "Forcing GTK back to 'y'."
	GTK="y"
	zenity --info --timeout=4 --text "GTK is now set to: $GTK"
	zenity --info --timeout=4 --text "Returning to the main menu..."
}
##wordify ddmmyy##
function f_wordifyddmmyy () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+3
				let prefixnum2=$prefixnum
				sed 's/^\(.\{'$prefixnum1'\}\)01/\1January/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\1December/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\11st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/"$filename"_wordified.lst"
				echo
}
##wordify ddmmyyyy#
function f_wordifyddmmyyyy () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+3
				let prefixnum2=$prefixnum
				sed 's/^\(.\{'$prefixnum1'\}\)01/\1January/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\1December/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\11st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/"$filename"_wordified.lst"
				echo
}
##wordify mmddyy##
function f_wordifymmddyy () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+3
				let prefixnum2=$prefixnum
				sed 's/^\(.\{'$prefixnum1'\}\)01/\11st/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/temp2.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\1January/' "$destination/temp2.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\1December/' "$destination/temp.temp" > ""$destination"/"$filename"_wordified.lst"
				echo
}
##wordify mmddyyyy##
function f_wordifymmddyyyy () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+3
				let prefixnum2=$prefixnum
				sed 's/^\(.\{'$prefixnum1'\}\)01/\11st/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/temp2.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\1January/' "$destination/temp2.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\1December/' "$destination/temp.temp" > ""$destination"/"$filename"_wordified.lst"
				echo
}
##wordify yymmdd##
function f_wordifyyymmdd () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+6
				let prefixnum2=$prefixnum+3
				sed 's/^\(.\{'$prefixnum1'\}\)01/\11st/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/temp2.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\1January/' "$destination/temp2.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\1December/' "$destination/temp.temp" > ""$destination"/"$filename"_wordified.lst"
				echo
}
##wordify yyyymmdd##
function f_wordifyyyyymmdd () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+8
				let prefixnum2=$prefixnum+5
				sed 's/^\(.\{'$prefixnum1'\}\)01/\11st/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/temp2.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\1January/' "$destination/temp2.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\1December/' "$destination/temp.temp" > ""$destination"/"$filename"_wordified.lst"
				echo
}
##wordify ddmmyy no sep##
function f_wordifyddmmyy_nosep () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+2
				let prefixnum2=$prefixnum
				sed 's/^\(.\{'$prefixnum1'\}\)01/\1January/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\1December/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\11st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/"$filename"_wordified.lst"
				echo
}
##wordify ddmmyyyy no sep##
function f_wordifyddmmyyyy_nosep () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+2
				let prefixnum2=$prefixnum
				sed 's/^\(.\{'$prefixnum1'\}\)01/\1January/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\1December/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\11st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/"$filename"_wordified.lst"
				echo
}
##wordify mmddyy no sep##
function f_wordifymmddyy_nosep () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+2
				let prefixnum2=$prefixnum
				sed 's/^\(.\{'$prefixnum1'\}\)01/\11st/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/temp2.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\1January/' "$destination/temp2.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\1December/' "$destination/temp.temp" > ""$destination"/"$filename"_wordified.lst"
				echo
}
##wordify mmddyyyy no sep##
function f_wordifymmddyyyy_nosep () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+2
				let prefixnum2=$prefixnum
				sed 's/^\(.\{'$prefixnum1'\}\)01/\11st/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/temp2.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\1January/' "$destination/temp2.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\1December/' "$destination/temp.temp" > ""$destination"/"$filename"_wordified.lst"
				echo
}
##wordify yymmdd no sep##
function f_wordifyyymmdd_nosep () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+4
				let prefixnum2=$prefixnum+2
				sed 's/^\(.\{'$prefixnum1'\}\)01/\11st/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/temp2.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\1January/' "$destination/temp2.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\1December/' "$destination/temp.temp" > ""$destination"/"$filename"_wordified.lst"
				echo
}
##wordify yyyymmdd no sep##
function f_wordifyyyyymmdd_nosep () {
				echo
				prefixnumber=$(echo $prefix | wc -c)
				let prefixnum=$prefixnumber-1
				let prefixnum1=$prefixnum+6
				let prefixnum2=$prefixnum+4
				sed 's/^\(.\{'$prefixnum1'\}\)01/\11st/' "$destination/$filename.lst" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)02/\12nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)03/\13rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)04/\14th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)05/\15th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)06/\16th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)07/\17th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)08/\18th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)09/\19th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)10/\110th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)11/\111th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)12/\112th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)13/\113th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)14/\114th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)15/\115th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)16/\116th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)17/\117th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)18/\118th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)19/\119th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)20/\120th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)21/\121st/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)22/\122nd/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)23/\123rd/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)24/\124th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)25/\125th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)26/\126th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)27/\127th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)28/\128th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)29/\129th/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)30/\130th/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum1'\}\)31/\131st/' "$destination/temp1.temp" > "$destination/temp2.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)01/\1January/' "$destination/temp2.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)02/\1February/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)03/\1March/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)04/\1April/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)05/\1May/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)06/\1June/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)07/\1July/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)08/\1August/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)09/\1September/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)10/\1October/' "$destination/temp.temp" > "$destination/temp1.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)11/\1November/' "$destination/temp1.temp" > "$destination/temp.temp"
				sed 's/^\(.\{'$prefixnum2'\}\)12/\1December/' "$destination/temp.temp" > ""$destination"/"$filename"_wordified.lst"
				echo
}
##wordify an existing wordlist##
function f_wordifyexisting () {
	f_echobreak
	echo "About to wordify and existing word list created with Tape's Datelist"
	sleep 2
	f_echobreak
	echo "Enter the full file name of the word list you would like to Wordify."
	echo "(The file extension .lst will be appended to your selection so...)"
	echo "(...make sure the file you select is already in .lst format!!!)"
	echo "(e.x. Datelist)"
	echo
	read filename	
	echo
	echo "Enter the full path to the wordlist you would like to Wordify."
	echo "(e.x. /root/Desktop)"
	echo
	read destination
	f_echobreak
	echo "1) ddmmyy"
	echo "2) ddmmyyyy"
	echo "3) mmddyy"
	echo "4) mmddyyyy"
	echo "5) yymmdd"
	echo "6) yyyymmdd" 
	echo
	echo "Enter date format of the word list you would like to wordify."
	echo
	read dateformat
	if [ "$dateformat" = "1" ]; then
		format="ddmmyy"
	elif [ "$dateformat" = "2" ]; then
		format="ddmmyyyy"
	elif [ "$dateformat" = "3" ]; then
		format="mmddyy"
	elif [ "$dateformat" = "4" ]; then
		format="mmddyyyy"
	elif [ "$dateformat" = "5" ]; then
		format="yymmdd"
	elif [ "$dateformat" = "6" ]; then
		format="yyyymmdd"
	else
		echo "You did not enter a correct answer."
		echo 
		echo "Starting Over..."
		sleep 3
		f_wordifyexisting
	fi
	echo
	echo "Does $filename have a seperators? (y/n)"
	echo "(e.x. yyyy-mm-dd would be y)"
	echo
	read seperators
	echo
	if [ "$seperators" = "y" ]; then
		echo
		if [ "$format" == "ddmmyy" ]; then
				f_wordifyddmmyy
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifyexisting
				fi
			elif [ "$format" == "ddmmyyyy" ]; then
				f_wordifyddmmyyyy
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifyexisting
				fi
			elif [ "$format" == "mmddyy" ]; then
				f_wordifymmddyy
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifyexisting
				fi
			elif [ "$format" == "mmddyyyy" ]; then
				f_wordifymmddyyyy
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifyexisting
				fi
			elif [ "$format" == "yymmdd" ]; then
				f_wordifyyymmdd
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo "'$filename'_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifyexisting
				fi
			elif [ "$format" == "yyyymmdd" ]; then
				f_wordifyyyyymmdd
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifyexisting
				fi
			fi
	elif [ "$seperators" = "n" ]; then
		echo
		if [ "$format" == "ddmmyy" ]; then
				f_wordifyddmmyy_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "ddmmyyyy" ]; then
				f_wordifyddmmyyyy_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "mmddyy" ]; then\
				f_wordifymmddyy_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "mmddyyyy" ]; then
				f_wordifymmddyyyy_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "yymmdd" ]; then
				f_wordifyyymmdd_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "yyyymmdd" ]; then
				f_wordifyyyyymmdd_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo "'$filename'_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			fi
	else
		echo "You did not enter a correct answer."
		echo 
		echo "Starting Over..."
		sleep 3
		f_wordifyexisting
	fi
}
##wordify a datelist##
function f_wordifycheck () {
f_echobreak
if [ "$GTK" = "y" ]; then
	f_wordifycheckGTK
else
	if  [[ "$format" == "ddmmyy" || "$format" == "ddmmyyyy" || "$format" == "mmddyy" || "$format" == "mmddyyyy" || "$format" == "yymmdd" || "$format" == "yyyymmdd" ]] ; then
		echo
		echo "You chose to wordify the word list you just created:"
		echo "$destination/$filename.lst"
		echo
		echo "The word list you created was in: $format format."
		#echo
		#echo "How many total characters did you prefix the date list with?"
		#echo "(if you did not use TAPE's Datelist prefix function enter 0)"
		#echo
		#read prefixnum
		echo
		if [ "$sep" != "" ]; then
			echo "The word list you created has '$sep' as a seperator."
			echo
			echo "Adjusting the wordify process accordingly for your datelist format."
			echo
			sleep 3
			if [ "$format" == "ddmmyy" ]; then
				f_wordifyddmmyy
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "ddmmyyyy" ]; then
				f_wordifyddmmyyyy
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "mmddyy" ]; then
				f_wordifymmddyy
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "mmddyyyy" ]; then
				f_wordifymmddyyyy
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo "'$filename'_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "yymmdd" ]; then
				f_wordifyyymmdd
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo "'$filename'_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "yyyymmdd" ]; then
				f_wordifyyyyymmdd
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo "'$filename'_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			fi
		else
			echo "Your word list did not use a seperator."
			echo
			echo "Adjusting the wordify process accordingly for your datelist format."
			echo
			sleep 3
			if [ "$format" == "ddmmyy" ]; then
				f_wordifyddmmyy_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "ddmmyyyy" ]; then
				f_wordifyddmmyyyy_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "mmddyy" ]; then\
				f_wordifymmddyy_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "mmddyyyy" ]; then
				f_wordifymmddyyyy_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo ""$filename"_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "yymmdd" ]; then
				f_wordifyyymmdd_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo "'$filename'_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			elif [ "$format" == "yyyymmdd" ]; then
				f_wordifyyyyymmdd_nosep
				echo "Almost done creating the modified word list: "$filename"_wordified.lst"
				echo
				echo "Removing the temp files..."
				echo
				sleep 1
				rm $destination/*.temp
				if [ -e $destination/"$filename"_wordified.lst ]; then
					echo
					echo "'$filename'_wordified.lst exists in the directory: $destination"
					echo
					sleep 1
					echo "The word list "$filename"_wordified.lst has been successfully created in: $destination"
					echo
					sleep 2
				else
					echo
					echo ""$filename"_wordified.lst does not exist in the directory: $destination"
					echo
					sleep 2
					echo "Let's try this again..."
					sleep 2
					f_wordifycheck
				fi
			fi
		fi
	else
		echo "About to generate a wordified word list from scratch."
		echo
		sleep 3
		f_echobreak
		echo "1) ddmmyy"
		echo "2) ddmmyyyy"
		echo "3) mmddyy"
		echo "4) mmddyyyy"
		echo "5) yymmdd"
		echo "6) yyyymmdd" 
		echo
		echo "Enter date format you would like to use?"
		echo
		read dateformat
		if [ "$dateformat" = "1" ]; then
			format="ddmmyy"
		elif [ "$dateformat" = "2" ]; then
			format="ddmmyyyy"
		elif [ "$dateformat" = "3" ]; then
			format="mmddyy"
		elif [ "$dateformat" = "4" ]; then
			format="mmddyyyy"
		elif [ "$dateformat" = "5" ]; then
			format="yymmdd"
		elif [ "$dateformat" = "6" ]; then
			format="yyyymmdd"
		else
			echo "You did not enter a correct answer."
			echo 
			echo "Starting Over..."
			sleep 3
			f_wordifycheck
		fi
		f_echobreak
		echo "1) ' ' (A Space)         e.x. yyyy mm dd"
		echo "2) '-' (A Hyphen)        e.x. yyyy-mm-dd"
		echo "3) '.' (A Period)        e.x. yyyy.mm.dd"
		echo "4) '/' (A Back Slash)    e.x. yyyy/mm/dd"
		echo "5) '\' (A Forward Slash) e.x. yyyy\mm\dd"
		echo "6) ''  (No Space)        e.x. yyyymmdd"
		echo
		echo "What would you like to use as a seperator?"
		echo
		read seperator
		if [ "$seperator" = "1" ]; then
			sep=' '
		elif [ "$seperator" = "2" ]; then
			sep="-"
		elif [ "$seperator" = "3" ]; then
			sep="."
		elif [ "$seperator" = "4" ]; then
			sep="/"
		elif [ "$seperator" = "5" ]; then
			sep='\'
		elif [ "$seperator" = "6" ]; then
			sep=""
		else
			echo "You did not enter the correct answer."
			echo 
			echo "Starting Over..."
			sleep 3
			f_wordifycheck
		fi
		f_echobreak
		echo "What would you like to use as the starting date?"
		echo "    (The date must be in yyyy-mm-dd format)"
		echo
		read start
		echo
		echo
		echo "What would you like to use as the ending date?"
		echo "  (The date must be in yyyy-mm-dd format)"
		echo
		read end
		echo
		echo "What text would you like to prefix the wordified datelist with?"
		echo "  (Leave blank if you would not like to prefix text)"
		echo
		read prefix
		echo
		echo
		echo "What text would you like to append the wordified datelist with?"
		echo "  (Leave blank if you would not like to append text)"
		echo
		read append
		echo
		echo "         What would you like your output file to be named?"
		echo "The file extension _wordified.lst will be appended to the filename"
		echo
		read filename
		echo
		echo "Where would you like "$filename"_wordified.lst to be placed?"
		echo "e.x. /root/Desktop/"
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
					f_wordifycheck
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
		echo "About to create a word list with the dates from:" $start
		echo "     to $end, seperated by the character:'$sep'"
		echo "Would you like to create the word list: "$filename"_wordified.lst? (y/n)"
		echo
		read createfile
		if [ "$createfile" = "y" ]; then
			f_tape
			echo
			echo "The word list: $filename.lst has been created."
			echo
			echo "About to move $filename.lst to $destination."
			mv $filename.lst $destination
			if [ -e $destination/$filename.lst ]; then
				f_wordifycheck
				rm $destination/$filename.lst
			else
				echo
				echo "$filename.lst does not exist in the directory: $destination"
				echo
				sleep 2
				echo "Let's try this again..."
				sleep 2
				f_wordifycheck
			fi
		else
			echo "You selected not to create $filename.lst"
			echo
			echo "Returning to the main menu..."
			sleep 3
			f_menu
		fi
	fi
fi	
}
