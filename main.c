/*
    File:        main.c
    Description: This is module created to control stepper motor based on ULN2003
    Assumptions: Module is created to work with at least
                 Raspberry Pi module Revision A
    Created:     Tueasday January 24, 2020, 10:19:14
    Author:      Michal Wolowik
    Email:       m.wolowik@emsyslabs.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <termios.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#define LED_RED         21
#define LED_GREEN       25

#define SM_IN1          10
#define SM_IN2          9
#define SM_IN3          11
#define SM_IN4          8

PI_THREAD(thread_led_green)
{
    while(1)
    {
        digitalWrite(LED_GREEN, HIGH);
        delay(500);
        digitalWrite(LED_GREEN, LOW);
        delay(500);
    }

    return 0;
}

PI_THREAD(thread_led_red)
{
    while(1)
    {
        digitalWrite(LED_RED, HIGH);
        delay(2500);
        digitalWrite(LED_RED, LOW);
        delay(2500);
    }

    return 0;
}

void loop(uint8_t step)
{
   switch(step)
   {
   case 0:
       digitalWrite(SM_IN1, HIGH);
       digitalWrite(SM_IN2, LOW);
       digitalWrite(SM_IN3, LOW);
       digitalWrite(SM_IN4, LOW);
   break;
   case 1:
       digitalWrite(SM_IN1, HIGH);
       digitalWrite(SM_IN2, HIGH);
       digitalWrite(SM_IN3, LOW);
       digitalWrite(SM_IN4, LOW);
   break;
   case 2:
       digitalWrite(SM_IN1, LOW);
       digitalWrite(SM_IN2, HIGH);
       digitalWrite(SM_IN3, LOW);
       digitalWrite(SM_IN4, LOW);
   break;
   case 3:
       digitalWrite(SM_IN1, LOW);
       digitalWrite(SM_IN2, HIGH);
       digitalWrite(SM_IN3, HIGH);
       digitalWrite(SM_IN4, LOW);
   break;
   case 4:
       digitalWrite(SM_IN1, LOW);
       digitalWrite(SM_IN2, LOW);
       digitalWrite(SM_IN3, HIGH);
       digitalWrite(SM_IN4, LOW);
   break;
   case 5:
       digitalWrite(SM_IN1, LOW);
       digitalWrite(SM_IN2, LOW);
       digitalWrite(SM_IN3, HIGH);
       digitalWrite(SM_IN4, HIGH);
   break;
     case 6:
         digitalWrite(SM_IN1, LOW);
         digitalWrite(SM_IN2, LOW);
         digitalWrite(SM_IN3, LOW);
         digitalWrite(SM_IN4, HIGH);
   break;
   case 7:
       digitalWrite(SM_IN1, HIGH);
       digitalWrite(SM_IN2, LOW);
       digitalWrite(SM_IN3, LOW);
       digitalWrite(SM_IN4, HIGH);
   break;
   default:
       digitalWrite(SM_IN1, LOW);
       digitalWrite(SM_IN2, LOW);
       digitalWrite(SM_IN3, LOW);
       digitalWrite(SM_IN4, LOW);
   break;
   }
}

int main(int argc, char **argv)
{
    int x;
    uint8_t step = 0;

    struct tm *timeinfo ;
    time_t rawtime ;
    rawtime = time (NULL);
    timeinfo = localtime(&rawtime);

    wiringPiSetupGpio();

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    pinMode(SM_IN1, OUTPUT);
    pinMode(SM_IN2, OUTPUT);
    pinMode(SM_IN3, OUTPUT);
    pinMode(SM_IN4, OUTPUT);

    digitalWrite(SM_IN1, LOW);
    digitalWrite(SM_IN2, LOW);
    digitalWrite(SM_IN3, LOW);
    digitalWrite(SM_IN4, LOW);

    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);

    /* Start modem thread */
    x = piThreadCreate(thread_led_red);

    if(!x)
        printf("Step 1. Led Red thread started\n");

    x = piThreadCreate(thread_led_green);

    if(!x)
        printf("Step 2. Led Green thread started\n");

    while(1){
           loop(step);
           step++;
           if (step==7)
               step = 0;
           delay(1);
    }
}

