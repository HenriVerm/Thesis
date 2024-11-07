
#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H

#include "kernels/include.h"

void ntt(adf::input_buffer<uint16> & in_data, adf::output_buffer<uint16> & out, adf::input_async_buffer<uint16>& in_twiddle_factors_buf, adf::input_async_buffer<uint16>& in_barret_factors_buf);

#endif
