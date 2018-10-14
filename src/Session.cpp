#include "Session.h"
#include <chrono>
#include "HTTPRequest.h"
#include "HTTPResponse.h"

using namespace std;

Session::Session(TCPSocket *connection) : m_connection(connection)
{
}

Session::~Session()
{
    // Close connection
    if(m_connection)
        delete m_connection;
}

void Session::Run()
{
    bool alive = true;

    m_connection->SetRecvTimeout(1000);

    auto startTimer = chrono::system_clock::now();

    while (alive)
    {
        // Receive data
        char data[1024];
        uint32_t length = 0;

        while(length == 0){
            length = m_connection->BlockingRecv(data, 1024);
            if(m_connection->DidTimeout()){
                auto timeCheck = chrono::system_clock::now();
                if( chrono::duration_cast<chrono::seconds>(timeCheck-startTimer).count() > 10){
                    cout <<"Keep alive expired."<<endl;
                    break;
                }
            }
        }

        // Display tthe header information (for fun)
        cout << "Received data of length: " << std::dec<<length << endl;
        string dataString = string(data, length);
        HTTPRequest request(dataString);

        cout << "Header:" << endl;
        cout << request.GetMethod() << endl;
        cout << request.GetUrl() << endl;
        cout << request.GetProtocol() << endl;
        cout << request.GetHost() << endl;
        cout << "Keepalive:" << request.GetKeepAlive() << endl;

        if(request.GetKeepAlive()){
            cout << "Sstarting keep alive timer"<<endl;
            startTimer = chrono::system_clock::now();
        } else {
            break;
        }

        // Send the correct response.
        HTTPResponse response;
    }

    m_connection->CloseSocket();                
}