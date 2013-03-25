PRODUCT = ofca
VERSION = 1.1
DEPENDENCIES = main.o globals.o parse.o execute.o dbase.o 
EXTRAS = -DDEBUG -O0

CC = g++
CFLAGS = -ansi -Wall -pedantic
DEFINE = -DVERSION=\"$(VERSION)\" -DPRODUCT=\"$(PRODUCT)\" $(EXTRAS)
INCLUDE =

DIRNAME = $(PRODUCT)-$(VERSION)

COMPILE = $(CC) $(CFLAGS) $(DEFINE) $(INCLUDE)

.SUFFIXES: .cpp .o

.cpp.o:
	$(COMPILE) -c $< -o $@

default: all

all: $(PRODUCT)

$(PRODUCT): $(DEPENDENCIES) 
	$(COMPILE) $(DEPENDENCIES) -o $(PRODUCT)

rebuild: clean all

clean:
	-rm *.o
	-rm $(PRODUCT)
	-rm $(PRODUCT).exe
	-rm *~

distclean: clean
	-rm $(DIRNAME).tar.bz2

dist: distclean
	mkdir ../$(DIRNAME)
	cp * ../$(DIRNAME)/
	tar -cvjf $(DIRNAME).tar.bz2 ../$(DIRNAME)
	-rm -r ../$(DIRNAME)
