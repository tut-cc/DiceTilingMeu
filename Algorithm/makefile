include ../makefile.in

OUTDIR := ../lib
ifeq ($(buildtype),release)
	TARGET = libdtma.a
else ifeq ($(buildtype),debug)
	TARGET = libdtmad.a
endif

SRCS := $(wildcard src/*.cpp)
PROG := $(OUTDIR)/$(TARGET)
OBJS := $(SRCS:src/%.cpp=obj/$(buildtype)/%.o)
DEPS := $(SRCS:src/%.cpp=obj/$(buildtype)/%.d)

CXXFLAGS +=
CPPFLAGS += -Iinclude

.PHONY: all clean distclean

all : $(PROG)

-include $(DEPS)

$(PROG) : $(OBJS)
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(AR) rs -o $@ $^

obj/$(buildtype)/%.o : src/%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEFS) -o $@ -c -MMD -MP -MF $(@:%.o=%.d) $<

clean:
	rm -rf $(OUTDIR)/$(TARGET) ./obj

distclean:
	rm -rf ../lib ./obj

# vim:set tabstop=4 noexpandtab

