
#include <adf.h>
#include <cmath>
#include "adf/window/types.h"
#include "kernels.h"
#include "kernels/include.h"
#include "project.h"

using namespace adf;


void compute_twiddle_factors(uint16* twiddle_factors);
void compute_barret_factors(uint16* twiddle_factors, uint16* barret_factors);

simpleGraph mygraph;

int main(void) { 
  mygraph.init();

  // Determine twiddle factors
  uint16 *twiddle_factors=(uint16*)GMIO::malloc(FACTORS_LENGTH*sizeof(uint16));
  uint16 *barret_factors=(uint16*)GMIO::malloc(FACTORS_LENGTH*sizeof(uint16));
  compute_twiddle_factors(twiddle_factors);
  compute_barret_factors(twiddle_factors, barret_factors);

  // Set RTP
  mygraph.gmioInFactors.gm2aie_nb(twiddle_factors, FACTORS_LENGTH*sizeof(uint16));
  mygraph.gmioInBarret.gm2aie_nb(barret_factors, FACTORS_LENGTH*sizeof(uint16));
  
  // Run the Graph
  mygraph.run(NO_OF_NTTS);
  mygraph.end();
  return 0;
}


// Bit reverse
unsigned long long bitrev(const unsigned long long a, const unsigned short NO_OF_BITS) {
    unsigned long long rev = 0;
    for (unsigned short i = 0; i < NO_OF_BITS; i++)
        if ((a & (1 << i))) rev |= 1 << ((NO_OF_BITS - 1) - i);
    LOG("input : %llu, bitrev : %llu\n", a, rev);
    return rev;
}

// Round a float to a uint16
// Custom function to not have to rely on std which might not be present
uint16 round(const float a) {
    if (a >= (1<<(sizeof(uint16)*8)) - 1) {
        printf("ERROR : barret factor too large, which shouldn't happen\n"); 
    }
    if (a >= 65536-1) {
        printf("ERROR : barret factor too large, which shouldn't happen\n"); 
    }
    const uint16 rounded = uint16(a + 0.5);
    return rounded;
}

// Compute the twiddle factors
void compute_twiddle_factors(uint16* twiddle_factors) {
    twiddle_factors[0] = 1;
    uint16 cur = twiddle_factors[0];
    for (unsigned long long i = 1; i < FACTORS_LENGTH; i++) {
      cur = uint32(cur * OMEGA) % MODULUS;
      LOG("cur : %d\n", cur);
      twiddle_factors[bitrev(i, NO_OF_LAYERS-1)] = cur; // NO_LAYERS = log_2(FACTORS_LENGTH)
    } 
}



// Compute the barret factors of the twiddle factors
void compute_barret_factors(uint16* twiddle_factors, uint16* barret_factors) {
    for (unsigned long long i = 0; i < FACTORS_LENGTH; i++) {
      const uint16 rounded = round(float(twiddle_factors[i] << K) / float(MODULUS));
      LOG("twiddle precalc: %f, rounded : %d\n", float(twiddle_factors[i] << K) / float(MODULUS), rounded);
      barret_factors[i] = uint16(rounded);
      LOG("twiddle : %d\n", twiddle_factors[i]);
      LOG("barret : %d\n", barret_factors[i]);
    } 
}