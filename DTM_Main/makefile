include ../makefile.in

OUTDIR := ../bin/$(buildtype)
TARGET = procon

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
	$(CXX) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

obj/$(buildtype)/%.o : src/%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEFS) -o $@ -c -MMD -MP -MF $(@:%.o=%.d) $<

clean:
	rm -rf $(OUTDIR)/$(TARGET) ./obj

distclean:
	rm -rf ../bin ./obj

# vim:set tabstop=4 noexpandtab

