PROJ = apriori
CC = g++

CFLAGS = -std=c++11 -c -g -Wall
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(PROJ)

$(PROJ): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(VAR)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROJ) $(OBJS) $(TEST_CASES) *.xml