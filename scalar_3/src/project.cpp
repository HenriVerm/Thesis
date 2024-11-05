
#include <adf.h>
#include <cmath>
#include "adf/window/types.h"
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
    unsigned long long rev = 0;
    for (unsigned short i = 0; i < NO_OF_BITS; i++)
        if ((a & (1 << i))) rev |= 1 << ((NO_OF_BITS - 1) - i);
    LOG("input : %llu, bitrev : %llu\n", a, rev);
    return rev;
}


// Compute the twiddle factors
void compute_twiddle_factors(uint16 (&twiddle_factors)[FACTORS_LENGTH]) {
    twiddle_factors[0] = 1;
    uint16 cur = twiddle_factors[0];
    for (unsigned long long i = 1; i < FACTORS_LENGTH; i++) {
      cur = uint32(cur * OMEGA) % MODULUS;
      LOG("cur : %d\n", cur);
      twiddle_factors[bitrev(i, NO_LAYERS-1)] = cur; // NO_LAYERS = log_2(FACTORS_LENGTH)
    } 
}



// Compute the barret factors of the twiddle factors
void compute_barret_factors(const uint16 (&twiddle_factors)[FACTORS_LENGTH], uint16 (&barret_factors)[FACTORS_LENGTH]) {
    for (unsigned long long i = 0; i < FACTORS_LENGTH; i++) {
      const float rounded = std::round(float(twiddle_factors[i] << K) / float(MODULUS));
      LOG("twiddle precalc: %f, rounded : %f\n", float(twiddle_factors[i] << K) / float(MODULUS), rounded);
      if (rounded >= (1<<(sizeof(uint16)*8))) {
          LOG("ERROR : barret factor too large\n"); 
      }
      barret_factors[i] = uint16(rounded);
      LOG("twiddle : %d\n", twiddle_factors[i]);
      LOG("barret : %d\n", barret_factors[i]);
    } 
}