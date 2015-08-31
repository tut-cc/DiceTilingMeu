buildtype := debug

.PHONY: all clean

all :
	cd Problem  ; buildtype=$(buildtype) $(MAKE)
	cd Field    ; buildtype=$(buildtype) $(MAKE)
	cd Stone    ; buildtype=$(buildtype) $(MAKE)
	cd Algorithm; buildtype=$(buildtype) $(MAKE)
	cd DTM_Main ; buildtype=$(buildtype) $(MAKE)

clean :
	cd Problem  ; buildtype=$(buildtype) $(MAKE) clean
	cd Field    ; buildtype=$(buildtype) $(MAKE) clean
	cd Stone    ; buildtype=$(buildtype) $(MAKE) clean
	cd Algorithm; buildtype=$(buildtype) $(MAKE) clean
	cd DTM_Main ; buildtype=$(buildtype) $(MAKE) clean

# vim:set tabstop=4 noexpandtab

