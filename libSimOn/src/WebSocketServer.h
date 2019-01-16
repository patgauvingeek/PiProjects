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

      // WebSocket is const to prevent the user to use the method send which cannot work before the handshake is completed.
      const Event<WebSocketServer&, const WebSocket&>& onNewConnection() { return mNewConnectionEvent; }
      const Event<WebSocketServer&, WebSocket&>& onHandshakeCompleted() { return mHandshakeCompletedEvent; }
      const Event<WebSocketServer&, WebSocket&, const std::string &>& onCommandReceived() { return mCommandReceivedEvent; }

    private:
      void acceptNewSocket();

      int mListeningSocketFileDescriptor;
      std::vector<WebSocket> mWebSockets;

      Event<WebSocketServer&, const WebSocket&> mNewConnectionEvent;
      Event<WebSocketServer&, WebSocket&> mHandshakeCompletedEvent;
      Event<WebSocketServer&, WebSocket&, const std::string &> mCommandReceivedEvent;

  };
}