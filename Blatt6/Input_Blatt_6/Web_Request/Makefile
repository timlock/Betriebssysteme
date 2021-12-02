BIN_SIMPLE = simple_bot
INCDIR     = ./include/
LIBDIR     = ./lib/
SRCDIR     = ./
CFLAGS     = -Wall -Werror -Wpedantic -g -I$(INCDIR)
CPPFLAGS   = $(CFLAGS) -std=c++11
LDFLAGS    = -lm -lssl -lcrypto -pthread /usr/lib/x86_64-linux-gnu/libcurl.so.4 #-lcurl

# executables
all: $(OBJFILES) | $(BIN_SIMPLE)

$(BIN_SIMPLE): $(SRCDIR)/simple_bot.o $(LIBDIR)/web_request.o
	gcc -o $@ $^ $(LDFLAGS)

# object files
%.o: %.c
	gcc -c -o $@ $(CFLAGS) $^

%.o: %.cpp
	g++ -c -o $@ $(CPPFLAGS) $^

.PHONY: clean
clean:
	$(RM) $(BIN_SIMPLE)
	$(RM) $(wildcard $(SRCDIR)*.o)
	$(RM) $(wildcard $(LIBDIR)*.o)
	$(RM) *.html
