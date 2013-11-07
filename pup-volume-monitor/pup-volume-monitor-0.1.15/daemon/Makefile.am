
bin_PROGRAMS = pup-volume-monitor
pup_volume_monitor_CFLAGS =  $(GIO_CFLAGS)  $(UDEV_CFLAGS) $(BLKID_CFLAGS) \
		-I$(top_srcdir) -Wall
pup_volume_monitor_LDADD = $(GIO_LIBS) $(UDEV_LIBS) $(BLKID_LIBS) \
		$(top_builddir)/libpupvm/libpupvm.la
pup_volume_monitor_SOURCES = common.h \
		operations.c operations.h \
		monitor.c monitor.h \
		drive-probes.c drive-probes.h \
		server.c server.h \
		main.c

