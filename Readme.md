# TCP-Socket-Wrapper (C++)

These wrappers are meant to speed up the declaration of a TCP Socket Connection in C++ on Windows.
- Multi platform version is coming.
- Multi client connection supported via threads.
- Includes 2 main files to test the headers.


Please compile adding the -lws2_32 flag:

	g++ ClientMain.cpp -o main -lws2_32
