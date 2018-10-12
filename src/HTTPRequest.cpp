#include "HTTPRequest.h"
#include <sstream>
#include <vector>

HTTPRequest::HTTPRequest(std::string &data)
{
    m_keepAlive = false;
    m_method = "";
    m_protocol = "";

    ParseHeader(data);
}

void HTTPRequest::ParseHeader(std::string &data)
{
    // Tokenize the string by new lines first.
    std::stringstream stringStream(data);
    // Loop over all the lines.
    std::string line;
    std::string token;
    uint32_t index = 0;
    while (std::getline(stringStream, line))
    {
        // Loop over all the owrds.
        if (index == 0)
        {
            uint32_t wordIndex = 0;
            std::string token;
            std::stringstream lineStream(line);
            while (std::getline(lineStream, token, ' '))
            {
                switch (wordIndex)
                {
                case 0:
                    m_method = token;
                    break;
                case 1:
                    m_url = token;
                    break;
                case 2:
                    m_protocol = token;
                    break;
                }
                wordIndex++;
            }
            if (wordIndex != 3)
            {
                throw std::runtime_error("Not enough information in header.");
            }
        }
        else
        {
            size_t pos = line.find("Host:");
            if ( pos != std::string::npos)
            {
                m_host = line.substr(pos+5,line.length());
            }
            else
            {
                pos = line.find("Connection:");
                if (pos != std::string::npos )
                {
                    pos = line.find("Keep-alive");
                    m_keepAlive = pos != std::string::npos ? true : false;
                }
            }
        }
        index++;
    }
}

HTTPRequest::~HTTPRequest()
{
}

bool HTTPRequest::GetKeepAlive()
{
    return m_keepAlive;
}

std::string HTTPRequest::GetMethod()
{
    return m_method;
}

std::string HTTPRequest::GetUrl()
{
    return m_url;
}

std::string HTTPRequest::GetHost()
{
    return m_host;
}

std::string HTTPRequest::GetProtocol()
{
    return m_protocol;
}