pupvmplugindir = $(libdir)/pup-volume-monitor

pupvmplugin_LTLIBRARIES = 

if USE_CDROM
pupvmplugin_LTLIBRARIES += libcdrom.la

libcdrom_la_CFLAGS = $(GIO_CFLAGS) $(BLKID_CFLAGS) $(UDEV_CFLAGS) \
		-Wall -I$(top_srcdir)
libcdrom_la_LIBADD = $(GIO_LIBS) $(BLKID_LIBS) $(UDEV_LIBS) \
		$(LIBS) $(top_builddir)/libpupvm/libpupvm.la
libcdrom_la_LDFLAGS = -avoid-version
		
libcdrom_la_SOURCES = cdrom.c cdrom.h
endif

