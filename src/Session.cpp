#include "Session.h"

Session::Session(TCPSocket &connection)
{
}

Session::~Session()
{
    // Close connection
}

void Session::Run()
{
    bool alive = true;

    while (alive)
    {
        // Receive data

        //process header

        // reset timer

        // Send response
    }
}