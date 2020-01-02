TOPDIR = ..

NEED_PTHREADS = 1
NEED_DL = 1
NEED_LIBRT = 1
NEED_SOCKET = 1
NEED_TCANETPP = 1
NEED_TCACMDBUF = 1

ifdef TRANSAM_DEBUG
OPT_FLAGS= 	-g
endif

CXXFLAGS=	-std=c++0x
INCLUDES=   -Iinclude $(shell pkg-config --cflags taglib)
LIBS=	    $(shell pkg-config --libs taglib)
LFLAGS=     -L/usr/lib/x86_64-linux-gnu

BIN=		transam transcheck transtag
OBJS=		src/TransFile.o src/Encode.o src/Decode.o src/transam_main.o
TRCOBJ=		src/trans_check.o

ALL_OBJS=	$(OBJS) $(TRCOBJ)
ALL_BINS=	$(BIN)

all: transam transcheck

include $(TOPDIR)/tcamake/project_defs


transam: $(OBJS)
	$(make-cxxbin-rule)
	@echo

transcheck: $(TRCOBJ) src/TransFile.o
	$(make-cxxbin-rule)
	@echo

.PHONY: test
test:

clean:
	$(RM) $(ALL_OBJS) \
	*.d *.D *.o src/*.d src/*.D src/*.bd src/*.o
	@echo

distclean: clean
	$(RM) $(ALL_BINS)
	@echo

install:
ifdef TRANSAM_PREFIX
	@echo
endif
