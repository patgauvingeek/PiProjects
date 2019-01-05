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

    private:
      void acceptNewSocket();

      int mListeningSocketFileDescriptor;
      std::vector<WebSocket> mWebSockets;

  };
}