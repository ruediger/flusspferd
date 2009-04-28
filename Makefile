-include .localsettings.mak

WAFFLAGS ?= -v
ifdef NOTNOISY
WAFFLAGS := 
endif

.PHONY: all clean reconfigure distclean test
all: .configure
	$(PYTHON) ./waf $(WAFFLAGS) 2>&1 | sed -e 's/^\.\.\//\.\//' -e 's/cxx://' \
-e 's/[0-9][0-9]:[0-9][0-9]:[0-9][0-9]//g'

.configure:
	$(PYTHON) ./waf configure
	@touch .configure

reconfigure:
	$(PYTHON) ./waf configure

install:
	$(PYTHON) ./waf install

clean:
	rm -f .configure
	$(PYTHON) ./waf clean

distclean:
	rm -f .configure
	$(PYTHON) ./waf distclean

test: all
	@util/run_tests.sh