PREFIX  = ../build
INCLUDE += -I .
INCLUDE += -I ../exec_once
INCLUDE += -I ../voba_str
INCLUDE += -I ../vhash
INCLUDE += -I ~/d/other-working/GC/bdwgc/include
GC_PATH := /home/chunywan/d/other-working/GC/bdwgc/mybuild

CFLAGS   += $(INCLUDE)
CXXFLAGS += $(INCLUDE)

FLAGS += -Wall -Werror
FLAGS += -fPIC

CFLAGS += -ggdb -O0
CFLAGS += -std=c99
CFLAGS += $(FLAGS)
CFLAGS += -D_BSD_SOURCE # otherwise realpath is not defined.

CXXFLAGS += -std=c++11
CXXFLAGS += $(FLAGS)

LDFLAGS  += -L $(GC_PATH) -Wl,-rpath,$(GC_PATH) -lgcmt-dll

all: install

install: libvoba_value.so
	install libvoba_value.so $(PREFIX)/voba/lib/
	install value.h $(PREFIX)/voba/include/value.h
	install voba_for_each.h $(PREFIX)/voba/include/
	install data_type_imp.h $(PREFIX)/voba/include/
	install data_type_imp.c $(PREFIX)/voba/include/

libvoba_value.so: voba_value.o  voba_value_cpp.o
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^  $(LDFLAGS)

voba_value.o: voba_value.c value.h data_type_imp.h data_type_imp.c
voba_value_cpp.o: voba_value_cpp.cc value.h data_type_imp.h data_type_imp.c hash_paul.inc

clean:
	rm *.o *.so

.PHONY: all clean install

