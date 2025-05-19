#ifndef ATTRIBUTES_TASK_H_
#define ATTRIBUTES_TASK_H_

#include "global.h"
#include "../device/AI_Task.h"

/// @brief Update callback that will be called as soon as one of the provided shared attributes changes value,
/// if none are provided we subscribe to any shared attribute change instead
/// @param data Data containing the shared attributes that were changed and their current value
void processSharedAttributeUpdate(const JsonObjectConst &data);

/// @brief Subscribe to device's shared attributes
bool shared_attributes_setup();

void processClientAttributeRequest(const JsonObjectConst &data);

void request_client_attributes();

void processSharedAttributeRequest(const JsonObjectConst &data);

void request_shared_attributes();

#endif