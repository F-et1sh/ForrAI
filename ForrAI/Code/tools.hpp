#pragma once

namespace fa {
#define FA_CONCATENATE_DIRECT(x, y) x##y
#define FA_CONCATENATE(x, y) FA_CONCATENATE_DIRECT(x, y)
#define FA_VARIABLE_ON_LINE(str) FA_CONCATENATE(str, __LINE__)
} // namespace fa
