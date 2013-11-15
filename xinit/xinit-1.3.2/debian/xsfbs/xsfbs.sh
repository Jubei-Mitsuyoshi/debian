# This is the X Strike Force shell library for X Window System package
# maintainer scripts.  It serves to define shell functions commonly used by
# such packages, and performs some error checking necessary for proper operation
# of those functions.  By itself, it does not "do" much; the maintainer scripts
# invoke the functions defined here to accomplish package installation and
# removal tasks.

# If you are reading this within a Debian package maintainer script (e.g.,
# /var/lib/dpkg/info/PACKAGE.{config,preinst,postinst,prerm,postrm}), you can
# skip past this library by scanning forward in this file to the string
# "GOBSTOPPER".

SOURCE_VERSION=@SOURCE_VERSION@
OFFICIAL_BUILD=@OFFICIAL_BUILD@

# Use special abnormal exit codes so that problems with this library are more
# easily tracked down.
SHELL_LIB_INTERNAL_ERROR=86
SHELL_LIB_THROWN_ERROR=74
SHELL_LIB_USAGE_ERROR=99

# old -> new variable names
if [ -z "$DEBUG_XORG_PACKAGE" ] && [ -n "$DEBUG_XFREE86_PACKAGE" ]; then
  DEBUG_XORG_PACKAGE="$DEBUG_XFREE86_PACKAGE"
fi
if [ -z "$DEBUG_XORG_DEBCONF" ] && [ -n "$DEBUG_XFREE86_DEBCONF" ]; then
  DEBUG_XORG_DEBCONF="$DEBUG_XFREE86_DEBCONF"
fi

# initial sanity checks
if [ -z "$THIS_PACKAGE" ]; then
  cat >&2 <<EOF
Error: package maintainer script attempted to use shell library without
definining \$THIS_PACKAGE shell variable.  Please report the package name,
version, and the text of this error message to the Debian Bug Tracking System.
Visit <http://www.debian.org/Bugs/Reporting> on the World Wide Web for
instructions, read the file /usr/share/doc/debian/bug-reporting.txt from the
"doc-debian" package, or install the "reportbug" package and use the command of
the same name to file a report against version $SOURCE_VERSION of this package.
EOF
  exit $SHELL_LIB_USAGE_ERROR
fi

if [ -z "$THIS_SCRIPT" ]; then
  cat >&2 <<EOF
Error: package maintainer script attempted to use shell library without
definining \$THIS_SCRIPT shell variable.  Please report the package name,
version, and the text of this error message to the Debian Bug Tracking System.
Visit <http://www.debian.org/Bugs/Reporting> on the World Wide Web for
instructions, read the file /usr/share/doc/debian/bug-reporting.txt from the
"doc-debian" package, or install the "reportbug" package and use the command of
the same name to file a report against version $SOURCE_VERSION of the
"$THIS_PACKAGE" package.
EOF
  exit $SHELL_LIB_USAGE_ERROR
fi

if [ "$1" = "reconfigure" ] || [ -n "$DEBCONF_RECONFIGURE" ]; then
  RECONFIGURE="true"
else
  RECONFIGURE=
fi

if ([ "$1" = "install" ] || [ "$1" = "configure" ]) && [ -z "$2" ]; then
  FIRSTINST="yes"
fi

if [ -z "$RECONFIGURE" ] && [ -z "$FIRSTINST" ]; then
  UPGRADE="yes"
fi

trap "message;\
      message \"Received signal.  Aborting $THIS_PACKAGE package $THIS_SCRIPT script.\";\
      message;\
      exit 1" HUP INT QUIT TERM

reject_nondigits () {
  # syntax: reject_nondigits [ operand ... ]
  #
  # scan operands (typically shell variables whose values cannot be trusted) for
  # characters other than decimal digits and barf if any are found
  while [ -n "$1" ]; do
    # does the operand contain anything but digits?
    if ! expr "$1" : "[[:digit:]]\+$" > /dev/null 2>&1; then
      # can't use die(), because it wraps message() which wraps this function
      echo "$THIS_PACKAGE $THIS_SCRIPT error: reject_nondigits() encountered" \
           "possibly malicious garbage \"$1\"" >&2
      exit $SHELL_LIB_THROWN_ERROR
    fi
    shift
  done
}

reject_unlikely_path_chars () {
  # syntax: reject_unlikely_path_chars [ operand ... ]
  #
  # scan operands (typically shell variables whose values cannot be trusted) for
  # characters unlikely to be seen in a path and which the shell might
  # interpret and barf if any are found
  while [ -n "$1" ]; do
    # does the operand contain any funny characters?
    if expr "$1" : '.*[!$&()*;<>?|].*' > /dev/null 2>&1; then
      # can't use die(), because I want to avoid forward references
      echo "$THIS_PACKAGE $THIS_SCRIPT error: reject_unlikely_path_chars()" \
           "encountered possibly malicious garbage \"$1\"" >&2
      exit $SHELL_LIB_THROWN_ERROR
    fi
    shift
  done
}

# Query the terminal to establish a default number of columns to use for
# displaying messages to the user.  This is used only as a fallback in the
# event the COLUMNS variable is not set.  ($COLUMNS can react to SIGWINCH while
# the script is running, and this cannot, only being calculated once.)
DEFCOLUMNS=$(stty size 2> /dev/null | awk '{print $2}') || true
if ! expr "$DEFCOLUMNS" : "[[:digit:]]\+$" > /dev/null 2>&1; then
  DEFCOLUMNS=80
fi

message () {
  # pretty-print messages of arbitrary length
  reject_nondigits "$COLUMNS"
  echo "$*" | fmt -t -w ${COLUMNS:-$DEFCOLUMNS} >&2
}

observe () {
  # syntax: observe message ...
  #
  # issue observational message suitable for logging someday when support for
  # it exists in dpkg
  if [ -n "$DEBUG_XORG_PACKAGE" ]; then
    message "$THIS_PACKAGE $THIS_SCRIPT note: $*"
  fi
}

warn () {
  # syntax: warn message ...
  #
  # issue warning message suitable for logging someday when support for
  # it exists in dpkg; also send to standard error
  message "$THIS_PACKAGE $THIS_SCRIPT warning: $*"
}

die () {
  # syntax: die message ...
  #
  # exit script with error message
  message "$THIS_PACKAGE $THIS_SCRIPT error: $*"
  exit $SHELL_LIB_THROWN_ERROR
}

internal_error () {
  # exit script with error; essentially a "THIS SHOULD NEVER HAPPEN" message
  message "internal error: $*"
  if [ -n "$OFFICIAL_BUILD" ]; then
    message "Please report a bug in the $THIS_SCRIPT script of the" \
            "$THIS_PACKAGE package, version $SOURCE_VERSION to the Debian Bug" \
            "Tracking System.  Include all messages above that mention the" \
            "$THIS_PACKAGE package.  Visit " \
            "<http://www.debian.org/Bugs/Reporting> on the World Wide Web for" \
            "instructions, read the file" \
            "/usr/share/doc/debian/bug-reporting.txt from the doc-debian" \
            "package, or install the reportbug package and use the command of" \
            "the same name to file a report."
  fi
  exit $SHELL_LIB_INTERNAL_ERROR
}

usage_error () {
  message "usage error: $*"
  message "Please report a bug in the $THIS_SCRIPT script of the" \
          "$THIS_PACKAGE package, version $SOURCE_VERSION to the Debian Bug" \
          "Tracking System.  Include all messages above that mention the" \
          "$THIS_PACKAGE package.  Visit " \
          "<http://www.debian.org/Bugs/Reporting> on the World Wide Web for" \
          "instructions, read the file" \
          "/usr/share/doc/debian/bug-reporting.txt from the doc-debian" \
          "package, or install the reportbug package and use the command of" \
          "the same name to file a report."
  exit $SHELL_LIB_USAGE_ERROR
}

font_update () {
  # run $UPDATECMDS in $FONTDIRS

  local dir cmd shortcmd x_font_dir_prefix

  x_font_dir_prefix="/usr/share/fonts/X11"

  if [ -z "$UPDATECMDS" ]; then
    usage_error "font_update() called but \$UPDATECMDS not set"
  fi
  if [ -z "$FONTDIRS" ]; then
    usage_error "font_update() called but \$FONTDIRS not set"
  fi

  reject_unlikely_path_chars "$UPDATECMDS"
  reject_unlikely_path_chars "$FONTDIRS"

  for dir in $FONTDIRS; do
    if [ -d "$x_font_dir_prefix/$dir" ]; then
      for cmd in $UPDATECMDS; do
        if which "$cmd" > /dev/null 2>&1; then
          shortcmd=${cmd##*/}
          observe "running $shortcmd in $dir font directory"
	  cmd_opts=
          if [ "$shortcmd" = "update-fonts-alias" ]; then
            cmd_opts=--x11r7-layout
          fi
          if [ "$shortcmd" = "update-fonts-dir" ]; then
            cmd_opts=--x11r7-layout
          fi
          if [ "$shortcmd" = "update-fonts-scale" ]; then
            cmd_opts=--x11r7-layout
          fi
          $cmd $cmd_opts $dir || warn "$cmd $cmd_opts $dir" \
                              "failed; font directory data may not" \
                              "be up to date"
        else
          warn "$cmd not found; not updating corresponding $dir font" \
               "directory data"
        fi
      done
    else
      warn "$dir is not a directory; not updating font directory data"
    fi
  done
}

remove_conffile_prepare () {
  # syntax: remove_conffile_prepare filename official_md5sum ...
  #
  # Check a conffile "filename" against a list of canonical MD5 checksums.
  # If the file's current MD5 checksum matches one of the "official_md5sum"
  # operands provided, then prepare the conffile for removal from the system.
  # We defer actual deletion until the package is configured so that we can
  # roll this operation back if package installation fails.
  #
  # Call this function from a preinst script in the event $1 is "upgrade" or
  # "install" and verify $2 to ensure the package is being upgraded from a
  # version (or installed over a version removed-but-not-purged) prior to the
  # one in which the conffile was obsoleted.

  local conffile current_checksum

  # validate arguments
  if [ $# -lt 2 ]; then
    usage_error "remove_conffile_prepare() called with wrong number of" \
                "arguments; expected at least 2, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  conffile="$1"
  shift

  # does the conffile even exist?
  if [ -e "$conffile" ]; then
    # calculate its checksum
    current_checksum=$(md5sum < "$conffile" | sed 's/[[:space:]].*//')
    # compare it to each supplied checksum
    while [ -n "$1" ]; do
      if [ "$current_checksum" = "$1" ]; then
        # we found a match; move the confffile and stop looking
        observe "preparing obsolete conffile $conffile for removal"
        mv "$conffile" "$conffile.$THIS_PACKAGE-tmp"
        break
      fi
      shift
    done
  fi
}

remove_conffile_lookup () {
  # syntax: remove_conffile_lookup package filename
  #
  # Lookup the md5sum of a conffile in dpkg's database, and prepare for removal
  # if it matches the actual file's md5sum.
  #
  # Call this function when you would call remove_conffile_prepare but only
  # want to check against dpkg's status database instead of known checksums.

  local package conffile old_md5sum

  # validate arguments
  if [ $# -ne 2 ]; then
    usage_error "remove_conffile_lookup() called with wrong number of" \
                "arguments; expected 1, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  package="$1"
  conffile="$2"

  if ! [ -e "$conffile" ]; then
    return
  fi
  old_md5sum="$(dpkg-query -W -f='${Conffiles}' "$package" | \
    awk '{ if (match($0, "^ '"$conffile"' ")) print $2}')"
  if [ -n "$old_md5sum" ]; then
    remove_conffile_prepare "$conffile" "$old_md5sum"
  fi
}

remove_conffile_commit () {
  # syntax: remove_conffile_commit filename
  #
  # Complete the removal of a conffile "filename" that has become obsolete.
  #
  # Call this function from a postinst script after having used
  # remove_conffile_prepare() in the preinst.

  local conffile

  # validate arguments
  if [ $# -ne 1 ]; then
    usage_error "remove_conffile_commit() called with wrong number of" \
                "arguments; expected 1, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  conffile="$1"

  # if the temporary file created by remove_conffile_prepare() exists, remove it
  if [ -e "$conffile.$THIS_PACKAGE-tmp" ]; then
    observe "committing removal of obsolete conffile $conffile"
    rm "$conffile.$THIS_PACKAGE-tmp"
  fi
}

remove_conffile_rollback () {
  # syntax: remove_conffile_rollback filename
  #
  # Roll back the removal of a conffile "filename".
  #
  # Call this function from a postrm script in the event $1 is "abort-upgrade"
  # or "abort-install" is  after having used remove_conffile_prepare() in the
  # preinst.

  local conffile

  # validate arguments
  if [ $# -ne 1 ]; then
    usage_error "remove_conffile_rollback() called with wrong number of" \
                "arguments; expected 1, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  conffile="$1"

  # if the temporary file created by remove_conffile_prepare() exists, move it
  # back
  if [ -e "$conffile.$THIS_PACKAGE-tmp" ]; then
    observe "rolling back removal of obsolete conffile $conffile"
    mv "$conffile.$THIS_PACKAGE-tmp" "$conffile"
  fi
}

replace_conffile_with_symlink_prepare () {
  # syntax: replace_conffile_with_symlink_prepare oldfilename newfilename \
  # official_md5sum ...
  #
  # Check a conffile "oldfilename" against a list of canonical MD5 checksums.
  # If the file's current MD5 checksum matches one of the "official_md5sum"
  # operands provided, then prepare the conffile for removal from the system.
  # We defer actual deletion until the package is configured so that we can
  # roll this operation back if package installation fails. Otherwise copy it
  # to newfilename and let dpkg handle it through conffiles mechanism.
  #
  # Call this function from a preinst script in the event $1 is "upgrade" or
  # "install" and verify $2 to ensure the package is being upgraded from a
  # version (or installed over a version removed-but-not-purged) prior to the
  # one in which the conffile was obsoleted.

  local conffile current_checksum

  # validate arguments
  if [ $# -lt 3 ]; then
    usage_error "replace_conffile_with_symlink_prepare() called with wrong" \
                " number of arguments; expected at least 3, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  oldconffile="$1"
  shift
  newconffile="$1"
  shift

  remove_conffile_prepare "$_oldconffile" "$@"
  # If $oldconffile still exists, then md5sums didn't match.
  # Copy it to new one.
  if [ -f "$oldconffile" ]; then
    cp "$oldconffile" "$newconffile"
  fi

}

replace_conffile_with_symlink_commit () {
  # syntax: replace_conffile_with_symlink_commit oldfilename
  #
  # Complete the removal of a conffile "oldfilename" that has been
  # replaced by a symlink.
  #
  # Call this function from a postinst script after having used
  # replace_conffile_with_symlink_prepare() in the preinst.

  local conffile

  # validate arguments
  if [ $# -ne 1 ]; then
    usage_error "replace_conffile_with_symlink_commit() called with wrong" \
                "number of arguments; expected 1, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  conffile="$1"

  remove_conffile_commit "$conffile"
}

replace_conffile_with_symlink_rollback () {
  # syntax: replace_conffile_with_symlink_rollback oldfilename newfilename
  #
  # Roll back the replacing of a conffile "oldfilename" with symlink to
  # "newfilename".
  #
  # Call this function from a postrm script in the event $1 is "abort-upgrade"
  # or "abort-install" and verify $2 to ensure the package failed to upgrade
  # from a version (or install over a version removed-but-not-purged) prior
  # to the one in which the conffile was obsoleted.
  # You should have  used replace_conffile_with_symlink_prepare() in the
  # preinst.

  local conffile

  # validate arguments
  if [ $# -ne 2 ]; then
    usage_error "replace_conffile_with_symlink_rollback() called with wrong" \
                "number of arguments; expected 2, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  oldconffile="$1"
  newconffile="$2"

  remove_conffile_rollback "$_oldconffile"
  if [ -f "$newconffile" ]; then
    rm "$newconffile"
  fi
}

run () {
  # syntax: run command [ argument ... ]
  #
  # Run specified command with optional arguments and report its exit status.
  # Useful for commands whose exit status may be nonzero, but still acceptable,
  # or commands whose failure is not fatal to us.
  #
  # NOTE: Do *not* use this function with db_get or db_metaget commands; in
  # those cases the return value of the debconf command *must* be checked
  # before the string returned by debconf is used for anything.

  local retval

  # validate arguments
  if [ $# -lt 1 ]; then
    usage_error "run() called with wrong number of arguments; expected at" \
                "least 1, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  "$@" || retval=$?

  if [ ${retval:-0} -ne 0 ]; then
    observe "command \"$*\" exited with status $retval"
  fi
}

make_symlink_sane () {
  # syntax: make_symlink_sane symlink target
  #
  # Ensure that the symbolic link symlink exists, and points to target.
  #
  # If symlink does not exist, create it and point it at target.
  #
  # If symlink exists but is not a symbolic link, back it up.
  #
  # If symlink exists, is a symbolic link, but points to the wrong location, fix
  # it.
  #
  # If symlink exists, is a symbolic link, and already points to target, do
  # nothing.
  #
  # This function wouldn't be needed if ln had an -I, --idempotent option.

  # Validate arguments.
  if [ $# -ne 2 ]; then
    usage_error "make_symlink_sane() called with wrong number of arguments;" \
      "expected 2, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  # We could just use the positional parameters as-is, but that makes things
  # harder to follow.
  local symlink target

  symlink="$1"
  target="$2"

  if [ -L "$symlink" ] && [ "$(readlink "$symlink")" = "$target" ]; then
      observe "link from $symlink to $target already exists"
  else
    observe "creating symbolic link from $symlink to $target"
    mkdir -p "${target%/*}" "${symlink%/*}"
    ln -s -b -S ".dpkg-old" "$target" "$symlink"
  fi
}

migrate_dir_to_symlink () {
  # syntax: migrate_dir_to_symlink old_location new_location
  #
  # Per Debian Policy section 6.5.4, "A directory will never be replaced by a
  # symbolic link to a directory or vice versa; instead, the existing state
  # (symlink or not) will be left alone and dpkg will follow the symlink if
  # there is one."
  #
  # We have to do it ourselves.
  #
  # This function moves the contents of old_location, a directory, into
  # new_location, a directory, then makes old_location a symbolic link to
  # new_location.
  #
  # old_location need not exist, but if it does, it must be a directory (or a
  # symlink to a directory).  If it is not, it is backed up.  If new_location
  # exists already and is not a directory, it is backed up.
  #
  # This function should be called from a package's preinst so that other
  # packages unpacked after this one --- but before this package's postinst runs
  # --- are unpacked into new_location even if their payloads contain
  # old_location filespecs.

  # Validate arguments.
  if [ $# -ne 2 ]; then
    usage_error "migrate_dir_to_symlink() called with wrong number of"
                "arguments; expected 2, got $#"
    exit $SHELL_LIB_USAGE_ERROR
  fi

  # We could just use the positional parameters as-is, but that makes things
  # harder to follow.
  local new old

  old="$1"
  new="$2"

  # Is old location a symlink?
  if [ -L "$old" ]; then
    # Does it already point to new location?
    if [ "$(readlink "$old")" = "$new" ]; then
      # Nothing to do; migration has already been done.
      observe "migration of $old to $new already done"
      return 0
    else
      # Back it up.
      warn "backing up symbolic link $old as $old.dpkg-old"
      mv -b "$old" "$old.dpkg-old"
    fi
  fi

  # Does old location exist, but is not a directory?
  if [ -e "$old" ] && ! [ -d "$old" ]; then
      # Back it up.
      warn "backing up non-directory $old as $old.dpkg-old"
      mv -b "$old" "$old.dpkg-old"
  fi

  observe "migrating $old to $new"

  # Is new location a symlink?
  if [ -L "$new" ]; then
    # Does it point the wrong way, i.e., back to where we're migrating from?
    if [ "$(readlink "$new")" = "$old" ]; then
      # Get rid of it.
      observe "removing symbolic link $new which points to $old"
      rm "$new"
    else
      # Back it up.
      warn "backing up symbolic link $new as $new.dpkg-old"
      mv -b "$new" "$new.dpkg-old"
    fi
  fi

  # Does new location exist, but is not a directory?
  if [ -e "$new" ] && ! [ -d "$new" ]; then
    warn "backing up non-directory $new as $new.dpkg-old"
    mv -b "$new" "$new.dpkg-old"
  fi

  # Create new directory if it does not yet exist.
  if ! [ -e "$new" ]; then
    observe "creating $new"
    mkdir -p "$new"
  fi

  # Copy files in old location to new location.  Back up any filenames that
  # already exist in the new location with the extension ".dpkg-old".
  observe "copying files from $old to $new"
  if ! (cd "$old" && cp -a -b -S ".dpkg-old" . "$new"); then
    die "error(s) encountered while copying files from $old to $new"
  fi

  # Remove files at old location.
  observe "removing $old"
  rm -r "$old"

  # Create symlink from old location to new location.
  make_symlink_sane "$old" "$new"
}

# vim:set ai et sw=2 ts=2 tw=80:

# GOBSTOPPER: The X Strike Force shell library ends here.
