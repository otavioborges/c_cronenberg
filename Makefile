.PHONY: clean
.PHONY: install
.PHONY: uninstall

CXXFLAGS= -fPIC -Wall -std=c++11 -g3
LDFLAGS= -shared

BINDIR= build

SOURCES= $(wildcard *.cpp)
OBJECTS= $(patsubst %.cpp,$(BINDIR)/%.o,$(SOURCES))

HEADERS= $(wildcard *.h)

TARGET_LIB= libcronenberg.so

all: dir $(BINDIR)/$(TARGET_LIB)

dir:
	mkdir -p $(BINDIR)

clean:
	rm -f $(BINDIR)/*o $(BINDIR)/$(TARGET_LIB)

install:
	install -d /usr/lib
	install -m 644 $(BINDIR)/$(TARGET_LIB) /usr/lib
	install -d /usr/include/cronenberg
	install -m 644 $(HEADERS) /usr/include/cronenberg

uninstall:
	rm /usr/lib/$(TARGET_LIB)
	rm -rf /usr/include/cronenberg

$(BINDIR)/$(TARGET_LIB): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(BINDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@ -lpthread
