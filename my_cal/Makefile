INCLUDES	:= -I../include/

%.o: %.c
	$(CC) $(DEFS) $(CFLAGS) $(INCLUDES) -c $<
%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: my_driver.o my_cal.o

clean:
	rm -rf *~ *.o

include ../include.mk
