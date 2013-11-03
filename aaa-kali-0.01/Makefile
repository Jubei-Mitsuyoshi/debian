VER=1

CONFDIR = /usr/share/aaa

BINPROGS = \
	aaa-build-cinnamon-pbuilder \
	aaa-setup-pbuilder \
	aa-setup-pbuilder
	
PATCHES = \
	remove_elevated_privelages_banner-patch
	


install:
	# tmpfs dir
	install -dm755 $(DESTDIR)/aaa-tmpfs
	
	#) my config dir
	install -dm755 $(DESTDIR)$(CONFDIR)
	install -dm755 $(DESTDIR)$(CONFDIR)/patches
	install -dm755 $(DESTDIR)$(CONFDIR)/pbuilder
	install -dm755 $(DESTDIR)$(CONFDIR)/pbuilder/hooks
	#install conf files
#	install -m744 .pbuilderrc $(DESTDIR)$HOME/.pbuilderrc
	install -m744 .aaa-master-config $(DESTDIR)$(CONFDIR)/.aaa-master-config
	install -m744 D05deps $(DESTDIR)$(CONFDIR)/pbuilder/hooks/D05deps
	
	#install patches
	install -m744 $(PATCHES) $(DESTDIR)/usr/share/aaa/patches/$(PATCHES)
	
	
	#repo dir --- and reporpor confs
	install -dm755 $(DESTDIR)/aaa-repo
	install -dm755 $(DESTDIR)/aaa-repo/conf
	install -m744 distributions $(DESTDIR)/aaa-repo/conf/distributions
	install -m744 options $(DESTDIR)/aaa-repo/conf/options
	install -m744 override.unstable $(DESTDIR)/aaa-repo/conf/override.unstable
	install -m744 override.stable $(DESTDIR)/aaa-repo/conf/override.stable
	install -m744 override.testing $(DESTDIR)/aaa-repo/conf/override.testing
	
	#install scripts
	install -m755 $(BINPROGS) $(DESTDIR)/usr/bin

uninstall:
	for f in $(BINPROGS); do $(RM) $(DESTDIR)/usr/bin/$$f; done
	rm -rf $(DESTDIR)/aaa-repo
	rm -rf $(DESTDIR)/usr/share/aaa
	

.PHONY: install uninstall
