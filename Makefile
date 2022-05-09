TARGET    = pwn-adv

CC        = i686-w64-mingw32-gcc-posix
CFLAGS    = -std=c99 -O3 -Wall -Wextra -Werror -fvisibility=hidden -funroll-loops

LD        = i686-w64-mingw32-gcc-posix
LDFLAGS   = -s -shared -lws2_32

LIB       = lib
BUILD     = build

SOURCE    = src
OBJECT    = build

SOURCES   = $(wildcard $(SOURCE)/*.c)
OBJECTS   = $(patsubst $(SOURCE)/%.c,$(OBJECT)/%.o,$(SOURCES))

INCLUDE   = include
INCLUDES  = $(addprefix -I,$(INCLUDE))

all: $(TARGET)


$(TARGET): $(LIB) $(BUILD) $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(LIB)/$(TARGET).dll

$(OBJECTS): $(OBJECT)/%.o : $(SOURCE)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	mkdir -p $@

$(BUILD):
	mkdir -p $@

clean:
	rm -fr lib/*
	rm -fr build/*

extra-clean:
	rm -fr lib
	rm -fr build
