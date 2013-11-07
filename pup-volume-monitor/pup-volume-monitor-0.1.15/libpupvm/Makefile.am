lib_LTLIBRARIES = libpupvm.la
libpupvm_la_CFLAGS = $(GIO_CFLAGS)  $(UDEV_CFLAGS) $(BLKID_CFLAGS) \
		-I$(top_srcdir) -Wall \
		-DPUP_VM_SCRIPTS_DIR=\"$(libexecdir)/pup-volume-monitor\"
libpupvm_la_LIBADD = $(LIBS) $(GIO_LIBS) $(UDEV_LIBS)
libpupvm_la_SOURCES = pupvm-plugin.h common-includes.h\
		utils.c utils.h \
		core.c core.h \
		socket.c socket.h \
		transfer.c transfer.h \
		protocol.c protocol.h \
		conv.c conv.h \
		pupvm-utils.c pupvm-utils.h \
		pupvm-settings.c pupvm-settings.h \
		pupvm-classes.c pupvm-classes.h \
		pupvm-monitor.c pupvm-monitor.h

pupvmincludedir = $(includedir)/pupvm
pupvminclude_HEADERS = pupvm-plugin.h \
		utils.h \
		core.h \
		socket.h \
		transfer.h \
		protocol.h \
		conv.h \
		pupvm-utils.h \
		pupvm-settings.h \
		pupvm-classes.h \
		pupvm-monitor.h 

