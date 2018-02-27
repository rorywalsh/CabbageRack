# Must follow the format in the Naming section of https://vcvrack.com/manual/PluginDevelopmentTutorial.html
SLUG = CabbageRack

# Must follow the format in the Versioning section of https://vcvrack.com/manual/PluginDevelopmentTutorial.html
VERSION = 0.5.0

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=

ifndef ARCH
	ARCH=lin
endif 

ifeq ($(ARCH), win)
    CSOUND_INCLUDE ?= "c:\PROGRA~1\Csound6_x64\include"
    CSOUND_LIBRARY ?= "c:\PROGRA~1\Csound6_x64\lib\csound64.lib"
	CXXFLAGS += -I $(CSOUND_INCLUDE)
    LDFLAGS += $(CSOUND_LIBRARY)
endif

ifeq ($(ARCH), lin)
	CXXFLAGS += -I /usr/local/include/csound
	LDFLAGS += -L /usr/loca/lib -lcsound64
endif

#CXXFLAGS += -I /Library/Frameworks/CsoundLib64.framework/Headers 

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine.


#LDFLAGS +=   -F /Library/Frameworks/ -framework CsoundLib64 -rpath

# Add .cpp and .c files to the build
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin is automatically added.
DISTRIBUTABLES += $(wildcard LICENSE*) res

# If RACK_DIR is not defined when calling the Makefile, default to two levels above
RACK_DIR ?= ../..

# Include the VCV Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk
