
#include <adf.h>
#include "kernels.h"
#include "kernels/include.h"

using namespace adf;

class simpleGraph : public adf::graph {
private:
  kernel first;
public:
  input_plio  in_factors;
  //input_plio in_barretFactors;
  input_plio  in_data;
  output_plio out;
  simpleGraph(){
    
    in_factors  = input_plio::create(plio_32_bits, "data/input_factors.txt");
    //in_barretFactors  = input_plio::create(plio_32_bits, "data/input_barretFactors.txt");
    in_data  = input_plio::create(plio_32_bits, "data/input_data.txt");
    out = output_plio::create(plio_32_bits, "data/output.txt");

    first = kernel::create(ntt);
    connect(in_data.out[0], first.in[0]);
    connect(in_factors.out[0], first.in[1]);
    //connect(in_barretFactors.out[0], first.in[2]);
    connect(first.out[0], out.in[0]);
    dimensions(first.in[0]) = { NTT_LENGTH };
    dimensions(first.in[1]) = { FACTORS_LENGTH };
    //dimensions(first.in[2]) = { FACTORS_LENGTH };
    dimensions(first.out[0]) = { NTT_LENGTH };

    source(first) = "kernels/kernels.cc";

    runtime<ratio>(first) = 0.1;
} };
