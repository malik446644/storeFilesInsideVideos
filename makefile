main: clear temp/main.o
	$(info =========> linking the app <=========)
	g++ \
	-o dist/main.exe \
	temp/*.o \
	-L./libs/ffmpeg/lib \
	-lavcodec \
	-lavformat  \
	-lavdevice  \
	-lavfilter  \
	-lavutil  \
	-lswscale  \
	-lswresample \
	-lpostproc

	dist/main.exe

encode: clear temp/main.o
	$(info =========> linking the app <=========)
	g++ \
	-o dist/main.exe \
	temp/*.o \
	-L./libs/ffmpeg/lib \
	-lavcodec \
	-lavformat  \
	-lavdevice  \
	-lavfilter  \
	-lavutil  \
	-lswscale  \
	-lswresample \
	-lpostproc

	dist/main.exe encode

decode: clear temp/main.o
	$(info =========> linking the app <=========)
	g++ \
	-o dist/main.exe \
	temp/*.o \
	-L./libs/ffmpeg/lib \
	-lavcodec \
	-lavformat  \
	-lavdevice  \
	-lavfilter  \
	-lavutil  \
	-lswscale  \
	-lswresample \
	-lpostproc

	dist/main.exe decode

temp/main.o: src/main.cpp
	$(info =========> compiling the app <=========)
	g++ \
	-std=c++17 \
	-g \
	-c \
	src/*.cpp \
	-I./libs/ffmpeg/include

	move *.o temp

clear:
	del temp\*.o
	del dist\*.exe
	del *.o