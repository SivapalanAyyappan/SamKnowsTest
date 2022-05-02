CC = gcc
CFLAGS = -Wall -g -O -fPIC
CFLAGS += -I include/

RM= rm -f

LIB_PATH = -L httprequest/src/
TARGET = StatsMedianApp
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)
LIBS = $(LIB_PATH) -lHttpStats

all: $(TARGET)

clean:
	cd httprequest/src/ && make clean
	$(RM) *.o *.so


$(TARGET): $(OBJECTS)
	cd httprequest/src/ && make
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) 
