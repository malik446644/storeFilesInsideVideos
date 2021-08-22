main: temp/*.o
	$(info =========> linking the app <=========)
	gcc \
	-o dist/videod.exe \
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

temp/*.o: src/*.cpp
	$(info =========> compiling the app <=========)
	gcc \
	-std=c++17 \
	-g \
	-c \
	src/*.cpp \
	-I./libs/ffmpeg/include

	mv *.o ./temp
