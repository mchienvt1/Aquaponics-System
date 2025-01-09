// Import required libraries

#include "global.h"
#include "tasks/dispatch_Task.h"

extern "C" void app_main(void)
{
  setup();
  while (true) {
    loop();
  }
}

void setup(){
  Serial.begin(BAUDRATE); 
  dispatch_task_init();
}

void loop(){
  // Nothing to do here, FreeRTOS tasks handle the work
}