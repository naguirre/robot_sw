C_COMP     = gcc
OBJC_COMP  = clang

detected_OS := $(shell uname -s)

# app src files

APP_C_SRC    = $(shell find . -type f -name '*.c')
APP_OBJ      = $(APP_C_SRC:.c=.o)
APP_FLAGS    = -Wall
APP_INC      = . utils

# libraries

LIB_PKG   = 
LIB_INC   = 
LIB_LPATH = 
LIB_LD    = -lm

# about C_COMP

CFLAGS = -fPIC $(APP_FLAGS) $(foreach d, $(APP_INC), -I$d) $(foreach d, $(LIB_INC), -I$d) $(foreach d, $(LIB_PKG), `pkg-config --cflags $d`)

#about link

LDFLAGS = $(foreach d, $(LIB_LPATH), -L$d) $(foreach d, $(LIB_PKG), `pkg-config --libs-only-L $d`) $(LIB_LD) $(foreach d, $(LIB_PKG), `pkg-config --libs-only-l $d`)

###########
# targets #
###########

%.o : %.c
	$(C_COMP) $(CFLAGS) -c -o $@ $<

%.o : %.m
	$(C_COMP) $(CFLAGS) $(OBJC_FLAGS) -c -o $@ $<

all: $(APP_OBJ)
	$(C_COMP) $(APP_OBJ) $(OBJC_FLAGS) $(LDFLAGS) -o deplacement

.PHONY: clean

clean:
	rm -f $(APP_OBJ) deplacement