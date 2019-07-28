//
// Created by Xx220xX on 19/07/2019.
//
/*** 
*  https://www.freertos.org/a00021.html
*  link for all function used here
*
***/

#include "ThreadEsp32.h"

#define DEBUG_PRINT(x) 
int last_id = 0;

Thread::Thread() {
    this->ready = false;
}


Thread::Thread(void (*function)(void *), const char *name, uint32_t stack_size, uint32_t priority, uint8_t nucleo) {
    this->pFunction = function;
    this->stack_size = stack_size;
    this->nucleo = nucleo;
    this->priority = priority;
    this->ready = true;
    if (name == NULL)
        snprintf(this->name, 14, "%s", "THREAD id: ", last_id);
    snprintf(this->name, 14, "%s", name, last_id);
    snprintf(this->name,17,"%s %d",this->name,last_id);
    last_id++;
    DEBUG_PRINT("Criado uma nova tarefa\n")
    DEBUG_PRINT(this->name)
    DEBUG_PRINT("\n")
}

void Run(void *pThread) {
    ((Thread *) pThread)->run();
}

bool Thread::start(void *params) {

    if (!this->ready || this->started) {
        return false;
    }
    void *pThread = this;
    this->params = params;
    bool pdPass;
	if(this->nucleo == 0 ||this->nucleo == 1){
		pdPass = xTaskCreatePinnedToCore(Run,
                                          this->name,
                                          this->stack_size,
                                          pThread,
                                          this->priority,
                                          &this->handle,
                                          this->nucleo);
	}else{
		pdPass = xTaskCreate(	  Run,
                                          this->name,
                                          this->stack_size,
                                          pThread,
                                          this->priority,
                                          &this->handle);
	}
    delay(1000);// tempo para iniciar e nao dar watchdog error
    this->started = true;
    return pdPass;

}

bool Thread::stop() {
    DEBUG_PRINT("tentativa de finalizar Tarefa: ")
    if (this->started) {
        vTaskDelete(this->handle);
        this->stoped = true;
        this->handle = NULL;
        DEBUG_PRINT("SUCCESS\n")
        return true;
    }
    DEBUG_PRINT("FALHA\n")
    return false;
}

void Thread::sleep(uint32_t ms) {
    ms = ms / portTICK_PERIOD_MS;
    vTaskDelay(ms);
}

uint32_t Thread::getPriority() {
    return uxTaskPriorityGet(this->handle);
}

bool Thread::isRuning() {
    return this->handle != NULL && !paused;
}

Thread Thread::getCurrent() {
    DEBUG_PRINT("Get Current Thread:\n")
    Thread currentThread;
    currentThread.ready = true;
    currentThread.started = true;
    currentThread.priority = uxTaskPriorityGet(NULL);
    snprintf(currentThread.name, 16, "%s", pcTaskGetTaskName(NULL));
    currentThread.nucleo = xPortGetCoreID();
    currentThread.handle = xTaskGetCurrentTaskHandle();
    DEBUG_PRINT("name: ")
    DEBUG_PRINT(currentThread.name)
    DEBUG_PRINT("\nPriority: ")
    DEBUG_PRINT(currentThread.priority)
    DEBUG_PRINT("\nnucleo: ")
    DEBUG_PRINT(currentThread.nucleo)
    DEBUG_PRINT("\n")
    return currentThread;

}

void Thread::pause() {
    DEBUG_PRINT(this->name)
    DEBUG_PRINT(" solicitou uma pausa :")
    if (ready && started && !paused) {
        paused = true;
        vTaskSuspend(handle);
        DEBUG_PRINT("Concedido\n")
        return;
    }
    DEBUG_PRINT("negado\n")
}

void Thread::resume() {
    if (ready && started && paused) {
        paused = false;
        vTaskResume(handle);
    }
}

void Thread::run() {
    this->pFunction(this->params);
    this->stop();
}

const char *Thread::getName() {
    return this->name;
}
