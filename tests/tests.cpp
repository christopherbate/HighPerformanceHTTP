#include "ctb_test.h"
#include "../src/HTTPRequest.h"
#include "../src/HTTPResponse.h"
#include "../src/TCPSocket.h"
#include <iostream>
#include <string>
using namespace std;
int main(int argc, char **argv)
{
    std::cout << "Starting tests." << std::endl;    

    TestRunner::GetRunner()->AddTest(
        "HTTP Response Creation",
        "Must create default protocol to HTTP/1.1",
        []() {
            HTTPResponse response;
            
            if(response.GetProtocol() != "HTTP/1.1"){
                return 0;
            }

            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Response Creation",
        "Must create default header with status 200",
        []() {
            HTTPResponse response;
            
            cout << response.GetHeader();

            if( response.GetHeader() != "HTTP/1.1 200 Document Follows\r\n\r\n"){
                return 0;
            }

            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Response Creation",
        "Must be able to set status code.",
        []() {
            HTTPResponse response;

            response.Status(500);
            
            cout << response.GetHeader();

            if( response.GetHeader() != "HTTP/1.1 500\r\n\r\n"){
                return 0;
            }

            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Response Creation",
        "Must be able to set headers.",
        []() {
            HTTPResponse response;

            response.SetHeaderField("Content-Type","text/html");
            
            cout << response.GetHeader();

            if( response.GetHeader() != "HTTP/1.1 200 Document Follows\r\nContent-Type: text/html\r\n\r\n"){
                return 0;
            }

            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Response Creation",
        "Must be able to create header from HTTPRequest header.",
        []() {
            string hdr = "GET /Protocols/rfc1945/rfc1945 HTTP/1.0";

            HTTPRequest request(hdr);
            HTTPResponse response(request);            
            
            cout << response.GetHeader();

            if( response.GetHeader() != "HTTP/1.0 200 Document Follows\r\n\r\n"){
                return 0;
            }

            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Response Creation - Files",
        "Must set content-length when given file",
        []() {
            string hdr = "GET /Protocols/rfc1945/rfc1945 HTTP/1.0";

            HTTPRequest request(hdr);
            HTTPResponse response(request);            
            
            return 0;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Request Parsing",
        "Must parse protocol version",
        []() {
            string hdr = "GET /Protocols/rfc1945/rfc1945 HTTP/1.1";
            HTTPRequest request(hdr);
            if (request.GetProtocol() != "HTTP/1.1")
                return 0;
            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Request Parsing",
        "Must parse url",
        []() {
            string hdr = "GET /Protocols/rfc1945/rfc1945 HTTP/1.1";
            HTTPRequest request(hdr);
            if (request.GetUrl() != "/Protocols/rfc1945/rfc1945")
                return 0;
            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Request Parsing",
        "Must parse GET method",
        []() {
            string hdr = "GET /Protocols/rfc1945/rfc1945 HTTP/1.1";
            HTTPRequest request(hdr);
            if (request.GetMethod() != "GET")
                return 0;
            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Request Parsing",
        "Must parse POST method",
        []() {
            string hdr = "POST /Protocols/rfc1945/rfc1945 HTTP/1.1";
            HTTPRequest request(hdr);
            if (request.GetMethod() != "POST")
                return 0;
            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Request Parsing",
        "Must parse keepalive",
        []() {
            string hdr = "POST /Protocols/rfc1945/rfc1945 HTTP/1.1\nConnection:Keep-alive";
            HTTPRequest request(hdr);
            if (request.GetKeepAlive() != true)
            {
                std::cerr << request.GetKeepAlive() << std::endl;
                return 0;
            }
            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "HTTP Request Parsing",
        "Must parse host",
        []() {
            string hdr = "POST /Protocols/rfc1945/rfc1945 HTTP/1.1\nHost:localhost\nConnection:Keep-alive";
            HTTPRequest request(hdr);
            if (request.GetHost() != "localhost")
            {
                std::cerr << request.GetHost() << std::endl;
                return 0;
            }
            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "TCP Socket Creation",
        "Must be able to create listen socket.",
        []() {
            TCPSocket socket;

            bool res = socket.CreateSocket("8080");
            if (!res)
            {
                std::cerr << "Failed to create socket." << endl;
                socket.ISocket::CloseSocket();
                return 0;
            }
            socket.ISocket::CloseSocket();
            return 1;
        });
    
    TestRunner::GetRunner()->AddTest(
        "TCP Socket Creation",
        "Must be able to listen on listen socket.",
        []() {
            TCPSocket socket;

            bool res = socket.CreateSocket("8080");
            if (!res)
            {
                std::cerr << "Failed to create socket." << endl;
                socket.ISocket::CloseSocket();
                return 0;
            }
            
            if(!socket.Listen() ){
                std::cerr << "Failed to listen." << endl;
                return 0;
            }
            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "TCP Socket Creation",
        "Must be able to create listen socket.",
        []() {
            TCPSocket socket;

            bool res = socket.CreateSocket("8080");
            if (!res)
            {
                std::cerr << "Failed to create list socket." << endl;
                return 0;
            }

            socket.Listen();

            TCPSocket connSocket;
            res = connSocket.CreateSocket("localhost", "8080");
            if (!res)
            {
                std::cerr << "Failed to create active socket." << endl;
                return 0;
            }

            connSocket.ISocket::CloseSocket();
            socket.ISocket::CloseSocket();
            return 1;
        });

    TestRunner::GetRunner()->AddTest(
        "TCP Socket Creation",
        "Must be able to accept connections",
        []() {
            TCPSocket socket;

            bool res = socket.CreateSocket("8080");
            if (!res)
            {
                std::cerr << "Failed to create list socket." << endl;
                return 0;
            }

            socket.Listen();

            TCPSocket connSocket;
            res = connSocket.CreateSocket("localhost", "8080");
            if (!res)
            {
                std::cerr << "Failed to create active socket." << endl;
                return 0;
            }

            TCPSocket *connection = socket.Accept();

            if(connection == NULL){
                std::cerr <<"Did not accept connection"<<endl;
                return 0;
            }

            delete connection;

            connSocket.ISocket::CloseSocket();
            socket.ISocket::CloseSocket();
            return 1;
        });

    TestRunner::GetRunner()->Run();

    TestRunner::GetRunner()->PrintSummary();

    return TestRunner::GetRunner()->GetRetCode();
}