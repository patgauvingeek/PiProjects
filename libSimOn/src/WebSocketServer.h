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

      const Event<WebSocketServer&, WebSocket&>& onNewConnection() { return mOnNewConnectionEvent; }
      const Event<WebSocketServer&, WebSocket&, const std::string &>& onCommandReceived() { return mOnCommandReceivedEvent; }

    private:
      void acceptNewSocket();

      int mListeningSocketFileDescriptor;
      std::vector<WebSocket> mWebSockets;

      Event<WebSocketServer&, WebSocket&> mOnNewConnectionEvent;
      Event<WebSocketServer&, WebSocket&, const std::string &> mOnCommandReceivedEvent;

  };
}