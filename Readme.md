# Neworks PA2
### Christopher Bate

## About
The assignment implements all requirements specified in PA2, including the optional requirements ofr POST, pipelining, and multithreading. 

## Compiling
Compilation is accomplished by running "make" in the source directory. The makefile uses GCC g++ and compiles with with the C++ 11 Standard. Two outputs are produced "server" which is the main server deliverable and "server_tests" which implements simple unit testing. A Docker file is also included for compiling and running the program.

## Code Organization
The code is organized into the following major classes:

- HTTPHeader - Responsible for parsing the data contained received and thought to contain the HTTP header message. 
- HTTPResponse - Constructs an HTTPResponse string from a HTTPHeader object and other user-given parameters (i.e. keep alive, content length, content type)
- TCPSocket - Wraps the Linux socket creation, teardown, accept, and listen
- Session - The main worker for handling connections. Accepts a TCPSocket returned from TCPSocket's Accept method. Responsible for parsing the incoming data using HTTPHeader, using FileManager to open and read files, using HTTPResponse to construct the response header, and returning the data to peer. Also starts a timer and terminates itself if no further requests are made prior to timeout (10 seconds).
- FileManager - Handles finding files within the specified public web root folder, returning the correct content-type fields and content-length fields