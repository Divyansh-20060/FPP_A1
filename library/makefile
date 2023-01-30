final:
		g++ -c -Wall -Werror -fpic stamp.cpp
		g++ -shared -o stamp.so stamp.o
		sudo mkdir -p /usr/local/include/para_api
		sudo cp stamp.h /usr/local/include/para_api
		sudo cp stamp.so /usr/local/include/para_api

