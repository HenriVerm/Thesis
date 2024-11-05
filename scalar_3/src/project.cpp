
#include <adf.h>
#include <cmath>
#include "kernels.h"
#include "project.h"

using namespace adf;


void compute_twiddle_factors(uint16 (&twiddle_factors)[FACTORS_LENGTH]);
void compute_barret_factors(const uint16 (&twiddle_factors)[FACTORS_LENGTH], uint16 (&barret_factors)[FACTORS_LENGTH]);

simpleGraph mygraph;

int main(void) {
  mygraph.init();

  // Determine twiddle factors
  uint16 twiddle_factors[FACTORS_LENGTH]; 
  compute_twiddle_factors(twiddle_factors);
  uint16 barret_factors[FACTORS_LENGTH];
  compute_barret_factors(twiddle_factors, barret_factors);

  // Set RTP
  mygraph.update(mygraph.modulus_port, uint16(MODULUS));
  mygraph.update(mygraph.twiddle_factors_port, twiddle_factors, FACTORS_LENGTH);
  mygraph.update(mygraph.barret_factors_port, barret_factors, FACTORS_LENGTH);
  
  // Run the Graph
  mygraph.run(1);
  mygraph.end();
  return 0;
}


// Bit reverse
unsigned long long bitrev(unsigned long long a, unsigned short NO_OF_BITS) {
    unsigned long long reverse_num = 0;
    for (unsigned short i = 0; i < NO_OF_BITS; i++)
        if ((num & (1 << i))) reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    return reverse_num;
}


// Compute the twiddle factors
void compute_twiddle_factors(uint16 (&twiddle_factors)[FACTORS_LENGTH]) {
    twiddle_factors[0] = 1;
    uint16 cur = twiddle_factors[0];
    for (unsigned long long i = 0; i < FACTORS_LENGTH; i++) {
      cur = uint32(cur * OMEGA) % MODULUS;
      twiddle_factors[bitrev(i, NO_LAYERS)] = cur; // NO_LAYERS = log_2(FACTORS_LENGTH)
    } 
}



// Compute the barret factors of the twiddle factors
void compute_barret_factors(const uint16 (&twiddle_factors)[FACTORS_LENGTH], uint16 (&barret_factors)[FACTORS_LENGTH]) {
    for (unsigned long long i = 0; i < FACTORS_LENGTH; i++) {
      barret_factors[i] = std::round((twiddle_factors[i] << K) / MODULUS);
    } 
}