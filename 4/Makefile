CC = gcc
CFLAGS = -g -Wall -I.
LDFLAGS = -L.
LDLIBS = -lstack

OBJDIR = obj
BINDIR = bin

EJS = $(BINDIR)/p3_e1 $(BINDIR)/p3_e2 $(BINDIR)/p3_e3

OBJS1 = $(OBJDIR)/p3_e1.o $(OBJDIR)/radio.o $(OBJDIR)/music.o $(OBJDIR)/queue.o
OBJS2 = $(OBJDIR)/p3_e2.o $(OBJDIR)/radio.o $(OBJDIR)/music.o $(OBJDIR)/queue.o
OBJS3 = $(OBJDIR)/p3_e3.o $(OBJDIR)/radio.o $(OBJDIR)/music.o $(OBJDIR)/list.o $(OBJDIR)/queue.o

all: $(OBJDIR) $(BINDIR) $(EJS)

$(BINDIR)/p3_e1: $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BINDIR)/p3_e2: $(OBJS2)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BINDIR)/p3_e3: $(OBJS3)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJDIR) $(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/p3_e1.o: p3_e1.c music.h radio.h queue.h
$(OBJDIR)/p3_e2.o: p3_e2.c music.h radio.h
$(OBJDIR)/p3_e3.o: p3_e3.c music.h radio.h list.h
$(OBJDIR)/music.o: music.c music.h types.h
$(OBJDIR)/radio.o: radio.c radio.h music.h types.h queue.h stack.h
$(OBJDIR)/queue.o: queue.c queue.h types.h
$(OBJDIR)/list.o: list.c list.h types.h

run: all
	@echo ">>>>>> Running p3_e1"
	./$(BINDIR)/p3_e1 radio.txt
	@echo ">>>>>> Running p3_e2"
	./$(BINDIR)/p3_e2 radio_bfs.txt 1 10
	@echo ">>>>>> Running p3_e3"
	./$(BINDIR)/p3_e3 radio_bfs.txt

runv: all
	@echo ">>>>>> Running p3_e1 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p3_e1 radio.txt
	@echo ">>>>>> Running p3_e2 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p3_e2 radio_bfs.txt 1 2
	@echo ">>>>>> Running p3_e3 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p3_e3 radio_bfs.txt

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean run runv