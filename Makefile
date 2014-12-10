ifdef TCAMAKE_PROJECT
    TOPDIR = ../..
else
    TOPDIR = .
endif

NEED_SOCKET = 1
NEED_PTHREADS = 1
NEED_TCANETPP = 1
NEED_TCACMDBUF = 1


ifdef TRANSAM_DEBUG
OPT_FLAGS= 	-g
endif

INCLUDES=       -Iinclude $(shell pkg-config --cflags taglib)
LIBS=		$(shell pkg-config --libs taglib)


BIN=		transam showtags
OBJS=		src/TransFile.o src/Encode.o src/Decode.o \
		src/transam_main.o
SHTOBJ=		src/showtags.o
ALL_OBJS=	$(OBJS) $(SHTOBJ)
ALL_BINS=	$(BIN)

all: transam

include $(TOPDIR)/tcamake/project_defs


transam: $(OBJS)
	$(make-cxxbin-rule)
	@echo

showtags: $(SHTOBJ) src/TransFile.o
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