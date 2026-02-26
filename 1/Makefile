CC = gcc
CFLAGS = -g -Wall -pedantic -ansi -I.

OBJDIR = obj
BINDIR = bin
DATA = radio1.txt

EJS = $(BINDIR)/p1_e1 $(BINDIR)/p1_e2 $(BINDIR)/p1_e3

OBJS1 = $(OBJDIR)/p1_e1.o $(OBJDIR)/music.o
OBJS2 = $(OBJDIR)/p1_e2.o $(OBJDIR)/radio.o $(OBJDIR)/music.o
OBJS3 = $(OBJDIR)/p1_e3.o $(OBJDIR)/radio.o $(OBJDIR)/music.o

all: $(OBJDIR) $(BINDIR) $(EJS)

$(BINDIR)/p1_e1: $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/p1_e2: $(OBJS2)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/p1_e3: $(OBJS3)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR) $(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/p1_e1.o: p1_e1.c music.h
$(OBJDIR)/p1_e2.o: p1_e2.c music.h radio.h
$(OBJDIR)/p1_e3.o: p1_e3.c music.h radio.h
$(OBJDIR)/music.o: music.c music.h
$(OBJDIR)/radio.o: radio.c radio.h music.h

run: all
	@echo ">>>>>> Running p1_e1"
	./$(BINDIR)/p1_e1
	@echo ">>>>>> Running p1_e2"
	./$(BINDIR)/p1_e2
	@echo ">>>>>> Running p1_e3"
	./$(BINDIR)/p1_e3 $(DATA)

runv: all
	@echo ">>>>>> Running p1_e1 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p1_e1
	@echo ">>>>>> Running p1_e2 with valgrind"
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/p1_e2
	@echo ">>>>>> Running p1_e3 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p1_e3 $(DATA)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean run runv
