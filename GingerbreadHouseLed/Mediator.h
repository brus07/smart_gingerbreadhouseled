#ifndef Mediator_h
#define Mediator_h

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <EEPROM.h>

#include <Fsm.h>

#define TO_DEMO_EVENT 0
#define NEXT_DEMO_EVENT 1
#define TO_SCANNER_LOOP_EVENT 101
#define TO_RAINBOW_LOOP_EVENT 102
#define TO_THEATER_LOOP_EVENT 103
#define TO_COLORWIPE_LOOP_EVENT 104
#define TO_FADE_LOOP_EVENT 105

#define EEPROM_STATE_ADDR 71
#define EEPROM_SPEED_ADDR 72
#define EEPROM_HAS_COLOR_ADDR 75
#define EEPROM_COLOR_R_ADDR 76
#define EEPROM_COLOR_G_ADDR 77
#define EEPROM_COLOR_B_ADDR 78

class Mediator
{
  private:
  Fsm *m_fsm;
  uint32_t m_color = -1;
  int m_speed = 10;

  int restoredState = -1;
  public:
    Mediator(Fsm *fsm): m_fsm(fsm)
    {
    }

    void Begin()
    {
      EEPROM.begin(4096);

      //speed
      int value = EEPROM.read(EEPROM_SPEED_ADDR);
      if (value > 0)
        m_speed = value;
      
      //state
      restoredState = EEPROM.read(EEPROM_STATE_ADDR);
      if (restoredState == TO_DEMO_EVENT)
      {
        SetColor("");
      }
      m_fsm->trigger(restoredState);

      //color
      bool hasColor = EEPROM.read(EEPROM_HAS_COLOR_ADDR);
      if (hasColor)
      {
        int r = EEPROM.read(EEPROM_COLOR_R_ADDR);
        int g = EEPROM.read(EEPROM_COLOR_G_ADDR);
        int b = EEPROM.read(EEPROM_COLOR_B_ADDR);
        m_color = Color(r, g, b);
      }

      Serial.println("Restored:");
      Serial.print("  speed:");
      Serial.println(m_speed);
      Serial.print("  state:");
      Serial.println(restoredState);
      Serial.print("  hascolor:");
      Serial.println(hasColor);
      Serial.print("  color:");
      Serial.println(m_color);
      
    }

    void SetColor(String hex)
    {
      if(hex[0] == 'c')
      {
        uint32_t rgb = (uint32_t) strtol((const char *) &hex[1], NULL, 16);
        m_color = Color(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF),((rgb >> 0) & 0xFF));
      }
      else
      {
        m_color = -1;
      }

      if (HasColor())
      {
        int m_colorR = ((m_color >> 16) & 0xFF);
        int m_colorG = ((m_color >>  8) & 0xFF);
        int m_colorB = ((m_color >>  0) & 0xFF);
        WriteToEeprom(EEPROM_HAS_COLOR_ADDR, true);
        WriteToEeprom(EEPROM_COLOR_R_ADDR, (byte)m_colorR);
        WriteToEeprom(EEPROM_COLOR_G_ADDR, (byte)m_colorG);
        WriteToEeprom(EEPROM_COLOR_B_ADDR, (byte)m_colorB);
      }
      else
      {
        WriteToEeprom(EEPROM_HAS_COLOR_ADDR, false);
      }
    }

    void ChangeSpeed(int speed)
    {
      if (speed <= 0)
        return;
      m_speed = speed;
      WriteToEeprom(EEPROM_SPEED_ADDR, (byte)m_speed);
    }

    void ChangeState(String state)
    {
      if (state == "demo")
      {
        SetColor("");
        m_fsm->trigger(TO_DEMO_EVENT);
        WriteToEeprom(EEPROM_STATE_ADDR, (byte)TO_DEMO_EVENT);
      }
      else if (state == "scanner")
      {
        m_fsm->trigger(TO_SCANNER_LOOP_EVENT);
        WriteToEeprom(EEPROM_STATE_ADDR, (byte)TO_SCANNER_LOOP_EVENT);
      }
      else if (state == "rainbow")
      {
        m_fsm->trigger(TO_RAINBOW_LOOP_EVENT);
        WriteToEeprom(EEPROM_STATE_ADDR, (byte)TO_RAINBOW_LOOP_EVENT);
      }
      else if (state == "theater")
      {
        m_fsm->trigger(TO_THEATER_LOOP_EVENT);
        WriteToEeprom(EEPROM_STATE_ADDR, (byte)TO_THEATER_LOOP_EVENT);
      }
      else if (state == "colorwipe")
      {
        m_fsm->trigger(TO_COLORWIPE_LOOP_EVENT);
        WriteToEeprom(EEPROM_STATE_ADDR, (byte)TO_COLORWIPE_LOOP_EVENT);
      }
      else if (state == "fade")
      {
        m_fsm->trigger(TO_FADE_LOOP_EVENT);
        WriteToEeprom(EEPROM_STATE_ADDR, (byte)TO_FADE_LOOP_EVENT);      
      }
      else
      {
        
      }
    }

    bool HasColor()
    {
      return m_color != -1;
    }
    uint32_t GetColor()
    {
      return m_color;
    }

    int GetSpeed()
    {
      return m_speed;
    }
    
  private:
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
    {
      return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
    }

    static void WriteToEeprom(int adr, byte val)
    {
      EEPROM.write(adr, val);
      EEPROM.commit();
    }
};

#endif
