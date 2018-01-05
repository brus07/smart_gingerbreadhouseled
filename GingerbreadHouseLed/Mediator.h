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
  public:
    Mediator(Fsm *fsm): m_fsm(fsm)
    {
      
    }

    void ChangeState(String state)
    {
      if (state == "demo")
      {
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
};

#endif
