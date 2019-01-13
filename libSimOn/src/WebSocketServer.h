#pragma once

#include <map>
#include <string>
#include <vector>

#include "Event.h"
#include "WebSocket.h"

namespace SimOn
{
  class WebSocketServer
  {      
    public:
      WebSocketServer();
      ~WebSocketServer();
      void update();
      void sendAll(const std::string &content);

      Event<WebSocketServer&, WebSocket&>& onNewConnection() { return mOnNewConnectionEvent; }

    private:
      void acceptNewSocket();

      int mListeningSocketFileDescriptor;
      std::vector<WebSocket> mWebSockets;

      Event<WebSocketServer&, WebSocket&> mOnNewConnectionEvent;

  };
}