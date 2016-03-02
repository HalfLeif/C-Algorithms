#include "debug.h"

namespace logging {
namespace Debug {
static bool debug_enabled_ = false;

void Enable() { debug_enabled_ = true; }
void Disable() { debug_enabled_ = false; }
bool IsEnabled() { return debug_enabled_; }
}  // namespace Debug
}  // namespace logging
