CC = gcc
CFLAGS = -g -Wall -I.
LDFLAGS = -L.
LDLIBS = -lstack

OBJDIR = obj
BINDIR = bin

EJS = $(BINDIR)/p4_e1 $(BINDIR)/p4_e2 

OBJS = $(addprefix $(OBJDIR)/, radio.o music.o queue.o bstree.o list.o)

all: $(OBJDIR) $(BINDIR) $(EJS)

$(BINDIR)/p4_e1: $(OBJDIR)/p4_e1.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BINDIR)/p4_e2: $(OBJDIR)/p4_e2.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJDIR) $(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/p4_e1.o: p4_e1.c music.h radio.h queue.h
$(OBJDIR)/p4_e2.o: p4_e2.c music.h radio.h
$(OBJDIR)/music.o: music.c music.h types.h
$(OBJDIR)/radio.o: radio.c radio.h music.h types.h queue.h stack.h
$(OBJDIR)/queue.o: queue.c queue.h types.h
$(OBJDIR)/stack.o: stack.c stack.h types.h
$(OBJDIR)/list.o: list.c list.h types.h
$(OBJDIR)/bstree.o: bstree.c bstree.h list.h types.h

run: all
	@echo ">>>>>> Running p4_e1"
	./$(BINDIR)/p4_e1 data_music_10.txt 8 normal
	@echo ">>>>>> Running p4_e1 sorted"
	./$(BINDIR)/p4_e1 data_music_10.txt 8 sorted
	@echo ">>>>>> Running p4_e2"
	./$(BINDIR)/p4_e2 data_music_10.txt 3 7

runv: all
	@echo ">>>>>> Running p4_e1 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p4_e1 data_music_10.txt 8 normal
	@echo ">>>>>> Running p4_e1 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p4_e1 data_music_10.txt 8 sorted
	@echo ">>>>>> Running p4_e2 with valgrind"
	valgrind --leak-check=full ./$(BINDIR)/p4_e2 data_music_10.txt 3 7

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean run runv