VER=1

CONFDIR = /usr/share/aaa

BINPROGS = \
	aaa-build \
	aaa-build-cinnamon-pbuilder-git \
	aaa-build-cinnamon-pbuilder-release \
	aaa-build-compiz-emerald \
	aaa-common-functions \
	aaa-setup-build-env \
	aaa-setup-tmpfs
	
PATCHES = \
	remove_elevated_privelages_banner.patch \
	142DPI.patch \
	convert_to_debian.patch
	
HOOKS = \
	D05deps
	

install:
	
	#install  dirs
	install -dm755 $(DESTDIR)$(CONFDIR)
	install -dm755 $(DESTDIR)$(CONFDIR)/patches
	install -dm755 $(DESTDIR)$(CONFDIR)/pbuilder
	install -dm755 $(DESTDIR)$(CONFDIR)/pbuilder/hooks
	#install conf files
	install -m755 aaa-master-config $(DESTDIR)$(CONFDIR)/aaa-master-config
	
	#install patches
	install -m744 $(PATCHES) $(DESTDIR)$(CONFDIR)/patches
	
	#install hooks
	install -m744 $(HOOKS) $(DESTDIR)$(CONFDIR)/pbuilder/hooks
	
	#install scripts
	install -m755 $(BINPROGS) $(DESTDIR)/usr/bin

uninstall:
	for f in $(BINPROGS); do $(RM) $(DESTDIR)/usr/bin/$$f; done
	rm -rf $(DESTDIR)$(CONFDIR)
	

.PHONY: install uninstall
