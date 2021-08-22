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

temp/*.o: src/*.c
	$(info =========> compiling the app <=========)
	gcc \
	-g \
	-c \
	src/*.c \
	-I./libs/ffmpeg/include

	mv *.o ./temp
