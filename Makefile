# vars
rootSrcDir = src
rootBuildDir = out
rootObjDir = $(rootBuildDir)/obj
binDir = $(rootBuildDir)/bin

helperSrcDir = $(rootSrcDir)
helperObjDir = $(rootObjDir)
helperSrc = $(wildcard $(helperSrcDir)/*.cpp)
helperHdr = $(wildcard $(helperSrcDir)/*.hpp)
helperObj = $(patsubst $(helperSrcDir)/%.cpp, $(helperObjDir)/%.o, $(helperSrc))

serverSrcDir = $(rootSrcDir)/server
serverObjDir = $(rootObjDir)/server
serverBin = $(binDir)/server
serverSrc = $(wildcard $(serverSrcDir)/*.cpp)
serverHdr = $(wildcard $(serverSrcDir)/*.hpp)
serverObj = $(patsubst $(serverSrcDir)/%.cpp, $(serverObjDir)/%.o, $(serverSrc))

botSrcDir = $(rootSrcDir)/bot
botObjDir = $(rootObjDir)/bot
botBin = $(binDir)/bot
botSrc = $(wildcard $(botSrcDir)/*.cpp)
botHdr = $(wildcard $(botSrcDir)/*.hpp)
botObj = $(patsubst $(botSrcDir)/%.cpp, $(botObjDir)/%.o, $(botSrc))

# compiler
CC = g++
CFLAGS = -Wall -Wextra -Werror -Wno-unknown-pragmas -std=c++11 -g

#targets
all: server bot

server: $(serverBin)
$(serverBin): $(serverObj) $(helperObj)
	@mkdir -p $(binDir)
	$(CC) $(CFLAGS) -o $@ $^
$(serverObjDir)/%.o: $(serverSrcDir)/%.cpp $(serverHdr) $(helperHdr)
	@mkdir -p $(serverObjDir)
	$(CC) $(CFLAGS) -c -o $@ $<

bot: $(botBin)
$(botBin): $(botObj) $(helperObj)
	@mkdir -p $(binDir)
	$(CC) $(CFLAGS) -o $@ $^
$(botObjDir)/%.o: $(botSrcDir)/%.cpp $(botHdr) $(helperHdr)
	@mkdir -p $(botObjDir)
	$(CC) $(CFLAGS) -c -o $@ $<

helper: $(helperObj)
$(helperObjDir)/%.o: $(helperSrcDir)/%.cpp $(helperHdr)
	@mkdir -p $(helperObjDir)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(binDir) $(rootObjDir)
clean-server:
	rm -rf $(serverObjDir) $(serverBin)
clean-bot:
	rm -rf $(botObjDir) $(botBin)
clean-helper:
	rm -rf $(helperObjDir)