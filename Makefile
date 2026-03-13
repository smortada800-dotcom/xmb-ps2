EE_BIN     = tokyo_xmb.elf
EE_OBJS    = main.o background.o xmb.o input.o
EE_LIBS    = -lgskit -ldmakit -lpatches -lpadx
EE_CFLAGS  = -O2 -G0 -Wall

EE_INCS    = -I$(PS2SDK)/ee/include \
             -I$(PS2SDK)/common/include \
             -I$(GSKIT)/include

EE_LDFLAGS = -L$(PS2SDK)/ee/lib \
             -L$(GSKIT)/lib

all: $(EE_BIN)

clean:
	rm -f $(EE_OBJS) $(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
