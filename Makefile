.PHONY: clean

BINDIR= build
SRCDIR= src

LDFLAGS= -lbluetooth -lpthread -lmysqlpp -lcurl
CXXFLAGS= -I/usr/include/mysql -I/usr/include/mysql++ -lmysqlpp -std=c++11 -g3

SOURCES= $(wildcard $(SRCDIR)/*.cpp)
INCLUDES= $(wildcard $(SRCDIR)/*.h)
OBJECTS= $(SOURCES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

TARGET= advFetcher

all: dir $(BINDIR)/$(TARGET)

dir:
		mkdir -p $(BINDIR)

clean:
		rm -f $(BINDIR)/*.o $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
		$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECTS): $(BINDIR)/%.o : $(SRCDIR)/%.cpp
		$(CXX) -c $(CXXFLAGS) $^ -o $@
