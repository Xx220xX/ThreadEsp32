//
// Created by Xx220xX on 19/07/2019.
//
/*** 
*  https://www.freertos.org/a00021.html
*  link for all function used here
*
***/

#include "ThreadEsp32.h"


#ifndef THREAD_ESP32_DEBUG
#error
#define THREAD_DEBUG_PRINT(x)
#define THREAD_DEBUG_PRINTLN(x)
#else
#define THREAD_DEBUG_PRINT(x) Serial.print(x);
#define THREAD_DEBUG_PRINTLN(x) Serial.println(x);
#endif

int last_id = 0;

Thread::Thread( ) {
	THREAD_DEBUG_PRINTLN("criando tarefa vazia agr")
    this->ready = false;
}

Thread::Thread(const char *name,void (*function)(void *), uint32_t stack_size, uint32_t priority, uint8_t nucleo) {
	THREAD_DEBUG_PRINTLN("criando tarefa agr")
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
    
   THREAD_DEBUG_PRINT("Criado uma nova tarefa: ")
   THREAD_DEBUG_PRINTLN(this->name)

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

void Thread::stop() {
   THREAD_DEBUG_PRINTLN("tentativa de finalizar Tarefa: ")
    if (this->started) {
        	this->stoped = true;
        	vTaskDelete(this->handle);
        	this->handle= 0;      
       		THREAD_DEBUG_PRINTLN("SUCCESS\n")
        	return ;
    }
   THREAD_DEBUG_PRINTLN("FALHA\n")

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
   THREAD_DEBUG_PRINTLN("Get Current Thread:")
    Thread currentThread;
    currentThread.ready = true;
    currentThread.started = true;
    currentThread.priority = uxTaskPriorityGet(NULL);
    snprintf(currentThread.name, 16, "%s", pcTaskGetTaskName(NULL));
    currentThread.nucleo = xPortGetCoreID();
    currentThread.handle = xTaskGetCurrentTaskHandle();
   THREAD_DEBUG_PRINT("name: ")
   THREAD_DEBUG_PRINTLN(currentThread.name)
   THREAD_DEBUG_PRINT("Priority: ")
   THREAD_DEBUG_PRINTLN(currentThread.priority)
   THREAD_DEBUG_PRINT("nucleo: ")
   THREAD_DEBUG_PRINTLN(currentThread.nucleo)
    
    return currentThread;

}

void Thread::pause() {
   THREAD_DEBUG_PRINT(this->name)
   THREAD_DEBUG_PRINTLN(": solicitou uma pausa :")
    if (ready && started && !paused) {
        paused = true;
        vTaskSuspend(handle);
       THREAD_DEBUG_PRINTLN("Concedido\n")
        return;
    }
   THREAD_DEBUG_PRINTLN("negado\n")
}

void Thread::resume() {
    if (ready && started && paused) {
        paused = false;
        vTaskResume(handle);
    }
}

void Thread::run() {
    this->pFunction(this->params);
    THREAD_DEBUG_PRINTLN("executou com sucesso agr vai parar :D")
	this->stoped = true;
	this->handle= NULL;
    vTaskDelete(NULL);
    
    
}

const char *Thread::getName() {
    return this->name;
}

#undef THREAD_DEBUG_PRINT(x)
#undef THREAD_DEBUG_PRINTLN(x)
