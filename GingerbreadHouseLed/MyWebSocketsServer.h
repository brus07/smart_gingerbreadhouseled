#ifndef MyWebSocketsServer_h
#define MyWebSocketsServer_h

//https://github.com/Links2004/arduinoWebSockets
#include <WebSocketsServer.h>

#include "Mediator.h"
    
class MyWebSockerServer
{
  private:
    WebSocketsServer webSocket;
    Mediator *m_mediator;
  public:
    MyWebSockerServer(int port, Mediator *mediator): webSocket(port), m_mediator(mediator)
    {
      
    }

    void Setup()
    {
      webSocket.begin();
      //webSocket.onEvent(webSocketEvent);
      webSocket.onEvent(std::bind(&MyWebSockerServer::webSocketHandle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    }

    void Loop()
    {
      webSocket.loop();
    }
    
    void webSocketHandle(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
    {
      switch(type) {
          case WStype_DISCONNECTED:
              Serial.printf("[%u] Disconnected!\n", num);
              break;
          case WStype_CONNECTED:
              {
                  IPAddress ip = webSocket.remoteIP(num);
                  Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
          
                  // send message to client
                  webSocket.sendTXT(num, "Connected");
                  webSocket.sendTXT(num, "Connected2");
              }
              break;
          case WStype_TEXT:
              Serial.printf("[%u] get Text: %s\n", num, payload);
              if(payload[0] == 'i') {
                  uint32_t pixelId = (uint32_t) strtol((const char *) &payload[1], NULL, 10);
                  
                  // we get RGB data
                  // decode rgb data
                  int colorIndex = 4;
                  if (pixelId >= 10)
                    colorIndex++;
                  uint32_t rgb = (uint32_t) strtol((const char *) &payload[colorIndex], NULL, 16);
                  // NeoPixels
                  ReceivedRgbAndIndex(pixelId, ((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF),((rgb >> 0) & 0xFF));
              }
              // send message to client
              // webSocket.sendTXT(num, "message here");
  
              // send data to all connected clients
              // webSocket.broadcastTXT("message here");
              break;
          case WStype_BIN:
              Serial.printf("[%u] get binary length: %u\n", num, length);
              hexdump(payload, length);
  
              // send message to client
              // webSocket.sendBIN(num, payload, length);
              break;
      }
    }

  private:
    void ReceivedRgbAndIndex(int index, int r, int g, int b)
    {
      m_mediator->SetPixel(index, r, g, b);
    }
    void ReceivedRgb(int r, int g, int b)
    {
      for (int i=0; i<12; i++)
      {
        ReceivedRgbAndIndex(r,g,b,i);
      }
    }
  
};


#endif /* MyWebSocketsServer_h */
