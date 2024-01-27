BUILDDIR = $(PWD)/__build__
PREFIX := /usr/local
DESTDIR := $(PREFIX)

CXX := g++
CXXFLAGS := -std=c++23 -O2 -g -I$(PWD)/include -I$(PREFIX)/include -I$(PREFIX)/include/SDL2

AR := ar
ARFLAGS := rcs

SOURCES = $(shell find src -name "*.cpp")
OBJECTS = $(patsubst src/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))
DEPS    = $(patsubst %.o, %.d, $(OBJECTS))
TARGET  = $(BUILDDIR)/libtrollworks-sdk-backend-sdl.a

.PHONY: all
all: $(TARGET)

.PHONY: install
install:
	@mkdir -p $(DESTDIR)/include
	@cp -R $(PWD)/include $(DESTDIR)
	@cp $(TARGET) $(DESTDIR)/lib

.PHONY: clean
clean:
	@rm -vrf $(BUILDDIR)

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	@echo "  AR      $(patsubst $(BUILDDIR)/%, %, $@)"
	@$(AR) $(ARFLAGS) $@ $^

$(BUILDDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@echo "  CXX     $(patsubst $(BUILDDIR)/%, %, $@)"
	@$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

-include $(DEPS)
