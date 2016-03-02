// Example:
// logging::Debug::Enable();
// DEBUG_LOG(ERROR) << "Message...";
#ifndef DEBUG_H
#define DEBUG_H

#include "logging.h"

#define DEBUG_LOG(stream) if(::logging::Debug::IsEnabled()) LOG(stream)

namespace logging {
namespace Debug {
void Enable();
void Disable();
bool IsEnabled();
}  // namespace Debug
}  // namespace logging

#endif
