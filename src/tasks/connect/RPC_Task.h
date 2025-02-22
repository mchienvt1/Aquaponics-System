#ifndef RPC_TASK_H_
#define RPC_TASK_H_

#include "global.h"

/// @brief Processes function for RPC call "process_relay_status"
/// @param data Data containing the rpc data that was called and its current value
/// @param response Data containgin the response value, any number, string or json, that should be sent to the cloud. Useful for getMethods
void process_relay_status(const JsonVariantConst &data, JsonDocument &response);

/// @brief Processes function for RPC call "process_rgb_status"
/// @param data Data containing the rpc data that was called and its current value
/// @param response Data containgin the response value, any number, string or json, that should be sent to the cloud. Useful for getMethods
void process_rgb_status(const JsonVariantConst &data, JsonDocument &response);

#endif // RPC_TASK_H_