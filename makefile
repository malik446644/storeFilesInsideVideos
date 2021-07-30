main: main.o
	$(info linking the app)
	g++ main.o

main.o: main.cpp
	$(info compiling the app)
	g++ \
	-std=c++17 \
	-c \
	main.cpp \

clean:
	del *.o
	del a.exe