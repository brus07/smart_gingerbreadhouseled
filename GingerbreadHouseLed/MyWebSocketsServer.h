#ifndef MyWebSocketsServer_h
#define MyWebSocketsServer_h

//https://github.com/Links2004/arduinoWebSockets
#include <WebSocketsServer.h>
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

void MyWebSocketSetup()
{
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void MyWebSocketLoop()
{
    webSocket.loop();
}

void ReceivedRgbAndIndex(int r, int g, int b, int index)
{
  
}
void ReceivedRgb(int r, int g, int b)
{
  for (int i=0; i<12; i++)
  {
    ReceivedRgbAndIndex(r,g,b,i);
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

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
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            if(payload[0] == '#') {
                // we get RGB data
                // decode rgb data
                uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
/*
                // NeoPixels
                for (int i=0; i<PixelCount; i++) {
                  strip.SetPixelColor(i, RgbColor(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF),((rgb >> 0) & 0xFF) ));
                }
                strip.Show();
*/
              ReceivedRgb(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF),((rgb >> 0) & 0xFF) );
            }
            if(payload[0] == 'c') {
                uint32_t pixelId = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
                // we get RGB data
                // decode rgb data
                uint32_t rgb = (uint32_t) strtol((const char *) &payload[4], NULL, 16);
                Serial.println(rgb);
                // NeoPixels
                Serial.println(pixelId);
                
                ReceivedRgbAndIndex(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF),((rgb >> 0) & 0xFF), pixelId);
/*
                strip.SetPixelColor(pixelId, RgbColor(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF),((rgb >> 0) & 0xFF) ));
                strip.Show();
*/
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

#endif MyWebSocketsServer_h
