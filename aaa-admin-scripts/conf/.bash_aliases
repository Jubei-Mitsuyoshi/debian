alias build='dpkg-buildpackage -us -uc -j$((    $(cat /proc/cpuinfo | grep processor | wc -l)+1    ))'
alias dquilt="quilt --quiltrc=${HOME}/.quiltrc-dpkg"
complete -F _quilt_completion $_quilt_complete_opt dquilt
