#include "WebSocketServer.h"

#include "Crypto.h"

#include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <errno.h>
#include <stdexcept>
#include <iostream>

#define PORT 15000 

namespace SimOn
{
  WebSocketServer::WebSocketServer()
    : mListeningSocketFileDescriptor(0)
    , mWebSockets()
    , mNewConnectionEvent()
    , mHandshakeCompletedEvent()
    , mCommandReceivedEvent()
  {       
    // Creating socket file descriptor 
    if ((mListeningSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
      throw std::runtime_error("error: listening socket creation failed");
    }
    fcntl(mListeningSocketFileDescriptor, F_SETFL, O_NONBLOCK);
       
    // Forcefully attaching socket to the port 15000 
    int opt = 1; // ??
    if (setsockopt(mListeningSocketFileDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }

    struct sockaddr_in wListeningAddress;
    wListeningAddress.sin_family = AF_INET; 
    wListeningAddress.sin_addr.s_addr = INADDR_ANY; 
    wListeningAddress.sin_port = htons(PORT); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(mListeningSocketFileDescriptor, (struct sockaddr *)&wListeningAddress, sizeof(wListeningAddress)) < 0) 
    {
      throw std::runtime_error("error: bind failed on listening socket");
    }
    if (listen(mListeningSocketFileDescriptor, 5) < 0) 
    {
      throw std::runtime_error("error: listen failed on listening socket"); 
    }
  }
  
  WebSocketServer::~WebSocketServer()
  {
    close(mListeningSocketFileDescriptor);
    for(auto &webSocket : mWebSockets)
    {
      webSocket.close();
    }
  }

  void WebSocketServer::acceptNewSocket()
  {
    struct sockaddr_in wClientAddress;
    int wClientAddressSize = sizeof(wClientAddress);
    int wWebSocketFileDescriptor = accept(mListeningSocketFileDescriptor, (struct sockaddr *)&wClientAddress, (socklen_t*)&wClientAddressSize);
    if (wWebSocketFileDescriptor < 0)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK ) 
      {
        return;
      }
      throw std::runtime_error("error: accept failed on listening socket"); 
    }
    std::stringstream wEndPoint;
    wEndPoint << ::inet_ntoa(wClientAddress.sin_addr) << ":" << wClientAddress.sin_port;
    mWebSockets.emplace_back(wWebSocketFileDescriptor, wEndPoint.str());
    mWebSockets.back().onHandshakeCompleted() += [&] (WebSocket &webSocket)
      {
        mHandshakeCompletedEvent.raise(*this, webSocket);
      };
    mWebSockets.back().onCommandReceived() += [&] (WebSocket &webSocket, const std::string & command)
      {
        mCommandReceivedEvent.raise(*this, webSocket, command);
      };
    mNewConnectionEvent.raise(*this, mWebSockets.back());
  }

  void WebSocketServer::update()
  {
    acceptNewSocket();
    for(auto &webSocket : mWebSockets)
    {
      webSocket.update();
    }
    auto wWebSocketsToRemove = std::remove_if(mWebSockets.begin(), mWebSockets.end(),
      [](const WebSocket& webSocket) 
      { 
        return webSocket.isClosed();
      });
    mWebSockets.erase(wWebSocketsToRemove, mWebSockets.end());
  }

  void WebSocketServer::sendAll(const std::string &content)
  {
    for(auto &webSocket : mWebSockets)
    {
      webSocket.send(content);
    }
  }

}