# Makefile - qdiff

ifndef CFG
CFG=Debug
endif
CC=gcc
CFLAGS=-m32 -fpic
CXX=g++
CXXFLAGS=$(CFLAGS)
else
ifeq "$(CFG)" "Debug"
CFLAGS+=  -W  -O0 -fexceptions -g  -fno-inline  DWIN32 -D_DEBUG -D_CONSOLE  -I C:\dev\sdk\boost_1_38_0;C:\dev\sdk\boost_1_38_0\boost
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=
LDFLAGS+= -LC:\dev\sdk\boost_1_38_0\stage\lib
LIBS+=-lstdc++ -lm
ifndef TARGET
TARGET=qdiff.exe
endif
else
ifeq "$(CFG)" "Release"
CFLAGS+=  -W  -O2 -fexceptions -g  -fno-inline  DWIN32 -DNDEBUG -D_CONSOLE 
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=
LDFLAGS+= 
LIBS+=-lstdc++ -lm
ifndef TARGET
TARGET=qdiff.exe
endif
endif
endif
ifndef TARGET
TARGET=qdiff.exe
endif
.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.res: %.rc
	$(RC) $(CPPFLAGS) -o $@ -i $<

SOURCE_FILES= \
	./argumenthelper.cpp \
	./arguments.cpp \
	./comparer.cpp \
	./crc32.cpp \
	./fileservice.cpp \
	./md5.cpp \
	./qdiff.cpp \
	./sort.cpp

HEADER_FILES= \
	./argumenthelper.h \
	./arguments.h \
	./comparer.h \
	./crc32.h \
	./fileservice.h \
	./md5.h \
	./sort.h \
	./targetver.h

RESOURCE_FILES= \

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) qdiff.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > qdiff.dep

-include qdiff.dep

