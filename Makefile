default: project

src_creature.o: ./src/creature.cpp
	em++ -c -g -Os -Wno-narrowing -std=c++20 -I./src -I../tram-sdk/libraries -I../tram-sdk/src -Wno-undefined-var-template -I../tram-sdk/libraries/bullet -I../tram-sdk/libraries/lua ./src/creature.cpp -o src_creature.o

src_main.o: ./src/main.cpp
	em++ -c -g -Os -Wno-narrowing -std=c++20 -I./src -I../tram-sdk/libraries -I../tram-sdk/src -Wno-undefined-var-template -I../tram-sdk/libraries/bullet -I../tram-sdk/libraries/lua ./src/main.cpp -o src_main.o

src_progresscontroller.o: ./src/progresscontroller.cpp
	em++ -c -g -Os -Wno-narrowing -std=c++20 -I./src -I../tram-sdk/libraries -I../tram-sdk/src -Wno-undefined-var-template -I../tram-sdk/libraries/bullet -I../tram-sdk/libraries/lua ./src/progresscontroller.cpp -o src_progresscontroller.o

clean:
	del src_creature.o
	del src_main.o
	del src_progresscontroller.o

project: src_creature.o src_main.o src_progresscontroller.o 
	em++ -g src_creature.o src_main.o src_progresscontroller.o -sASSERTIONS=2 -sSAFE_HEAP=0 -sALLOW_MEMORY_GROWTH -sSTACK_OVERFLOW_CHECK=1 -sUSE_BULLET=1 -sUSE_GLFW=3 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -L./ -o index.html --preload-file ./data/@/data/ --preload-file ./shaders/@/shaders/ --preload-file ./scripts/@/scripts/       -L../tram-sdk/libraries/binaries/web/ -ltramsdk -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath       -llua 