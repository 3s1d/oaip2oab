PROG=oaip2oab
CPPFLAGS=-g -Wall
BUILDDIR=build
LDFLAGS=-lboost_system -lboost_filesystem -lexpat -lboost_iostreams -lboost_regex 
OBJS=aiphandler.o coord.o xml.o oaip2oab.o oab.o


all: $(OBJS)
	mkdir -p $(BUILDDIR)
	$(CXX) -o $(BUILDDIR)/$(PROG) -I/usr/include $(OBJS) $(LDFLAGS)

clean:
	rm -rf *.o $(BUILDDIR)

install:
	cp $< build/oaip2oab /usr/local/bin

uninstall:
	rm -f /usr/local/bin/oaip2oab
