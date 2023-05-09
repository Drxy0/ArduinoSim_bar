#include<arduinoPlatform.h>
#include<tasks.h>
#include<interrupts.h>
#include<stdio.h>
#include<serial.h>
#include <pwm.h>
#include<data.h>

extern serial Serial;
int a,b,c,d,i=0;

void setup()
{
    pinMode(26,OUTPUT);
    pinMode(27,OUTPUT);
    pinMode(28,OUTPUT);
    pinMode(29,OUTPUT);

    int sensorValue = analogRead(A0);

}

void loop()
{
    a = i & 0b0001;
    b = i & 0b0010;
    c = i & 0b0100;
    d = i & 0b1000;
    i = (i+1)%16;
    digitalWrite(26,a);
    digitalWrite(27,b);
    digitalWrite(28,c);
    digitalWrite(29,d);
    delay(500);
}
