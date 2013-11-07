bin_PROGRAMS = pup-volume-monitor-admin

pup_volume_monitor_admin_CFLAGS = $(GIO_CFLAGS) $(UDEV_CFLAGS) \
		-Wall -I$(top_srcdir)
pup_volume_monitor_admin_LDADD = $(GIO_LIBS) $(UDEV_LIBS) $(LIBS) \
		$(top_builddir)/libpupvm/libpupvm.la
pup_volume_monitor_admin_SOURCES = common.h \
		main.c


