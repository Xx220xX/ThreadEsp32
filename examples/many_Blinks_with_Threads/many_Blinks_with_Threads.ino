
#include<ThreadEsp32.h>
//For this example 5 Leds will blink with delays at different times

// pins
#define LED_1 13
#define LED_2 12
#define LED_3 14
#define LED_4 27
#define LED_5 26

typedef struct {
    int pinLed;
    int timeForDelay;
} Led;

void piscaLed(void *param);

void setup() {
    Serial.begin(115200);
    Led l1 = {LED_1, 50};
    Led l2 = {LED_2, 100};
    Led l3 = {LED_3, 500};
    Led l4 = {LED_4, 300};
    Led l5 = {LED_5, 700};
    Thread("LED 1: ",piscaLed).start(&l1);
    Thread("LED 2: ",piscaLed ).start(&l2);
    Thread("LED 3: ",piscaLed ).start(&l3);
    Thread("LED 4: ",piscaLed ).start(&l4);
    Thread("LED 5: ",piscaLed ).start(&l5);

    Thread::getCurrent().stop(); // destroy thread loop

}
void loop() {
    // i am useless.
}


void piscaLed(void *param) {
    Led led = *(Led *)param;// convert param
    pinMode(led.pinLed, OUTPUT);
    for (;;) {
        digitalWrite(led.pinLed, !digitalRead(led.pinLed));
        delay(led.timeForDelay);
    }
}