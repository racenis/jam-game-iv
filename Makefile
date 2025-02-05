default: project

src_creature.o: ./src/creature.cpp
	g++ -c -g -O0 -Wno-narrowing -std=c++20 -I./src -I../tram-sdk/libraries -I../tram-sdk/src -I../tram-sdk/libraries/glfw -I../tram-sdk/libraries/glad -I../tram-sdk/libraries/bullet -I../tram-sdk/libraries/lua ./src/creature.cpp -o src_creature.o

src_main.o: ./src/main.cpp
	g++ -c -g -O0 -Wno-narrowing -std=c++20 -I./src -I../tram-sdk/libraries -I../tram-sdk/src -I../tram-sdk/libraries/glfw -I../tram-sdk/libraries/glad -I../tram-sdk/libraries/bullet -I../tram-sdk/libraries/lua ./src/main.cpp -o src_main.o

src_progresscontroller.o: ./src/progresscontroller.cpp
	g++ -c -g -O0 -Wno-narrowing -std=c++20 -I./src -I../tram-sdk/libraries -I../tram-sdk/src -I../tram-sdk/libraries/glfw -I../tram-sdk/libraries/glad -I../tram-sdk/libraries/bullet -I../tram-sdk/libraries/lua ./src/progresscontroller.cpp -o src_progresscontroller.o

clean:
	del src_creature.o
	del src_main.o
	del src_progresscontroller.o

project: src_creature.o src_main.o src_progresscontroller.o 
	g++ -o poo -L../tram-sdk/ -static src_creature.o src_main.o src_progresscontroller.o -ltramsdk  -lglfw3 -lgdi32  -L../tram-sdk/libraries/binaries/win64/ -lOpenAL32    -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath       -llua 