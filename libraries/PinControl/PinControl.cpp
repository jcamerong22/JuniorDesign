#include "Arduino.h"
#include "PinControl.h"

PinControl::PinControl()
{
    // Don't do anything here
}

void pinOpen(int p)
{
    digitalWrite(p, LOW);
}


void twoPinOpen(int p1, int p2)
{
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
}

void threePinOpen(int p1, int p2, int p3)
{
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
    digitalWrite(p3, LOW);
}

void fourPinOpen(int p1, int p2, int p3, int p4)
{
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
    digitalWrite(p3, LOW);
    digitalWrite(p4, LOW);
}

void fourPinsOpen()
{
      
}

void pinClose(int p)
{
    digitalWrite(p, HIGH);
}

void twoPinClose(int p1, int p2)
{
    digitalWrite(p1, HIGH);
    digitalWrite(p2, HIGH);
}
void threePinClose(int p1, int p2, int p3) 
{
    digitalWrite(p1, HIGH);
    digitalWrite(p2, HIGH);
    digitalWrite(p3, HIGH);
}

void fourPinClose(int p1, int p2, int p3, int p4)
{
    digitalWrite(p1, HIGH);
    digitalWrite(p2, HIGH);
    digitalWrite(p3, HIGH);
    digitalWrite(p4, HIGH);
}