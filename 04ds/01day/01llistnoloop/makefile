#list : main.o llist.o
#	gcc -o $@ &^
#clean :
#	rm -rf *.o list
TARGET	:=	main
LLIST	:=	main.o llist.o
COMPILE	:=	gcc
$(TARGET):$(LLIST)
	$(COMPILE) -o $(TARGET) $(LLIST)
clean :
	rm -rf $(TARGET) $(LLIST)
