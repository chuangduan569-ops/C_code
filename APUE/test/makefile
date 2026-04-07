LDFLAGS = -lpthread

all: main collector processor outputer

main: main.c ipc.c msgq.c
	gcc -o main main.c ipc.c msgq.c $(LDFLAGS)

collector: collector.c ipc.c
	gcc -o collector collector.c ipc.c $(LDFLAGS)

processor: processor.c ipc.c msgq.c
	gcc -o processor processor.c ipc.c msgq.c $(LDFLAGS)

outputer: outputer.c msgq.c tbf.c
	gcc -o outputer outputer.c msgq.c tbf.c $(LDFLAGS)

clean:
	rm -f main collector processor outputer
