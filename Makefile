CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 
OBJDIR = obj
SRCDIR = src
BINDIR = bin


SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/dormitory_system


$(shell mkdir -p $(OBJDIR) $(BINDIR))

all: $(TARGET)


$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJDIR)/*.o $(TARGET)
rebuild: clean all

.PHONY: all clean rebuild 
