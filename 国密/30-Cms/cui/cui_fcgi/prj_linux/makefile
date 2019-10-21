
MAKE := make --no-print-directory

all :
	@$(MAKE) -j4 -f makefile_debug clean;
	@$(MAKE) -j4 -f makefile_debug;
	@$(MAKE) -j4 -f makefile_debug clean;
	@$(MAKE) -j4 -f makefile_release clean;
	@$(MAKE) -j4 -f makefile_release;
	@$(MAKE) -j4 -f makefile_release clean;
#create make object respectly
clean:
	@$(MAKE) -j4 -f makefile_debug clean;
	@$(MAKE) -j4 -f makefile_release clean;
