
#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H

  void ntt(adf::input_buffer<uint16> & in_data, adf::input_buffer<uint16>& in_factors, adf::output_buffer<uint16> & out);
  //void ntt(adf::input_buffer<uint16> & in_data, adf::input_buffer<uint16>& in_factors, adf::input_buffer<uint16>& in_barretFactors, adf::output_buffer<uint16> & out);

#endif
