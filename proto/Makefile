OFLAGS += -g
CFLAGS += -I/opt/local/include
LDLIBS += -L/opt/local/lib
LDLIBS += -lgc -lm

MAIN = minproto

all : $(MAIN)

% : %.c
	$(CC) $(OFLAGS) $(CFLAGS) $(OFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

%.c : %.leg
	leg -o $@ $<

test : minproto
	./minproto < test.txt

clean : .FORCE
	rm -rf $(MAIN) *.o *.dSYM

spotless : clean
	rm -f *~

.FORCE :
