#ifndef FourPinControl_h
#define FourPinControl_h

#include "Arduino.h"

class FourPinControl
{
  public:
    FourPinControl();
    FourPinControl(int p1, int p2, int p3, int p4);

    void pinOpen(int p);
    void twoPinsOpen(int p1, int p2);
    void threePinsOpen(int p1, int p2, int p3);
    void allPinsOpen();

    void pinClose(int p);
    void twoPinsClose(int p1, int p2);
    void threePinsClose(int p1, int p2, int p3);
    void allPinsClose();

  private:
    int _p1, _p2, _p3, _p4;
};

#endif
