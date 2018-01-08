#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
void ConnectToWiFi();

#include "Mediator.h"

#include "NeoPatterns.h"
void ScannerComplete();
NeoPatterns Ring(12, 3, NEO_GRB + NEO_KHZ800, &ScannerComplete);

#include <Fsm.h>
void onEnterScanner();
void onEnterRainbowCycle();
void onEnterTheaterChase();
void onEnterColorWipe();
void onEnterFade();
void onEnterSocket();
State StateScanner(onEnterScanner, NULL, NULL);
State StateRainbowCycle(onEnterRainbowCycle, NULL, NULL);
State StateTheaterChase(onEnterTheaterChase, NULL, NULL);
State StateColorWipe(onEnterColorWipe, NULL, NULL);
State StateFade(onEnterFade, NULL, NULL);

State StateScannerLoop(onEnterScanner, NULL, NULL);
State StateRainbowCyclerLoop(onEnterRainbowCycle, NULL, NULL);
State StateTheaterChaseLoop(onEnterTheaterChase, NULL, NULL);
State StateColorWipeLoop(onEnterColorWipe, NULL, NULL);
State StateFadeLoop(onEnterFade, NULL, NULL);

State StateSocket(onEnterSocket, NULL, NULL);


Fsm fsm(&StateScanner);

Mediator mediator(&fsm);

#include "Web.h"
WebSite site(80, &mediator);

#include "MyWebSocketsServer.h"
MyWebSockerServer socket(81, &mediator);

void setup()
{
  
    // put your setup code here, to run once:
    Serial.begin(115200);

    ConnectToWiFi();

    site.Init();
    site.Begin();
    Serial.println("HTTP server started");

    
    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();


    socket.Setup();

    Ring.begin();
    Ring.Scanner(Ring.Color(255,0,0), 55);

    fsm.add_transition(&StateScanner, &StateRainbowCycle, NEXT_DEMO_EVENT, NULL);
    fsm.add_transition(&StateRainbowCycle, &StateTheaterChase, NEXT_DEMO_EVENT, NULL);
    fsm.add_transition(&StateTheaterChase, &StateColorWipe, NEXT_DEMO_EVENT, NULL);
    fsm.add_transition(&StateColorWipe, &StateFade, NEXT_DEMO_EVENT, NULL);
    fsm.add_transition(&StateFade, &StateScanner, NEXT_DEMO_EVENT, NULL);
    
    fsm.add_transition(&StateScanner, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCycle, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChase, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipe, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFade, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCyclerLoop, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChaseLoop, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipeLoop, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFadeLoop, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    
    fsm.add_transition(&StateScanner, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCycle, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChase, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipe, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFade, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateScannerLoop, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChaseLoop, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipeLoop, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFadeLoop, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    
    fsm.add_transition(&StateScanner, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCycle, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChase, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipe, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFade, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateScannerLoop, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCyclerLoop, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipeLoop, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFadeLoop, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    
    fsm.add_transition(&StateScanner, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCycle, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChase, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipe, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFade, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateScannerLoop, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCyclerLoop, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChaseLoop, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFadeLoop, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    
    fsm.add_transition(&StateScanner, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCycle, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChase, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipe, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateFade, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateScannerLoop, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateRainbowCyclerLoop, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateTheaterChaseLoop, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateColorWipeLoop, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    
    fsm.add_transition(&StateScannerLoop, &StateScanner, TO_DEMO_EVENT, NULL);
    fsm.add_transition(&StateRainbowCyclerLoop, &StateScanner, TO_DEMO_EVENT, NULL);
    fsm.add_transition(&StateTheaterChaseLoop, &StateScanner, TO_DEMO_EVENT, NULL);
    fsm.add_transition(&StateColorWipeLoop, &StateScanner, TO_DEMO_EVENT, NULL);
    fsm.add_transition(&StateFadeLoop, &StateScanner, TO_DEMO_EVENT, NULL);

    fsm.add_transition(&StateSocket, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateScanner, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateRainbowCycle, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateTheaterChase, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateColorWipe, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateFade, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateScannerLoop, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateRainbowCyclerLoop, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateTheaterChaseLoop, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateColorWipeLoop, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateFadeLoop, &StateSocket, SOCKET_EVENT, NULL);
    fsm.add_transition(&StateSocket, &StateScanner, TO_DEMO_EVENT, NULL);
    fsm.add_transition(&StateSocket, &StateScannerLoop, TO_SCANNER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateSocket, &StateRainbowCyclerLoop, TO_RAINBOW_LOOP_EVENT, NULL);
    fsm.add_transition(&StateSocket, &StateTheaterChaseLoop, TO_THEATER_LOOP_EVENT, NULL);
    fsm.add_transition(&StateSocket, &StateColorWipeLoop, TO_COLORWIPE_LOOP_EVENT, NULL);
    fsm.add_transition(&StateSocket, &StateFadeLoop, TO_FADE_LOOP_EVENT, NULL);
    
    //fsm.add_transition(&StateScanner, &StateFade, NEXT_DEMO_EVENT, NULL);
    //fsm.add_transition(&StateFade, &StateScanner, NEXT_DEMO_EVENT, NULL);

    fsm.run_machine();

    mediator.Begin();
}

void loop()
{
    site.HandleClient();
    
    socket.Loop();

    Ring.Update();

    fsm.run_machine();
}

// Ring Completion Callback
int circleIndex = 0;

void onEnterRainbowCycle()
{
  Serial.println("Started: RainbowCycle");
  circleIndex = 0;
  Ring.OnComplete = &RainbowCycleComplete;
  Ring.RainbowCycle(2*mediator.GetSpeed());
}
void RainbowCycleComplete()
{
  Ring.Interval = 2*mediator.GetSpeed();

  circleIndex++;
  if (circleIndex >= 5)
  {
    fsm.trigger(NEXT_DEMO_EVENT);
  }
}

void onEnterTheaterChase()
{
  Serial.println("Started: TheaterChase");
  circleIndex = 0;
  Ring.OnComplete = &TheaterChaseComplete;
  uint32_t color1 = Ring.Wheel(random(255));
  uint32_t color2 = Ring.Wheel(random(255));
  if (mediator.HasColor())
  {
      color1 = mediator.GetColor();
      color2 = 0;
  }
  Ring.TheaterChase(color1, color2, 10*mediator.GetSpeed());
}
void TheaterChaseComplete()
{
  uint32_t color1 = Ring.Wheel(random(255));
  uint32_t color2 = Ring.Wheel(random(255));  
  if (mediator.HasColor())
  {
      color1 = mediator.GetColor();
      color2 = 0;
  }
  Ring.Color1 = color1;
  Ring.Color2 = color2;
  
  Ring.Interval = 10*mediator.GetSpeed();
  
  circleIndex++;
  if (circleIndex >= 20)
  {
    fsm.trigger(NEXT_DEMO_EVENT);
  }
}

void onEnterColorWipe()
{
  Serial.println("Started: ColorWipe");
  circleIndex = 0;
  Ring.OnComplete = &ColorWipeComplete;
  uint32_t color = Ring.Wheel(random(255));
  if (mediator.HasColor())
    color = mediator.GetColor();
  Ring.ColorWipe(color, 5*mediator.GetSpeed());
}
void ColorWipeComplete()
{
  uint32_t color = Ring.Wheel(random(255));
  if (mediator.HasColor())
  {
    if (Ring.Color1 == mediator.GetColor())
      color = 0;
    else
      color = mediator.GetColor();
  }
  Ring.Color1 = color;
  
  Ring.Interval = 5*mediator.GetSpeed();
  
  circleIndex++;
  if (circleIndex >= 20)
  {
    fsm.trigger(NEXT_DEMO_EVENT);
  }
}

void onEnterScanner()
{
  Serial.println("Started: Scanner");
  circleIndex = 0;
  Ring.OnComplete = &ScannerComplete;
  uint32_t color = Ring.Wheel(random(255));
  if (mediator.HasColor())
    color = mediator.GetColor();
  Ring.Scanner(color, 5*mediator.GetSpeed());
}
void ScannerComplete()
{
  uint32_t color = Ring.Wheel(random(255));
  if (mediator.HasColor())
    color = mediator.GetColor();
  Ring.Color1 = color;
  
  Ring.Interval = 5*mediator.GetSpeed();
  
  circleIndex++;
  if (circleIndex >= 10)
  {
    fsm.trigger(NEXT_DEMO_EVENT);
  }
}

void onEnterFade()
{
  Serial.println("Started: Fade");
  circleIndex = 0;
  Ring.OnComplete = &FadeComplete;
  uint32_t color1 = Ring.Wheel(random(255));
  uint32_t color2 = Ring.Wheel(random(255));
  if (mediator.HasColor())
  {
      color1 = mediator.GetColor();
      color2 = 0;
  }
  Ring.Fade(color1, color2, 50, 5*mediator.GetSpeed());
}
void FadeComplete()
{
  uint32_t color1 = Ring.Color2;
  uint32_t color2 = Ring.Wheel(random(255));
  if (mediator.HasColor())
  {
    if (Ring.Color1 == mediator.GetColor())
    {
      color1 = 0;
      color2 = mediator.GetColor();
    }
    else if(Ring.Color2 == mediator.GetColor())
    {
      color1 = mediator.GetColor();
      color2 = 0;
    }
    else
    {
      color2 = mediator.GetColor();
    }
  }
  Ring.Color1 = color1;
  Ring.Color2 = color2;
  
  Ring.Interval = 5*mediator.GetSpeed();
  
  circleIndex++;
  if (circleIndex >= 10)
  {
    fsm.trigger(NEXT_DEMO_EVENT);
  }
}


void onEnterSocket()
{
  //Serial.println("Started: Socket");
  circleIndex = 0;
  Ring.OnComplete = NULL;
  Ring.ActivePattern = NONE;
  Ring.SetPixel(mediator.GetPixelId(), mediator.GetPixelColor());
}

void ConnectToWiFi()
{
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
    wifiManager.setBreakAfterConfig(true);

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    //wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    if (!wifiManager.autoConnect())
    {
        Serial.println("failed to connect, we should reset as see if it connects");
        delay(3000);
        ESP.reset();
        delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
}


