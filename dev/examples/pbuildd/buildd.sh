#!/bin/bash
# all-rebuilder script.

set -e

. /etc/pbuilder/buildd-config.sh

NOBUILDDEP=${BASEDIRECTORY}/FAILED/NOBUILDDEP
FAILED=${BASEDIRECTORY}/FAILED
DEPWAIT=${BASEDIRECTORY}/DEPWAIT
SUCCESS=${BASEDIRECTORY}/SUCCESS
mkdir "$FAILED" || true 
mkdir "$DEPWAIT" || true 
mkdir "$SUCCESS" || true 
mkdir ${BASEDIRECTORY}/WORKING || true 
mkdir ${BASEDIRECTORY}/STATUS || true 
mkdir "$NOBUILDDEP" || true
BUILDRESULTDIR=${PWD}/RESULT-deb
mkdir "$BUILDRESULTDIR" || true

STATUSFILE=${BASEDIRECTORY}/STATUS/$(hostname)-$$
BUILDTMP=${BASEDIRECTORY}/tmp-b-$(hostname)-$$

function status () {
    echo "$@" > $STATUSFILE
    echo "$@"
}

function buildone() {
    local PROGNAME="$1"
    local LOGFILE=${BASEDIRECTORY}/WORKING/"$PROGNAME.log"

    status "building $PROGNAME"
    mkdir $BUILDTMP || true
    (
	cd $BUILDTMP
	apt-get source -d $PROGNAME
	if sudo pbuilder build --hookdir "${HOOKDIR}" --buildresult . --logfile "$LOGFILE" *.dsc; then
	    mv "$LOGFILE" "$SUCCESS"
	    echo Build successful
	else
	    if grep "^E: pbuilder: Could not satisfy build-dependency." $LOGFILE > /dev/null; then
		mv "$LOGFILE" "$DEPWAIT"
		echo Dependency cannot be satisfied.
	    elif [ $(awk '/ -> Attempting to parse the build-deps/,/^ -> Finished parsing the build-deps/{print $0}' $LOGFILE | wc -l ) = "2" ]; then
		echo "Missing build-deps"
		mv "$LOGFILE" "$NOBUILDDEP"
	    elif grep '^E: Could not satisfy build-dependency' "$LOGFILE" > /dev/null ; then
		echo "Build-dep wait" 
		mv "$LOGFILE" "$DEPWAIT"
	    elif grep '^E: pbuilder-satisfydepends failed.' "$LOGFILE" > /dev/null ; then
		echo "Build-dep satisfaction failed on other package's installation"
		mv "$LOGFILE" "$DEPWAIT"
	    else
		mv "$LOGFILE" "$FAILED"
		echo Build failed
	    fi
	fi
    )
    status "finished building $PROGNAME"
    rm -rf $BUILDTMP;
}


$ROOTCOMMAND dselect update
$ROOTCOMMAND pbuilder update 

tmpfile=$(tempfile)
wget "${MIRROR}"/debian/dists/unstable/main/source/Sources.gz -O${tmpfile}

for A in $( zcat ${tmpfile} | awk "BEGIN { RS = \"\" }
  {if (match(\$0, /\nArchitecture: [^\n]*(all|any|`dpkg --print-architecture`)/)) print \$0}" | sed -n 's/^Package: //p' | cut -d\  -f1|sort | uniq | bogosort -n ); do

    # this part needs to be atomic
    status "considering $A"
    if grep "^$A$" $AVOIDLIST; then
	echo Skip.
	continue
    fi
    if echo "$A" | grep "^kernel-image"; then
    	echo I hate kernel images.
	continue
    fi

    if [ $(find ${BASEDIRECTORY} -name $A.log | wc -l ) = "1" ]; then
	echo Already build tried for "$A"
	continue
    fi
    # end of atomic.

    waitingroutine
    buildone $A
done

rm -f ${tmpfile}

