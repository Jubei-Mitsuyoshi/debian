## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

desktop_drive_iconsdocdir = ${prefix}/doc/desktop_drive_icons
desktop_drive_iconsdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(desktop_drive_iconsdoc_DATA) \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(desktop_drive_iconsdocdir)
