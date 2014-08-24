.SUFFIXES:
.PHONY: _

_: queue

queue: queue.o
	cc -std=c11 -o $@ $^

%.o : %.c
	cc -std=c11 -o $@ -c $<

clean:
	@rm -rf queue.o queue
