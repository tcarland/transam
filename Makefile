# Makefile for TransAm
#
NEED_PTHREADS = 1
NEED_DL = 1
NEED_LIBRT = 1
NEED_SOCKET = 1
NEED_TCANETPP = 1
NEED_TCACMDBUF = 1

ifdef TCAMAKE_DEBUG
OPT_FLAGS= -g
endif

CXXFLAGS=   -std=c++11
INCLUDES=   -Iinclude $(shell pkg-config --cflags taglib)
LIBS=	    $(shell pkg-config --libs taglib)
LFLAGS=     -L/usr/lib/x86_64-linux-gnu

BIN=        transam transcheck 
OBJS=       src/TransFile.o src/Encoder.o src/Decoder.o src/transam_main.o
TESTOBJ=    src/trans_check.o

ALL_OBJS=   $(OBJS) $(TESTOBJ)
ALL_BINS=   $(BIN)

# -----------------------------

ifeq ($(TCAMAKE_HOME),)
	TCAMAKE_HOME := $(shell realpath ../tcamake)
endif

include $(TCAMAKE_HOME)/tcamake_include

# -----------------------------

all: transam transcheck

transam: $(OBJS)
	$(make-cxxbin-rule)
	@echo

transcheck: $(TESTOBJ) src/TransFile.o
	$(make-cxxbin-rule)
	@echo

clean:
	$(RM) $(ALL_OBJS) \
	*.d *.D *.o src/*.d src/*.D src/*.bd src/*.o
	@echo

distclean: clean
	$(RM) $(ALL_BINS)
	@echo

install:
ifdef TCAMAKE_PREFIX
	$(CP) transam $(TCAMAKE_PREFIX)/bin/
	$(CP) transcheck $(TCAMAKE_PREFIX)/bin/
	$(CP) resources/transetTagByName.sh $(TCAMAKE_PREFIX)/bin/
	@echo
endif
