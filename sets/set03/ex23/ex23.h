#pragma once
#include <cstdint>
#include "../ex21/ex21.h"

namespace set03::ex23
{

// Untemper a single MT19937 output to recover state element
uint32_t untemper(uint32_t y);

// Clone an MT19937 generator by observing 624 outputs
set03::ex21::MT19937 clone_mt19937(set03::ex21::MT19937 &original);

} // namespace set03::ex23
