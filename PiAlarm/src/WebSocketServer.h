#pragma once

#include <map>
#include <string>
#include <vector>

#include "WebSocket.h"

namespace PiAlarm
{
  class WebSocketServer
  {      
    public:
      WebSocketServer();
      ~WebSocketServer();
      void update();
      void sendAll(const std::string &content);

    private:
      void acceptNewSocket();

      int mListeningSocketFileDescriptor;
      std::vector<WebSocket> mWebSockets;

  };
}