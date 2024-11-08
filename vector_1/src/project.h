
#include <adf.h>
#include "adf/new_frontend/adf.h"
#include "kernels.h"
#include "kernels/include.h"

using namespace adf;

class simpleGraph : public adf::graph {
private:
  kernel first;
public:
  adf::input_gmio gmioInFactors;
  adf::input_gmio gmioInBarret;

  input_plio  in_data;
  output_plio out_data;

  simpleGraph(){
    
    in_data  = input_plio::create(plio_32_bits, "data/input_data.txt");
    out_data = output_plio::create(plio_32_bits, "data/output.txt");


    gmioInFactors = adf::input_gmio::create("gmioInFactors", 64, 1000);
    gmioInBarret = adf::input_gmio::create("gmioInBarret", 64, 1000);

    first = kernel::create(ntt);

    connect(in_data.out[0], first.in[0]);
    connect(gmioInFactors.out[0], first.in[1]);
    connect(gmioInBarret.out[0], first.in[2]);
    connect(first.out[0], out_data.in[0]);

    single_buffer(first.in[1]);
    single_buffer(first.in[2]);


    dimensions(first.in[0]) = { NTT_LENGTH };
    dimensions(first.out[0]) = { NTT_LENGTH };

    dimensions(first.in[1]) = { FACTORS_LENGTH };
    dimensions(first.in[2]) = { FACTORS_LENGTH };

    source(first) = "kernels/kernels.cc";

    runtime<ratio>(first) = 0.1;
} };
