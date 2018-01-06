#ifndef Mediator_h
#define Mediator_h

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


#include <Fsm.h>

#define TO_DEMO_EVENT 0
#define NEXT_DEMO_EVENT 1
#define TO_SCANNER_LOOP_EVENT 101
#define TO_RAINBOW_LOOP_EVENT 102
#define TO_THEATER_LOOP_EVENT 103
#define TO_COLORWIPE_LOOP_EVENT 104
#define TO_FADE_LOOP_EVENT 105

class Mediator
{
  private:
  Fsm *m_fsm;
  uint32_t m_color = -1;
  int m_speed = 10;
  public:
    Mediator(Fsm *fsm): m_fsm(fsm)
    {
      
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
    }

    void ChangeSpeed(int speed)
    {
      if (speed <= 0)
        return;
      m_speed = speed;
    }

    void ChangeState(String state)
    {
      if (state == "demo")
      {
        SetColor("");
        m_fsm->trigger(TO_DEMO_EVENT);
      }
      else if (state == "scanner")
      {
        m_fsm->trigger(TO_SCANNER_LOOP_EVENT);
      }
      else if (state == "rainbow")
      {
        m_fsm->trigger(TO_RAINBOW_LOOP_EVENT);
      }
      else if (state == "theater")
      {
        m_fsm->trigger(TO_THEATER_LOOP_EVENT);
      }
      else if (state == "colorwipe")
      {
        m_fsm->trigger(TO_COLORWIPE_LOOP_EVENT);
      }
      else if (state == "fade")
      {
        m_fsm->trigger(TO_FADE_LOOP_EVENT);        
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
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
    {
      return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
    }
};

#endif
