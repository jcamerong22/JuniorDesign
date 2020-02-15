#include "Arduino.h"
#include "FourPinControl.h"


FourPinControl::FourPinControl()
{
    _p1 = _p2 = _p3 = _p4 = 0;
}

FourPinControl::FourPinControl(int p1, int p2, int p3, int p4)
{
    // Set pins to output mode, remember them
    pinMode(p1, OUTPUT);
    pinMode(p2, OUTPUT);
    pinMode(p3, OUTPUT);
    pinMode(p4, OUTPUT);
    _p1 = p1;
    _p2 = p2;
    _p3 = p3;
    _p4 = p4;
}

/* Opening Pins Section*/
// Open a pin
void FourPinControl::pinOpen(int p)
{
    digitalWrite(p, LOW);
}

// Open two pins
void FourPinControl::twoPinsOpen(int p1, int p2)
{
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
}

// Open three pins
void FourPinControl::threePinsOpen(int p1, int p2, int p3)
{
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
    digitalWrite(p3, LOW);
}

// Open all four pins
void FourPinControl::allPinsOpen()
{
    digitalWrite(_p1, LOW);
    digitalWrite(_p2, LOW);
    digitalWrite(_p3, LOW);
    digitalWrite(_p4, LOW);
}

/* Closing Pins Section*/
// Close a pin
void FourPinControl::pinClose(int p)
{
    digitalWrite(p, HIGH);
}

// Close two pins
void FourPinControl::twoPinsClose(int p1, int p2)
{
    digitalWrite(p1, HIGH);
    digitalWrite(p2, HIGH);
}

// Close three pins
void FourPinControl:: threePinsClose(int p1, int p2, int p3) 
{
    digitalWrite(p1, HIGH);
    digitalWrite(p2, HIGH);
    digitalWrite(p3, HIGH);
}

// Close all four pins
void FourPinControl::allPinsClose()
{
    digitalWrite(_p1, HIGH);
    digitalWrite(_p2, HIGH);
    digitalWrite(_p3, HIGH);
    digitalWrite(_p4, HIGH);
}