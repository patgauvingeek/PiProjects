#pragma once

#include <functional>
#include <memory>
#include <string>

namespace SimOn
{
  class WebSocket
  {      
    public:
      WebSocket(int socketFileDescriptor, const std::string &endpoint);
      WebSocket(const WebSocket&) = delete; // copy constructor disable
      WebSocket(WebSocket&& webSocket);     // move constructor
      ~WebSocket();
      
      void update();
      void send(const std::string& content);
      void close();
      bool isClosed() const;

	    WebSocket& operator=(WebSocket& webSocket) = delete;
      // Move assignment
	    WebSocket& operator=(WebSocket&& webSocket);
    
    private:
      class Impl;
      Impl* mImplementation;

  };
}