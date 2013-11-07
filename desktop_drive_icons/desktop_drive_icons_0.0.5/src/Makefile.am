## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(DESKTOP_DRIVE_ICONS_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = desktop_drive_icons

desktop_drive_icons_SOURCES = \
	common.h								\
	settings.c			settings.h			\
	desklet.c			desklet.h			\
	operation.c			operation.h			\
	icon-label.c		icon-label.h		\
	icon.c				icon.h				\
	icon-view.c			icon-view.h			\
	main.c

desktop_drive_icons_LDADD = $(DESKTOP_DRIVE_ICONS_LIBS)
