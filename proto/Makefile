OFLAGS += -O3 -DNDEBUG
GFLAGS += -g
CFLAGS += -I/opt/local/include
LDLIBS += -L/opt/local/lib
LDLIBS += -lgc -lm

MAIN = minproto

all : $(MAIN)

% : %.c
	$(CC) $(GFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

%-opt : %.c
	$(CC) $(OFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

%.c : %.leg
	leg -o $@ $<

test : minproto
	./minproto < test.txt

bench : $(MAIN)-opt
	time ./$(MAIN)-opt -O < bench.txt
	time ./$(MAIN)-opt -O < bench.txt
	time ./$(MAIN)-opt -O < bench.txt

FILES = Makefile $(MAIN).leg test.txt bench.txt

checkpoint : .FORCE
	tar cvfz ckpt/minproto-$(shell date "+%Y%m%d-%H%M%S").tgz $(FILES)

clean : .FORCE
	rm -rf $(MAIN) $(MAIN)-opt *.o *.dSYM *.sync-conflict-*

spotless : clean
	rm -f *~

.FORCE :
