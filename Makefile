CC=gcc
CFLAGS=-std=gnu99 -Iinclude

SOURCES = $(shell find src -name '*.c')
HEADERS = $(shell find include -name '*.h')
UNIT_TEST_HEADERS = $(shell find test/ -name '*.h')

bin/unit_tests: test/unit_tests.c $(UNIT_TEST_HEADERS) $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES)

test: bin/unit_tests
	./bin/unit_tests -c
