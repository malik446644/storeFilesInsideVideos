main: temp/main.o
	$(info =========> linking the app <=========)
	g++ \
	-o dist/main.exe \
	temp/main.o \
	-L./libs/ffmpeg/lib \
	-lavcodec \
	-lavformat  \
	-lavdevice  \
	-lavfilter  \
	-lavutil  \
	-lswscale  \
	-lswresample \
	-lpostproc

	dist/main.exe test.mp4 libx264

temp/main.o: src/main.cpp
	$(info =========> compiling the app <=========)
	g++ \
	-std=c++17 \
	-g \
	-c \
	src/main.cpp \
	-o main.o \
	-I./libs/ffmpeg/include

	move *.o temp

clear:
	del temp\*.o
	del dist\*.exe