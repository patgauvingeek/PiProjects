#include "WebSocket.h"

#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <stdexcept>

#include "Crypto.h"

namespace PiAlarm
{
  class WebSocket::Impl
  {      
    public:
      Impl(int socketFileDescriptor)
        : mIsClosed(false)
        , mSocketFileDescriptor(socketFileDescriptor)
        , mCurrentStateUpdate([&]() { this->processHandshake(); })
      {
        fcntl(socketFileDescriptor, F_SETFL, O_NONBLOCK);
      }

      Impl(const Impl&) = delete; // copy constructor disable
      Impl(Impl&&) = delete;      // move constructor
      
      ~Impl()
      {
        std::cout << "  WebSocket " << mSocketFileDescriptor << " destroyed !" << std::endl;
        close();
      }

      void processHandshake()
      {
        auto wMessageEndIndex = mRawStream.find("\r\n\r\n", 4);
        if (wMessageEndIndex == std::string::npos)
        {
          return;
        }
        static const std::string wKeyLabel = "Sec-WebSocket-Key: ";
        auto wLabelStartIndex = mRawStream.find(wKeyLabel);
        if (wLabelStartIndex == std::string::npos)
        {
          close();
          return;
        }
        auto wKeyStartIndex = wLabelStartIndex + wKeyLabel.size();
        auto wKeyEndIndex = mRawStream.find("\r\n", wKeyStartIndex, 2);
        if (wKeyEndIndex == std::string::npos)
        {
          close();
          return;
        }
        auto wKey = mRawStream.substr(wKeyStartIndex, wKeyEndIndex - wKeyStartIndex);
        static const std::string cWebSocketMagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        auto sha1 = Crypto::sha1(wKey + cWebSocketMagicString);
        auto wHandshake = std::string("HTTP/1.1 101 Switching Protocols\r\n") +
          "Upgrade: websocket\r\n" +
          "Connection: Upgrade\r\n" +
          "Sec-WebSocket-Accept: " + Crypto::Base64::encode(sha1) + "\r\n" +
          "\r\n";
        if (send(mSocketFileDescriptor, wHandshake.c_str(), wHandshake.size(), 0) < 0)
        {
          close();
          return;
        }
        mRawStream = mRawStream.substr(wMessageEndIndex + 4);
        mCurrentStateUpdate = [&]() { this->processHeader(); };
      }

      void processHeader()
      {
        std::cout << "header first pass: " << mSocketFileDescriptor << std::endl;
        mCurrentStateUpdate = []() {  };
      }
      
      void update()
      {
        if (mIsClosed)
        {
          return;
        }
        char wBuffer[1024] = {0};
        int wNumberOfBytes = recv(mSocketFileDescriptor, wBuffer, 1024, 0);
        if (wNumberOfBytes > 0)
        {
          mRawStream += std::string(wBuffer, wNumberOfBytes);
        }
        else if (wNumberOfBytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        {
          close();
          return;
        }
        mCurrentStateUpdate();       
      }

      void close()
      {
        ::close(mSocketFileDescriptor);
        mIsClosed = true;
      }

      bool isClosed()
      {
        return mIsClosed;
      }

	    Impl& operator=(Impl&) = delete;
	    Impl& operator=(Impl&&) = delete;

    private:
      bool mIsClosed;
      int mSocketFileDescriptor;
      std::string mRawStream;
      std::string mContentStream;

      std::function<void ()> mCurrentStateUpdate;

  };

  WebSocket::WebSocket(int socketFileDescriptor)
    : mImplementation(new Impl(socketFileDescriptor))
  {
    std::cout << "WebSocket::WebSocket(int)" << std::endl;
  }

  // move constructor
  WebSocket::WebSocket(WebSocket&& webSocket)
    : mImplementation(webSocket.mImplementation)
  {
    std::cout << "WebSocket::WebSocket(WebSocket&&)" << std::endl;
    webSocket.mImplementation = nullptr;
  }     
  
  WebSocket::~WebSocket()
  {
    std::cout << "WebSocket::~WebSocket()" << std::endl;
    if (mImplementation == nullptr)
    {
      return;
    }
    delete mImplementation;
  }

  void WebSocket::update()
  {
    if (mImplementation == nullptr)
    {
      return;
    }
    mImplementation->update();
  }

  void WebSocket::close()
  {
    if (mImplementation == nullptr)
    {
      return;
    }
    mImplementation->close();
  }

  bool WebSocket::isClosed() const
  {
    if (mImplementation == nullptr)
    {
      return false;
    }
    return mImplementation->isClosed();
  }

  WebSocket& WebSocket::operator=(WebSocket&& webSocket)
  {
    std::cout << "WebSocket::operator=(WebSocket&&)" << std::endl;
    // Self-assignment detection
    if (&webSocket == this)
      return *this;

    if (mImplementation != nullptr)
    {
      // Release any resource we're holding
      delete mImplementation;
    }

    // Transfer ownership of a.m_ptr to m_ptr
    mImplementation = webSocket.mImplementation;
    webSocket.mImplementation = nullptr;

    return *this;
  }

}