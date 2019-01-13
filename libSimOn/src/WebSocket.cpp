#include "WebSocket.h"

#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <stdexcept>

#include "Crypto.h"

namespace SimOn
{
  class WebSocket::Impl
  {
    public:
      Impl(int socketFileDescriptor, const std::string &endpoint)
        : mIsClosed(false)
        , mSocketFileDescriptor(socketFileDescriptor)
        , mEndPoint(endpoint)
        , mRawStream()
        , mNextContentLength(0)
        , mContentStream()
        , mCurrentStateUpdate([&]() { this->processHandshake(); })
      {
        fcntl(socketFileDescriptor, F_SETFL, O_NONBLOCK);
        std::cout << "new socket: " << mEndPoint << std::endl;
      }

      Impl(const Impl&) = delete; // copy constructor disable
      Impl(Impl&&) = delete;      // move constructor
      
      ~Impl()
      {
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
        if (::send(mSocketFileDescriptor, wHandshake.c_str(), wHandshake.size(), 0) < 0)
        {
          close();
          return;
        }
        mRawStream = mRawStream.substr(wMessageEndIndex + 4);
        mCurrentStateUpdate = [&]() { this->processHeader(); };
      }

      void processHeader()
      {
        if (mRawStream.size() < 2)
        {
          return;
        }

        auto wFinRsvOpCode = static_cast<unsigned char>(mRawStream[0]);
        auto wSecondByte = static_cast<unsigned char>(mRawStream[1]);
        mRawStream = mRawStream.substr(2);

        if (wFinRsvOpCode == 136)
        {
          // Disconnection requested by client.
          mCurrentStateUpdate = []() {  };
          close();
          return;
        }

        if((wFinRsvOpCode & 0x80) == 0 || (wFinRsvOpCode & 0x0f) == 0)
        {
          mCurrentStateUpdate = []() {  };
          close();
          throw std::runtime_error("Not supported: fin rsv opcode");  
        }

        // Close connection if unmasked message from client (protocol error)
        if(wSecondByte < 128) {
          std::cout << "message from client not masked" << std::endl;
          close();
          return;
        }

        mNextContentLength = (wSecondByte & 127);

        if (mNextContentLength == 126) 
        {
          // 2 next bytes is the size of content
          mNextContentLength = 0;
          mCurrentStateUpdate = [&]() { this->process2BytesLength(); };
        }
        else if (mNextContentLength == 127)
        {
          // 8 next bytes is the size of content
          mNextContentLength = 0;
          mCurrentStateUpdate = [&]() { this->process8BytesLength(); };
        }
        else
        {
          mCurrentStateUpdate = [&]() { this->processContent(); };
        }
      }

      void process2BytesLength()
      {
        if (mRawStream.size() < 2)
        {
          return;
        }
        mNextContentLength = static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[0])) << 8;
        mNextContentLength += static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[1]));
        mRawStream = mRawStream.substr(2);
        mCurrentStateUpdate = [&]() { this->processContent(); };
      }

      void process8BytesLength()
      {
        if (mRawStream.size() < 8)
        {
          return;
        }
        mNextContentLength = static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[0])) << 56;
        mNextContentLength += static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[1])) << 48;
        mNextContentLength += static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[2])) << 40;
        mNextContentLength += static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[3])) << 32;
        mNextContentLength += static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[4])) << 24;
        mNextContentLength += static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[5])) << 16;
        mNextContentLength += static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[6])) << 8;
        mNextContentLength += static_cast<std::size_t>(static_cast<unsigned char>(mRawStream[7]));
        mRawStream = mRawStream.substr(8);
        mCurrentStateUpdate = [&]() { this->processContent(); };
      }

      void processContent()
      {
        if (mRawStream.size() < mNextContentLength + 4 /* mask size */)
        {
          return;
        }
        auto mContent = mRawStream.substr(0, mNextContentLength + 4 /* mask size */);

        unsigned char wMask[4];
        wMask[0] = static_cast<unsigned char>(mRawStream[0]);
        wMask[1] = static_cast<unsigned char>(mRawStream[1]);
        wMask[2] = static_cast<unsigned char>(mRawStream[2]);
        wMask[3] = static_cast<unsigned char>(mRawStream[3]);
        mRawStream = mRawStream.substr(4);

        for(std::size_t wIndex = 0; wIndex < mNextContentLength; wIndex++)
        {
          mContentStream += static_cast<char>(mRawStream[wIndex] ^ wMask[wIndex % 4]);
        }
        mRawStream = mRawStream.substr(mNextContentLength);
        mCurrentStateUpdate = [&]() { this->processHeader(); };
      }

      void processCommand()
      {
        auto wCommandEndIndex = mContentStream.find("\r\n\r\n", 4);
        if (wCommandEndIndex == std::string::npos)
        {
          return;
        }
        auto wCommand = mContentStream.substr(0, wCommandEndIndex);
        mContentStream = mContentStream.substr(wCommandEndIndex + 4);

        if (wCommand.compare("ping") == 0)
        {
          send("pong");
          return;
        }
        std::cout << "Unknown command \"" << wCommand << "\" sent by " << mSocketFileDescriptor << std::endl;
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
        processCommand();
      }

      void send(const std::string &content)
      {
        std::size_t wLength = content.size();
        std::string wFrame;

        wFrame += static_cast<char>(129 /* one fragment text */);
        
        if (wLength < 126)
        {
          wFrame += static_cast<char>(wLength);
        }
        else if (wLength >= 126 && wLength <= 65535)
        {
          wFrame += static_cast<char>(126);
          wFrame += static_cast<char>((wLength >> 8) && 255);
          wFrame += static_cast<char>(wLength && 255);
        }
        else
        {
          wFrame += static_cast<char>(127);
          wFrame += static_cast<char>((wLength >> 56) && 255);
          wFrame += static_cast<char>((wLength >> 48) && 255);
          wFrame += static_cast<char>((wLength >> 40) && 255);
          wFrame += static_cast<char>((wLength >> 32) && 255);
          wFrame += static_cast<char>((wLength >> 24) && 255);
          wFrame += static_cast<char>((wLength >> 16) && 255);
          wFrame += static_cast<char>((wLength >> 8) && 255);
          wFrame += static_cast<char>(wLength && 255);
        }
        wFrame += content;
        ::send(mSocketFileDescriptor, wFrame.c_str(), wFrame.size(), 0);
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
      std::string mEndPoint;
      std::string mRawStream;
      std::size_t mNextContentLength;
      std::string mContentStream;
      
      std::function<void ()> mCurrentStateUpdate;

  };

  WebSocket::WebSocket(int socketFileDescriptor, const std::string &endpoint)
    : mImplementation(new Impl(socketFileDescriptor, endpoint))
  {
  }

  // move constructor
  WebSocket::WebSocket(WebSocket&& webSocket)
    : mImplementation(webSocket.mImplementation)
  {
    webSocket.mImplementation = nullptr;
  }     
  
  WebSocket::~WebSocket()
  {
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

  void WebSocket::send(const std::string& content)
  {
    if (mImplementation == nullptr)
    {
      return;
    }
    mImplementation->send(content);
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