SOURCES	= wakeup.c main.c
BINARY	= wakeup

$(BINARY): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(BINARY)
