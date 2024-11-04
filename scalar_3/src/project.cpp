
#include <adf.h>
#include "kernels.h"
#include "kernels/include.h"
#include "project.h"

using namespace adf;

simpleGraph mygraph;

int main(void) {
  mygraph.init();

  // Set RTP
  uint16 twiddle_factors[FACTORS_LENGTH] = {1, 3383};
  uint16 barret_factors[FACTORS_LENGTH] = {9, 28865};
  mygraph.update(mygraph.modulus_port, uint16(MODULUS));
  mygraph.update(mygraph.twiddle_factors_port, twiddle_factors, FACTORS_LENGTH);
  mygraph.update(mygraph.barret_factors_port, barret_factors, FACTORS_LENGTH);
  
  mygraph.run(1);
  mygraph.end();
  return 0;
}
