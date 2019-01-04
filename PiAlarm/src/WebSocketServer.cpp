#include "WebSocketServer.h"

#include "Crypto.h"

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <errno.h>
#include <stdexcept>
#include <iostream>

#define PORT 15000 

namespace PiAlarm
{
  WebSocketServer::WebSocketServer()
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
    for(auto connectingWebSocket : mConnectingWebSockets)
    {
      close(connectingWebSocket.first);
    }
    for(auto webSocket : mWebSockets)
    {
      close(webSocket.first);
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
    fcntl(wWebSocketFileDescriptor, F_SETFL, O_NONBLOCK);
    mConnectingWebSockets.emplace(wWebSocketFileDescriptor, std::string());
  }

  void WebSocketServer::acceptNewWebSocket()
  {
    std::vector<int> wRemoveWebSockets;
    for(auto connectingWebSocket : mConnectingWebSockets)
    {
      // web socket connection logic
      char wBuffer[1024] = {0};
      int wNumberOfBytes = recv(connectingWebSocket.first, wBuffer, 1024, 0);
      if (wNumberOfBytes > 0)
      {
        std::cout << "handshake chunk size: " << wNumberOfBytes << std::endl;
        std::cout << "handshake chunk : " << std::string(wBuffer, wNumberOfBytes) << std::endl;
        connectingWebSocket.second += std::string(wBuffer, wNumberOfBytes);
      }
      else if (wNumberOfBytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
      {
        close(connectingWebSocket.first);
        wRemoveWebSockets.push_back(connectingWebSocket.first);
        continue;
      }
      auto wMessageEndIndex = connectingWebSocket.second.find("\r\n\r\n", 4);
      if (wMessageEndIndex == std::string::npos)
      {
        continue;
      }
      static const std::string wKeyLabel = "Sec-WebSocket-Key: ";
      auto wLabelStartIndex = connectingWebSocket.second.find(wKeyLabel);
      if (wLabelStartIndex == std::string::npos)
      {
        close(connectingWebSocket.first);
        wRemoveWebSockets.push_back(connectingWebSocket.first);
        continue;
      }
      auto wKeyStartIndex = wLabelStartIndex + wKeyLabel.size();
      auto wKeyEndIndex = connectingWebSocket.second.find("\r\n", wKeyStartIndex, 2);
      if (wKeyEndIndex == std::string::npos)
      {
        close(connectingWebSocket.first);
        wRemoveWebSockets.push_back(connectingWebSocket.first);
        continue;
      }
      auto wKey = connectingWebSocket.second.substr(wKeyStartIndex, wKeyEndIndex - wKeyStartIndex);
      static const std::string cWebSocketMagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
      auto sha1 = Crypto::sha1(wKey + cWebSocketMagicString);
      auto wHandshake = std::string("HTTP/1.1 101 Switching Protocols\r\n") +
        "Upgrade: websocket\r\n" +
        "Connection: Upgrade\r\n" +
        "Sec-WebSocket-Accept: " + Crypto::Base64::encode(sha1) + "\r\n" +
        "\r\n";
      auto wBadRequest = std::string("HTTP/1.1 400 Bad Request\r\n") +
        "\r\n";
      if (send(connectingWebSocket.first, wHandshake.c_str(), wHandshake.size(), 0) < 0)
      {
        close(connectingWebSocket.first);
        wRemoveWebSockets.push_back(connectingWebSocket.first);
        continue;
      }
      std::cout << "transfer : " << connectingWebSocket.second.substr(wMessageEndIndex + 4) << std::endl;
      mWebSockets.emplace(connectingWebSocket.first, connectingWebSocket.second.substr(wMessageEndIndex + 4));
      wRemoveWebSockets.push_back(connectingWebSocket.first);
    }
    for (int removeWebSocket : wRemoveWebSockets)
    {
      mConnectingWebSockets.erase(removeWebSocket);
    }
  }

  void WebSocketServer::readCommands()
  {
    std::vector<int> wRemoveWebSockets;
    for(auto webSocket : mWebSockets)
    { 
      char wBuffer[1024] = {0};
      int wNumberOfBytes = recv(webSocket.first, wBuffer, 1024, 0);
      if (wNumberOfBytes > 0)
      {
        std::cout << "command chunk size: " << wNumberOfBytes << std::endl;
        std::cout << "command chunk : " << std::string(wBuffer, wNumberOfBytes) << std::endl;
        webSocket.second += std::string(wBuffer, wNumberOfBytes);
      }
      else if (wNumberOfBytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
      {
        std::cout << "error disconnect web socket" << std::endl;
        close(webSocket.first);
        wRemoveWebSockets.push_back(webSocket.first);
        continue;
      }
      auto wCommandEndIndex = webSocket.second.find("\r\n\r\n", 4);
      if (wCommandEndIndex == std::string::npos)
      {
        continue;
      }
      // since we don't accept commands yet, we just output the command.
      std::cout << webSocket.second.substr(0, wCommandEndIndex) << std::endl;

      std::cout << "transfer : " << webSocket.second.substr(wCommandEndIndex + 4) << std::endl;
      webSocket.second = webSocket.second.substr(wCommandEndIndex + 4);
    }
    for (int removeWebSocket : wRemoveWebSockets)
    {
      mWebSockets.erase(removeWebSocket);
    }
  }

  void WebSocketServer::sendUpdates()
  {
    // later...
  }

  void WebSocketServer::update()
  {
    acceptNewSocket();
    acceptNewWebSocket();
    readCommands();
    sendUpdates();
  }
}