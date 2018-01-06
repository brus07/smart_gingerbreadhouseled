#ifndef WEB_H
#define WEB_H

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <ESP8266WebServer.h>

#include "Mediator.h"

class WebSite
{
  private:
    ESP8266WebServer server;
    Mediator *m_mediator;
    
  public:
    WebSite(int port): server(port)
    {
    }
    WebSite(int port, Mediator *mediator): server(port), m_mediator(mediator)
    {
    }

    void Begin()
    {
      server.begin();
    }

    void Init()
    {
      internalInit();
    }

    void HandleClient()
    {
      server.handleClient();
    }
    
  private:
    void internalInit()
    {
      server.on("/", [&](){
        handleRoot();
      });
      
      server.on("/demo", [&](){
        handleDemo();
      });

      server.on("/inline", [&](){
          Serial.println("Requested: /inline");
          server.send(200, "text/plain", "this works as well");
      });
      
      server.onNotFound([&](){
          handleNotFound();
      });
    }

    void handleRoot() {
      Serial.println("Requested: /");
      server.send(200, "text/plain", "hello from esp8266!");
    }

    void handleDemo() {
      Serial.println("Requested: /demo");
      server.send(200, "text/plain", "Ok!");
      bool receivedArgs = false;
      if (server.hasArg("color"))
      {
        Serial.println("  color: "+server.arg("color"));
        m_mediator->SetColor(server.arg("color"));
        receivedArgs = true;
      }
      if (server.hasArg("state"))
      {
        Serial.println("  state: "+server.arg("state"));
        m_mediator->ChangeState(server.arg("state"));
        receivedArgs = true;
      }
      
      if (!receivedArgs)
      {
        m_mediator->ChangeState("demo");
      }
    }
    
    void handleNotFound(){
      String message = "File Not Found\n\n";
      message += "URI: ";
      message += server.uri();
      message += "\nMethod: ";
      message += (server.method() == HTTP_GET)?"GET":"POST";
      message += "\nArguments: ";
      message += server.args();
      message += "\n";
      for (uint8_t i=0; i<server.args(); i++){
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      }
      server.send(404, "text/plain", message);
    }
};

#endif WEB_H
