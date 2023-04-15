

FIRSTNAME=Cyrus
LASTNAME=Bayat

ROOTNAME=$(LASTNAME)_$(FIRSTNAME)
Hi=32
FOPTION=_main
RUNOPTIONS="Prompt> "
CC=gcc
CFLAGS= -g -I.
LIBS =-l pthread
DEPS = 
OBJ = $(ROOTNAME)$(Hi)$(FOPTION).o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(ROOTNAME)$(Hi)$(FOPTION): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o $(ROOTNAME)$(Hi)$(FOPTION)

run: $(ROOTNAME)$(Hi)$(FOPTION)
	./$(ROOTNAME)$(Hi)$(FOPTION) $(RUNOPTIONS)

vrun: $(ROOTNAME)$(Hi)$(FOPTION)
	valgrind ./$(ROOTNAME)$(Hi)$(FOPTION) $(RUNOPTIONS)


