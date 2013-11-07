giomodulesdir = $(libdir)/gio/modules
giomodules_LTLIBRARIES = libpup-volume-monitor.la
libpup_volume_monitor_la_CFLAGS = $(GIO_CFLAGS) -Wall -I$(top_srcdir)
libpup_volume_monitor_la_LIBADD = $(GIO_LIBS) \
		$(LIBS) $(top_builddir)/libpupvm/libpupvm.la
libpup_volume_monitor_la_LDFLAGS = -avoid-version
libpup_volume_monitor_la_SOURCES = common.h \
		utils.c utils.h \
		client-monitor.c client-monitor.h \
		volume-monitor.c volume-monitor.h \
		volume.c volume.h \
		mount.c mount.h \
		drive.c drive.h \
		main.c
		
