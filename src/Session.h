/**
 * Session.cpp
 * Christopher Bate
 */
#ifndef CTB_SESSION_H
#define CTB_SESSION_H

#include <chrono>

#include "TCPSocket.h"

class Session
{
  public:
    Session( TCPSocket *connection );
    ~Session();

    void Run();

  private:
    TCPSocket *m_connection;    
};

#endif