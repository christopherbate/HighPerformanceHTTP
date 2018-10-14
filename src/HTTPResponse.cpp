#include "HTTPResponse.h"
#include <sstream>
#include <string>

/** Default constructor -- you must then set all fields of the response manually */
HTTPResponse::HTTPResponse()
{
    m_status = 200;
    m_contentLength = 0;
    m_protocol = "HTTP/1.1";
}
/** Constructs an HTTPReponse object based on a request object */
HTTPResponse::HTTPResponse(HTTPRequest &request)
{
    if (request.GetKeepAlive())
    {
        headerValues["Connection"] = "Keep-Alive";
    }
    m_status = 200;
    m_contentLength = 0;
    m_protocol = request.GetProtocol();
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::Send(TCPSocket &socket)
{
}

void HTTPResponse::SetHeaderField(string key, string value)
{
    headerValues[key] = value;
}

std::string HTTPResponse::GetHeader()
{
    // Construct the header.
    std::stringstream ss;

    ss << m_protocol << " " << m_status;

    if(m_status >= 200 && m_status <= 300)
       ss << " Document Follows";
    
    ss << "\r\n";    
    
    for(auto it = headerValues.begin(); it != headerValues.end(); it++){
        ss << it->first <<": "<< it->second <<"\r\n";
    }    

    ss<<"\r\n";

    return ss.str();
}