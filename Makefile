#
# Makefile
#
# To make all examples, just type 'make' or 'gmake'.
#
# For a specific example, type 'make' or 'gmake' followed by the project,
# such as 'make FindRsrc'.
#
##################################################################

# Directory Information
# In your project, set INC_DIR to where visa.h is located.
# In your project, LIB_DIR should be set to where libvisa.so is located.
INC_DIR  = -I /usr/include/ni-visa
LIB_DIR  = -L /usr/lib/x86_64-linux-gnu

# Compiler Information
CC       = gcc
CC_FLAGS = -ansi -O1 -D_REENTRANT $(INC_DIR)

# Linker Information
LD       = gcc
LD_FLAGS = $(LIB_DIR)
LD_LIBS  = -lvisa -lc -lpthread

##################################################################

TARGETS=     \
   AsyncIO   \
   AsyncSRQ  \
   FindRsrc  \
   Locking   \
   RdWrt     \
   RdWrtSrl  \
   ReserveTrig \
   RouteTrig \
   TCPIP     \
   WaitSRQ

.PHONY: $(TARGETS)

all: $(TARGETS)

##################################################################

AsyncIO: gpib/AsyncIO

AsyncSRQ: gpib/AsyncSRQ

FindRsrc: general/FindRsrc

Locking: general/Locking

RdWrt: general/RdWrt

RdWrtSrl: serial/RdWrtSrl

ReserveTrig: pxi/ReserveTrig

RouteTrig: pxi/RouteTrig

TCPIP: tcpip/TCPIP

WaitSRQ: gpib/WaitSRQ

clean:
	rm -f general/FindRsrc general/Locking general/RdWrt
	rm -f gpib/AsyncIO gpib/AsyncSRQ gpib/WaitSRQ
	rm -f pxi/ReserveTrig pxi/RouteTrig
	rm -f serial/RdWrtSrl
	rm -f tcpip/TCPIP
	find . -name "*.o" -exec rm -f {} \;

##################################################################

general/FindRsrc: general/FindRsrc.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

general/Locking: general/Locking.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

general/RdWrt: general/RdWrt.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

gpib/AsyncIO: gpib/AsyncIO.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

gpib/AsyncSRQ: gpib/AsyncSRQ.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

gpib/WaitSRQ: gpib/WaitSRQ.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

pxi/ReserveTrig: pxi/ReserveTrig.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

pxi/RouteTrig: pxi/RouteTrig.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

serial/RdWrtSrl: serial/RdWrtSrl.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@

tcpip/TCPIP: tcpip/tcpip.c
	$(CC) $(CC_FLAGS) -c $< -o $@.o
	$(LD) $(LD_FLAGS) $@.o $(LD_LIBS) -o $@
