CC      = gcc
CFLAGS  = -Wall -Wextra -I.
LIBS    = -lssl -lcrypto

all: table_builder lookup analyzer

table_builder: table_builder.c rainbow.h
	$(CC) $(CFLAGS) table_builder.c -o table_builder $(LIBS)

lookup: lookup.c rainbow.h
	$(CC) $(CFLAGS) lookup.c -o lookup

analyzer: analyzer.c rainbow.h
	$(CC) $(CFLAGS) analyzer.c -o analyzer $(LIBS)

clean:
	rm -f table_builder lookup analyzer rainbow_table.txt
