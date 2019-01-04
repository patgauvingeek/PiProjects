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
    for(int connectingWebSocket : mWebSocketFileDescriptors)
    {
      close(connectingWebSocket);
    }
  }

  void WebSocketServer::acceptNewWebSocket()
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

  static bool endsWith(const std::string& str, const std::string& suffix)
  {
    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
  }

  void WebSocketServer::readCommands()
  {
    for(auto connectingWebSocket : mConnectingWebSockets)
    {
      // web socket connection logic
      char buffer[1024] = {0};
      int wNumberOfBytes = recv(connectingWebSocket.first, buffer, 1024, 0);
      if (wNumberOfBytes == 0)
      {
        continue;
      }
      if (wNumberOfBytes < 0)
      {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
          continue;
        }
        close(connectingWebSocket.first); //+ remove from vector
      }
      connectingWebSocket.second += buffer;
      if (!endsWith(connectingWebSocket.second, "\r\n\r\n"))
      {
        continue;
      }
      static const std::string wKeyLabel = "Sec-WebSocket-Key: ";
      auto wLabelStartIndex = connectingWebSocket.second.find(wKeyLabel);
      if (wLabelStartIndex == std::string::npos)
      {
        close(connectingWebSocket.first);
        // todo remove
      }
      auto wKeyStartIndex = wLabelStartIndex + wKeyLabel.size();
      auto wKeyEndIndex = connectingWebSocket.second.find("\r", wKeyStartIndex);
      if (wKeyEndIndex == std::string::npos)
      {
        close(connectingWebSocket.first);
        // todo remove
      }
      auto wKey = connectingWebSocket.second.substr(wKeyStartIndex, wKeyEndIndex - wKeyStartIndex);
      static const std::string cWebSocketMagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
      auto sha1 = Crypto::sha1(wKey + cWebSocketMagicString);    
      auto wHandshake = std::string("HTTP/1.1 101 Web Socket Protocol Handshake\r\n") +
        "Upgrade: websocket\r\n" +
        "Connection: Upgrade\r\n" +
        "Sec-WebSocket-Accept: " + Crypto::Base64::encode(sha1) + "\r\n" +
        "\r\n";
      if (send(connectingWebSocket.first, wHandshake.c_str(), wHandshake.size(), 0) < 0)
      {
        close(connectingWebSocket.first);
        // todo remove
      }
    }
    for(int connectingWebSocket : mWebSocketFileDescriptors)
    {
      // listen for command

    }
    // valread = read( new_socket , buffer, 1024); 
    // printf("%s\n",buffer ); 
    // send(new_socket , hello , strlen(hello) , 0 ); 
    // printf("Hello message sent\n"); 
    // return 0; 
  }

  void WebSocketServer::sendUpdates()
  {
    // later...
  }

  void WebSocketServer::update()
  {
    acceptNewWebSocket();
    readCommands();
    sendUpdates();
  }
}