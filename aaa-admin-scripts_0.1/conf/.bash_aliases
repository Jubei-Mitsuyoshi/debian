alias build='dpkg-buildpackage -us -uc -j$((    $(cat /proc/cpuinfo | grep processor | wc -l)+1    ))'
