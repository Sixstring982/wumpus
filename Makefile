PROJNAME:=wumpus

SRCDIR=./src
INCDIR=$(SRCDIR)/inc
TOOLDIR=./tools
BUILDDIR=./build
BINDIR=./bin

SRCEXT:=.cpp
INCEXT:=.hpp
OBJEXT:=.o

SOURCES:=$(shell find $(SRCDIR) -type f -name '*$(SRCEXT)')
HEADERS:=$(shell find $(INCDIR) -type f -name '*$(INCEXT)')
OBJECTS:=$(subst $(SRCDIR),$(BUILDDIR),$(subst $(SRCEXT),$(OBJEXT),$(SOURCES)))

CC:=clang++-3.5
STDCCFLAGS:=-Weverything -Werror -Wno-padded -Wno-weak-vtables
CCCOMMAND=$(CC) $(CCFLAGS)
LINT:=$(TOOLDIR)/cpplint.py

EXE:=$(BINDIR)/$(PROJNAME)

ifdef DEBUG
CCFLAGS:=$(STDCCFLAGS) -g
else
CCFLAGS:=$(STDCCFLAGS) -O3
endif

all:	$(BUILDDIR) $(BINDIR) $(EXE)

$(EXE):	$(OBJECTS)
	$(CCCOMMAND) -o $@ $^

$(BUILDDIR) $(BINDIR):
	mkdir -p $@

$(BUILDDIR)/%$(OBJEXT):	$(SRCDIR)/%$(SRCEXT) $(INCDIR)/%$(INCEXT)
	$(CCCOMMAND) -c -o $@ $<

$(BUILDDIR)/%$(OBJEXT):	$(SRCDIR)/%$(SRCEXT)
	$(CCCOMMAND) -c -o $@ $^

lint:
	$(LINT) $(SOURCES) $(HEADERS)

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY:	lint clean
