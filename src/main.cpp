// Import required libraries
#include "global.h"
#include "tasks/dispatch_Task.h"

void setup(){
  Serial.begin(115200); 
  DispatchTask_INIT();
}
 
void loop(){
  // Nothing to do here, FreeRTOS tasks handle the work
}