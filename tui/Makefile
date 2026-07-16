CC = gcc
CFLAGS = -fPIC -Wall -O2
TARGET = libtui.so
DEMO = demo
PREFIX ?= /usr/local

LANG_SRCS = $(wildcard lang/*.c)
LANG_OBJS = $(LANG_SRCS:.c=.o)

OBJS = tui_core.o tui_widget.o tui_lang.o $(LANG_OBJS)

all: $(TARGET) $(DEMO)

$(TARGET): $(OBJS)
	$(CC) -shared -o $(TARGET) $(OBJS)

tui_core.o: tui_core.c tui.h tui_lang.h
	$(CC) $(CFLAGS) -c tui_core.c

tui_widget.o: tui_widget.c tui.h
	$(CC) $(CFLAGS) -c tui_widget.c

tui_lang.o: tui_lang.c tui_lang.h
	$(CC) $(CFLAGS) -c tui_lang.c

$(LANG_OBJS): lang/%.o: lang/%.c tui_lang.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DEMO): tui_demo.c $(TARGET)
	$(CC) -Wall -O2 -o $(DEMO) tui_demo.c -L. -ltui -Wl,-rpath,'$$ORIGIN'

clean:
	rm -f $(OBJS) $(TARGET) $(DEMO)

install: $(TARGET)
	cp $(TARGET) $(PREFIX)/lib/
	cp tui.h $(PREFIX)/include/

.PHONY: all clean install