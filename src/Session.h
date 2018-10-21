/**
 * Session.cpp
 * Christopher Bate
 */
#ifndef CTB_SESSION_H
#define CTB_SESSION_H

#include <chrono>

#include "TCPSocket.h"
#include "FileManager.h"

class Session
{
  public:
    Session( TCPSocket *connection, const char *root  );
    
    ~Session();

    void Run();

  private:
    TCPSocket *m_connection;    
    FileManager m_fileManager;
};

#endif