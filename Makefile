
INCLUDE += -I .
INCLUDE += -I ../exec_once
INCLUDE += -I ../voba_str
INCLUDE += -I ../../vhash
INCLUDE += -I ~/d/other-working/GC/bdwgc/include
GC_PATH := /home/chunywan/d/other-working/GC/bdwgc/mybuild
LIBS += -L $(GC_PATH)
CFLAGS   += $(INCLUDE)
CXXFLAGS += $(INCLUDE)
LDFLAGS  += $(LIBS)
LDFLAGS  += -Wl,-rpath,$(GC_PATH) -lgcmt-dll


FLAGS += -Wall -Werror
FLAGS += -fPIC



CFLAGS += -ggdb -O0
CFLAGS += -std=c99
CFLAGS += $(FLAGS)


CXXFLAGS += -std=c++11
CXXFLAGS += $(FLAGS)

libvoba_value.so: voba_value.o  voba_value_cpp.o
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,$@ -Wl,-rpath,/home/chunywan/d/other-working/GC/bdwgc/mybuild -o $@ $^ -lgcmt-dll

voba_value.o: voba_value.c voba_value.h data_type_imp.h data_type_imp.c inline.h
voba_value_cpp.o: voba_value_cpp.cc voba_value.h data_type_imp.h data_type_imp.c inline.h hash_paul.inc

clean:
	rm *.o *.so

.PHONY: all clean

