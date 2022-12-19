CC_x64 		= x86_64-w64-mingw32-g++
CC_x86 		= i686-w64-mingw32-g++
OPTIONS		= -lws2_32 -ldnsapi
OUTPUTFILE 	= build/dns
SOURCE 		= main.cpp

make: clean
	@mkdir -p build
	@$(CC_x64) $(SOURCE) $(OPTIONS) -o $(OUTPUTFILE)_x64.exe
	@echo "Compiled x64 Successfully"

	@$(CC_x86) $(SOURCE) $(OPTIONS) -o $(OUTPUTFILE)_x86.exe 
	@echo "Compiled x86 Successfully"

clean:
	@rm -rf build/*.exe

