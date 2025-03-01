<<<<<<< HEAD
#ifndef ATTRIBUTE_TASK_H_
#define ATTRIBUTE_TASK_H_
=======
#ifndef ATTRIBUTES_TASK_H_
#define ATTRIBUTES_TASK_H_
>>>>>>> publish_task

#include "global.h"

/// @brief Update callback that will be called as soon as one of the provided shared attributes changes value,
/// if none are provided we subscribe to any shared attribute change instead
/// @param data Data containing the shared attributes that were changed and their current value
void processSharedAttributeUpdate(const JsonObjectConst &data);

<<<<<<< HEAD
/// @brief Update telemetry data to thingsboard broker
/// @param _data Data formatted as JSON string
void update_data(const String &data);

// void publish_task_init();
=======
/// @brief Subscribe to device's shared attributes 
bool shared_attributes_setup();
>>>>>>> publish_task

#endif