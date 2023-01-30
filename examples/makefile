final:
		sudo cp /usr/local/include/para_api/stamp.h .
		sudo cp /usr/local/include/para_api/stamp.so .
		g++ -L/usr/local/include -Wl,-rpath=/usr/local/include -Wall -o matrix matrix.cpp -l:stamp.so
		g++ -L/usr/local/include -Wl,-rpath=/usr/local/include -Wall -o vector vector.cpp -l:stamp.so
		g++ -L/usr/local/include -Wl,-rpath=/usr/local/include -Wall -o fib fib.cpp -l:stamp.so 
