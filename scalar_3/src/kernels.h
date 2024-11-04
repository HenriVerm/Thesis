
#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H

  #include "kernels/include.h"
void ntt(adf::input_buffer<uint16> & in_data, adf::output_buffer<uint16> & out, const uint16 modulus, const uint16 (&in_twiddle_factors)[FACTORS_LENGTH], const uint16 (&in_barret_factors)[FACTORS_LENGTH]);

#endif
