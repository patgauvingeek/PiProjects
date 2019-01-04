#pragma once

#include <map>
#include <string>
#include <vector>

namespace PiAlarm
{
  class WebSocketServer
  {      
    public:
      WebSocketServer();
      ~WebSocketServer();
      void update();
    private:
      void acceptNewWebSocket();
      void readCommands();
      void sendUpdates();

      int mListeningSocketFileDescriptor;
      std::map<int, std::string> mConnectingWebSockets;
      std::vector<int> mWebSocketFileDescriptors;

  };
}