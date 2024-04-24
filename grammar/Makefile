GFLAGS += -g
CFLAGS += -I/opt/local/include
LDLIBS += -L/opt/local/lib
LDLIBS += -lgc -lm
DEPENDENCIES += objects.c

% : %.c
	$(CC) $(GFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(DEPENDENCIES) $(LDLIBS)

%.c : %.leg
	leg -o $@ $<

tree : grammar
	./grammar < grammar.leg

.PHONY: clean
clean :
	rm -rf grammar grammar.c *.o
