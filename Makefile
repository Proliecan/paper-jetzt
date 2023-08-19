# vars
srcDir = src
hdrDir = src
objDir = out/obj
binDir = out
# src
src := $(wildcard $(srcDir)/*.cpp)
# hdr
hdr := $(wildcard $(hdrDir)/*.h)
# obj
obj := $(patsubst $(srcDir)/%.cpp, $(objDir)/%.o, $(src))
# bin
bin = $(binDir)/server
# compiler
CC = g++
# flags
CFLAGS = -Wall -Wextra -Werror -std=c++11 -g

# target
all: $(bin)

$(bin): $(obj)
	@mkdir -p $(binDir)
	$(CC) $(CFLAGS) -o $@ $^

$(objDir)/%.o: $(srcDir)/%.cpp $(hdr)
	@mkdir -p $(objDir)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rfv $(objDir) $(binDir)