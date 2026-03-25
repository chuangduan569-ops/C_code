LIB += -lcrypt
TARGET = eff
OBJS = main.o eff.o

$(TARGET): $(OBJS)
	gcc -o $@ $^ $(LIB)

main.o: main.c eff.h
	gcc -c main.c -o main.o

eff.o: eff.c eff.h
	gcc -c eff.c -o eff.o

clean:
	rm -rf *.o $(TARGET)
