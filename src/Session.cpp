#include "Session.h"
#include <chrono>
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include <cassert>

using namespace std;

Session::Session(TCPSocket *connection, const char *root) : m_connection(connection), m_fileManager(root)
{
}

Session::~Session()
{
    // Close connection
    if (m_connection)
        delete m_connection;
}

void Session::Run()
{
    if (!m_connection)
    {
        throw std::runtime_error("Connection socket is not set.");
    }
    const unsigned timeout = 10000000;
    m_connection->SetRecvTimeout(timeout);
    auto startTimer = chrono::system_clock::now();
    // Receive data
    char data[2048];
    uint32_t length = 0;

    cout << "Running session." << endl;

    while (1)
    {
        //assert(m_connection);
        //assert(!m_connection->IsBad());

        do
        {
            length = m_connection->BlockingRecv(data, 2048);            

            if (m_connection->DidTimeout())
            {
                auto timeCheck = chrono::system_clock::now();
                if (chrono::duration_cast<chrono::microseconds>(timeCheck - startTimer).count() > timeout)
                {
                    cout << "Keep alive expired" << endl;
                    cout << "Ending session" << endl;
                    m_connection->CloseSocket();
                    return;
                }
            }
            else if (length == 0)
            {
                cout << "Peer closed connection." << endl;
                m_connection->CloseSocket();
                return;
            }

        } while (length == 0);

        // Display tthe header information (for fun)
        cout << "Received request of length: " << std::dec << length << endl;
        string dataString = string(data, length);
        HTTPRequest request(dataString);
        cout << request.GetUrl() << endl;

        // Print header information.
        /*
        cout << "Header:" << endl;
        cout << request.GetMethod() << endl;
        cout << request.GetUrl() << endl;
        cout << request.GetProtocol();
        cout << request.GetHost() << endl;
        cout << "Keepalive:" << request.GetKeepAlive() << endl;
        */

        // Create response.
        HTTPResponse response(request);

        // If the url is present, retrieve the corresponding file.
        std::string url = request.GetUrl();
        std::string filename;
        uint64_t size = 0;
        if (url != "")
        {
            try
            {
                filename = m_fileManager.GetFilename(url, size);

                response.SetHeaderField("Content-Length", to_string(size));
                response.SetHeaderField("Content-Type", m_fileManager.GetContentType(filename));
            }
            catch (std::runtime_error &e)
            {
                cerr << "Exception in URL processing. " << e.what() << endl;                
                response.Status(500);
                response.SetHeaderField("Content-Length", to_string(0));
                size = 0;
            }
        }

        // Display response.
        std::string respHeader = response.GetHeader();

        m_connection->BlockingSend(respHeader.c_str(), respHeader.length());

        // Send the file.
        char buffer[1024];
        if (size > 0)
        {
            ifstream infile(filename);
            while (!infile.eof())
            {
                infile.read(buffer, 1024);
                m_connection->BlockingSend(buffer, infile.gcount());
            }
            infile.close();
        }

        if (request.GetKeepAlive())
        {
            cout << "Starting keep alive timer" << endl;
            startTimer = chrono::system_clock::now();
        }
        else {
            break;
        }
    }

    cout << "Ending session" << endl;

    m_connection->CloseSocket();
}