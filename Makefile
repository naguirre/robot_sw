
detected_OS := $(shell uname -s)

# app src files

APP_C_SRC    = $(shell find . -type f -name '*.cpp')
APP_OBJ      = $(APP_C_SRC:.cpp=.o)
APP_FLAGS    = -Wall -g -std=c++11
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

%.o : %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

%.o : %.m
	$(CXX) $(CFLAGS) $(OBJC_FLAGS) -c -o $@ $<

all: $(APP_OBJ)
	$(CXX) $(APP_OBJ) $(OBJC_FLAGS) $(LDFLAGS) -o robot

.PHONY: clean

clean:
	rm -f $(APP_OBJ) robot
