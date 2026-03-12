CC = gcc
CFLAGS = -g -Wall -pedantic -ansi -I.
LDFLAGS = -L.
LDLIBS = -lstack

OBJDIR = obj
BINDIR = bin
DATA = radio.txt  #a cambiar

EJS = $(BINDIR)/p2_e1 #$(BINDIR)/p2_e2a #$(BINDIR)/p2_e2b #$(BINDIR)/p2_e3

OBJS1  = $(OBJDIR)/p2_e1.o $(OBJDIR)/radio.o $(OBJDIR)/music.o
OBJS2A = $(OBJDIR)/p2_e2a.o $(OBJDIR)/radio.o $(OBJDIR)/music.o
OBJS2B = $(OBJDIR)/p2_e2b.o $(OBJDIR)/radio.o $(OBJDIR)/music.o
OBJS3  = $(OBJDIR)/p2_e3.o $(OBJDIR)/radio.o $(OBJDIR)/music.o

all: $(OBJDIR) $(BINDIR) $(EJS)

$(BINDIR)/p2_e1: $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BINDIR)/p2_e2a: $(OBJS2A)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BINDIR)/p2_e2b: $(OBJS2B)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BINDIR)/p2_e3: $(OBJS3)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJDIR) $(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/p2_e1.o: p2_e1.c music.h radio.h stack.h
$(OBJDIR)/p2_e2a.o: p2_e2a.c music.h radio.h stack.h
$(OBJDIR)/p2_e2b.o: p2_e2b.c music.h radio.h stack.h
$(OBJDIR)/p2_e3.o: p2_e3.c music.h radio.h stack.h
$(OBJDIR)/music.o: music.c music.h types.h
$(OBJDIR)/radio.o: radio.c radio.h music.h types.h

run: all
	@echo ">>>>>> Running p2_e1 with $(DATA)"
	./$(BINDIR)/p2_e1 $(DATA)

runv: all
	@echo ">>>>>> Running p2_e1 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p2_e1 $(DATA)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean run runv
