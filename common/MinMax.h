#ifndef COMMON_MINMAX_H
#define COMMON_MINMAX_H

#include <algorithm>

#if defined( min ) || defined( max )
#error "Don't define min and/or max, use std::min and std::max instead!"
#endif

using std::min;
using std::max;

#endif //COMMON_MINMAX_H
