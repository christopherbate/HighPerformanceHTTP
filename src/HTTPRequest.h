#ifndef HTTP_REQ_CTB
#define HTTP_REQ_CTB

#include <string>

class HTTPRequest
{
  public:
    HTTPRequest(std::string &data);
    ~HTTPRequest();
    void ParseHeader( std::string &data );
    std::string GetHeader();
    bool GetKeepAlive();
    std::string GetProtocol();
    std::string GetHost();
    std::string GetMethod();
    std::string GetUrl();
  private:
    bool m_keepAlive;
    std::string m_method;
    std::string m_protocol;
    std::string m_url;
    std::string m_host;
};

#endif