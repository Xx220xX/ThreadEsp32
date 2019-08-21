
#include<ThreadEsp32.h>
//For this example a listener will be created for serial communication

void serialListener(void *params);
void onReceveMsg(String msg);

void setup() {
    Serial.begin(115200);
    Thread("Serial Listener",serialListener).start();// start listener

}
void loop() {
    // i do other thing
    Serial.println("__LOOP: -> i do other thing");
    delay(5000);
}


void onReceveMsg(String msg) {
    Serial.println("Have msg to you: "+msg);
}
void serialListener(void *params){
    for(;;){
        if(Serial.available()){
            String c ="";
            while(Serial.available()){
                c = c + String((char)Serial.read());
            }
            onReceveMsg(c);
        }
        delay(10);
    }
}