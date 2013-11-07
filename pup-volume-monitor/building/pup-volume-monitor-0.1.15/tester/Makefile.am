noinst_PROGRAMS = client server gio-testing misc-testing

client_SOURCES = client.c
client_CFLAGS = $(GIO_CFLAGS) -I$(top_srcdir) -lm
client_LDADD = $(GIO_LIBS) $(LIBS) $(top_builddir)/libpupvm/libpupvm.la

server_SOURCES = server.c
server_CFLAGS = $(GIO_CFLAGS) -I$(top_srcdir) -lm
server_LDADD = $(GIO_LIBS) $(LIBS) $(top_builddir)/libpupvm/libpupvm.la

gio_testing_SOURCES = gio-testing.c
gio_testing_CFLAGS = $(GIO_CFLAGS)
gio_testing_LDADD = $(GIO_LIBS)

misc_testing_SOURCES = misc-testing.c
misc_testing_CFLAGS = $(GIO_CFLAGS) -I$(top_srcdir) -lm
misc_testing_LDADD = $(GIO_LIBS) $(LIBS) $(top_builddir)/libpupvm/libpupvm.la
