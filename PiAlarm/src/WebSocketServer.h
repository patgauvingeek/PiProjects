#pragma once

#include <map>
#include <string>

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
      void acceptNewWebSocket();
      void readCommands();
      void sendUpdates();

      int mListeningSocketFileDescriptor;
      std::map<int, std::string> mConnectingWebSockets;
      std::map<int, std::string> mWebSockets;

  };
}