//
// Created by Xx220xX on 19/07/2019.
// look https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/system/freertos.html

#ifndef SMPLUG_THEADESP32_H
#define SMPLUG_THEADESP32_H


#define THREAD_ESP32_VERSION 1

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef ESP32
#error essa biblioteca foi feita exclusivamente para ESP32
#endif


class Thread {
protected:

public:
    Thread();

    Thread(void (*function)(void *), const char *name = NULL, uint32_t stack_size = 10000, uint32_t priority = 1,
           uint8_t nucleo = 0);

    bool start(void *params = NULL); // start thread, it can't alread started.
    bool stop(); // stop and delete task, you'll can't start again.
    void sleep(uint32_t ms);

    bool isRuning(); // return if thread is current.
    bool paused = false;
    uint32_t getPriority();

    void pause();
    void resume();

    static Thread getCurrent();
    void run();

    const char *getName();

protected:

    void (*pFunction)(void *params);
    void *params;
    char name[17]= {0};
    uint8_t nucleo = 0;
    uint32_t stack_size=0;
    uint32_t priority=0;
    bool ready = false;
    bool started = false;
    bool stoped = false;
    TaskHandle_t handle = NULL;
};


#endif //SMPLUG_THEADESP32_H
