
#include <adf.h>
#include "adf/new_frontend/adf.h"
#include "kernels.h"
#include "kernels/include.h"

using namespace adf;

class simpleGraph : public adf::graph {
private:
  kernel first;
public:
  input_port modulus_port;
  input_port twiddle_factors_port;
  input_port barret_factors_port;

  input_plio  in_data;
  output_plio out_data;

  simpleGraph(){
    
    in_data  = input_plio::create(plio_32_bits, "data/input_data.txt");
    out_data = output_plio::create(plio_32_bits, "data/output.txt");

    first = kernel::create(ntt);

    connect(in_data.out[0], first.in[0]);
    connect<parameter>(modulus_port, async(first.in[1]));
    connect<parameter>(twiddle_factors_port, async(first.in[2]));
    connect<parameter>(barret_factors_port, async(first.in[3]));
    connect(first.out[0], out_data.in[0]);

    single_buffer(first.in[1]);
    single_buffer(first.in[2]);
    single_buffer(first.in[3]);

    dimensions(first.in[0]) = { NTT_LENGTH };
    dimensions(first.out[0]) = { NTT_LENGTH };

    source(first) = "kernels/kernels.cc";

    runtime<ratio>(first) = 0.1;
} };
