alias build='dpkg-buildpackage -b -us -uc -j$((    $(cat /proc/cpuinfo | grep processor | wc -l)+1    ))'
