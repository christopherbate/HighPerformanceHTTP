/*
main.cpp

/author Christopher Bate
*/
#include <iostream>
#include <thread>
#include "TCPSocket.h"
#include "HTTPRequest.h"
#include <csignal>
#include <string>
#include "Session.h"

using namespace std;

TCPSocket server;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " [port num] " << endl;
    }

    if (!server.CreateSocket(argv[1]))
    {
        cerr << "Failed to create socket" << endl;
        return -1;
    }

    if (!server.Listen())
    {
        cerr << "Failed to listen" << endl;
        return -1;
    }

    // Register interrupts.
    std::signal(SIGINT, [](int s) {
        cout << "Received signal " << s << endl;
        server.CloseSocket();
        exit(0);
    });
    std::signal(SIGTERM, [](int s) {
        cout << "Received signal " << s << endl;
        server.CloseSocket();
        exit(0);
    });

    while (1)
    {
        cout << "Listening on " << argv[1] << endl;
        try
        {
            TCPSocket *newSocket = server.Accept();
            if (newSocket == NULL)
            {
                continue;
            }

            cout << "Connection from: " << std::hex << newSocket->GetPeer() << endl;

            // Spawn the thread.
            std::thread session([newSocket]() {
                Session newSession(newSocket, "./www/");
                newSession.Run();
            });
            session.detach();
        }
        catch (std::exception &e)
        {
            cerr << "Exception: " << e.what() << endl;
        }
    }

    return 0;
}