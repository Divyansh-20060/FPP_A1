final:
		sudo cp /usr/local/include/para_api/stamp.h .
		sudo cp /usr/local/include/para_api/stamp.so .
		g++ -L./ -Wl,-rpath=./ -Wall -o matrix matrix.cpp -l:stamp.so
		g++ -L./ -Wl,-rpath=./ -Wall -o vector vector.cpp -l:stamp.so
		g++ -L./ -Wl,-rpath=./ -Wall -o fib fib.cpp -l:stamp.so 
