main: main.o
	$(info =========> linking the app <=========)
	g++ \
	-o dist/main.exe \
	main.o \
	-L./libs/ffmpeg/lib \
	-lavcodec \
	-lavformat  \
	-lavdevice  \
	-lavfilter  \
	-lavutil  \
	-lswscale  \
	-lswresample  \

main.o: main.cpp
	$(info =========> compiling the app <=========)
	g++ \
	-std=c++17 \
	-c \
	main.cpp \
	-I./libs/ffmpeg/include \

clear:
	del *.o
	del a.exe

run:
	dist/main.exe