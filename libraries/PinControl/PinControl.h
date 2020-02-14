#ifndef PinControl_h
#define PinControl_h

#include "Arduino.h"

class Morse
{
  public:
    PinControl(int p1, int p2, int p3, int p4);

    void pinOpen(int p);
    void twoPinOpen(int p1, int p2);
    void threePinOpen(int p1, int p2, int p3);
    void fourPinOpen(int p1, int p2, int p3, int p4);

    void pinClose(int p);
    void twoPinClose(int p1, int p2);
    void threePinClose(int p1, int p2, int p3);
    void fourPinClose(int p1, int p2, int p3, int p4);

  private:
};

#endif
