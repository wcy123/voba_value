CFLAGS += -fPIC

all: libvoba_value.so

libvoba_value.so: voba_value.o  voba_value_cpp.o
	$(CXX) -shared -Wl,-soname,$@  -o $@ $<

voba_value.o: voba_value.c voba_value.h data_type_imp.h data_type_imp.c inline.h
voba_value_cpp.o: voba_value_cpp.cc voba_value.h data_type_imp.h data_type_imp.c inline.h hash_paul.inc

clean:
	rm *.o *.so

.PHONY: all clean
