all: menu.o

menu.o:
	gcc \
	  -c \
	  menu.cpp

rebuild:
	rm -f *.o
	${MAKE} all

clean:
	rm -f menu.o
