# If RACK_DIR is not defined when calling the Makefile, default to two levels above
RACK_DIR ?= ../

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=

CXXFLAGS += -I /usr/local/include/csound
LDFLAGS += -L /usr/loca/lib -lcsound64


# Add .cpp and .c files to the build
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin is automatically added.
DISTRIBUTABLES += $(wildcard LICENSE*) res $(wildcard README*)

# Include the VCV Rack plugin Makefile framework
include ../plugin.mk