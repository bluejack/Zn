#==============================================================================#
#                                                                              #
#                  See full documentation in common.make                       #
#                                                                              #
#==============================================================================#

SHELL := /bin/bash

.PHONY: all export clean

all:    export
	@for DIRSET in  $$(find src/lib  -name Makefile)        \
			$$(find src/apps -name Makefile)        \
			$$(find test     -name Makefile);       \
	do                                                      \
		DIR=$${DIRSET%/*};                              \
		$(MAKE)  --directory=$$DIR;                     \
	done;

export:
	@for DIRSET in  $$(find src/lib  -name Makefile)        \
			$$(find src/apps -name Makefile)        \
			$$(find test     -name Makefile);       \
	do                                                      \
		DIR=$${DIRSET%/*};                              \
		$(MAKE) --directory=$$DIR export;               \
	done;

clean: 
	@echo
	@echo "      ------------------------------------------ "
	@for DIRSET in  $$(find src/lib  -name Makefile)        \
			$$(find src/apps -name Makefile)        \
			$$(find test     -name Makefile);       \
	do                                                      \
		DIR=$${DIRSET%/*};                              \
		$(MAKE) -s --directory=$$DIR clean;             \
	done;
	@rm -rf lib/
	@rm -rf bin/
	@rm -rf build/
	@rm -rf include/
	@rm -rf script/
	@rm -f #*
	@rm -f *~
	@echo "       :   cleaned: environment               :  "
	@echo "      ------------------------------------------ "
	@echo "       :        All Clean                     :  "
	@echo "      ------------------------------------------ "
	@echo

#==============================================================================#
