/* A simple kernel
 */

#include <adf.h>
#include "adf/new_frontend/adf_api_dynlink.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "include.h"

#ifdef __X86SIM__
#include <adf/x86sim/x86simDebug.h>
#endif

X86SIM_THREAD_LOCAL aie::tile tile=aie::tile::current(); //get the tile of the kernel

__attribute__((always_inline)) uint16 modular_reduction(const uint16 a, const uint16 modulus) {
  if (a >= modulus) {
    return a - modulus;
    LOG("Input of mod_red: %d, output : %d\n", a, a - modulus);
  } else {
    LOG("Input of mod_red: %d, output : %d\n", a, a);
    return a;
  }
}



__attribute__((always_inline)) uint16 modular_multiplication(const uint16 factor, const uint16 barret_factor, const uint16 a, const uint16 modulus) {
  uint32 t = (a * barret_factor) >> K;
  uint16 res = a*factor - modulus*t; // Eignk round ipv floor
  LOG("Input of mod_mul: %d, %d, %d, t : %d, res : %d\n", factor, barret_factor, a, t, res);
  return res;
}


void ntt(adf::input_buffer<uint16> & in_data, adf::output_buffer<uint16> & out, adf::input_async_buffer<uint16>& in_twiddle_factors_buf, adf::input_async_buffer<uint16>& in_barret_factors_buf) {
  in_twiddle_factors_buf.acquire();
  in_barret_factors_buf.acquire();

  uint16* in_twiddle_factors = in_twiddle_factors_buf.data();
  uint16* in_barret_factors = in_barret_factors_buf.data();
  

  uint16* inDataItr = in_data.data();
  uint16* outItr = out.data();
  const uint16 modulus = MODULUS;


  LOG("Modulus : %d\n", modulus);

  // Invar : At innermost loop : crossover_sections_n * butterflies_per_section_double == NTT_LENGTH
  int crossover_sections_n = 1; // Number of butterfly sections
  int butterflies_per_section = NTT_LENGTH>>1; // Number of butterflies in each section
  int butterflies_per_section_double = NTT_LENGTH; // Number of butterflies in each section x2 = Number of samples per section
  for (unsigned i=0; i< NO_OF_LAYERS; i++) { // Loop over layers
    uint16* section_start = inDataItr;
    for (unsigned j=0; j < crossover_sections_n; j++) { // Loop over butterfly sections
      uint16 omega_cur = in_twiddle_factors[j];
      uint16 barret_omega_cur = in_barret_factors[j];
      uint16* current_butterfly_firstterm_pos = section_start;
      uint16* current_butterfly_lastterm_pos = section_start + butterflies_per_section;
      for (unsigned k=0; k < butterflies_per_section; k++) { // Loop over the butterflies in a section
        LOG("Omega: %d, barret_omega : %d, a1 : %d, a2 : %d\n", omega_cur, barret_omega_cur, *current_butterfly_firstterm_pos, *current_butterfly_lastterm_pos);
        uint16 T = modular_multiplication(omega_cur, barret_omega_cur, (*current_butterfly_lastterm_pos), modulus);
        uint16 U = (*current_butterfly_firstterm_pos);
        (*current_butterfly_firstterm_pos) = U + T;
        (*current_butterfly_lastterm_pos) = U - T + modulus;
        LOG("a1 : %d, a2 : %d\n", *current_butterfly_firstterm_pos, *current_butterfly_lastterm_pos);
        current_butterfly_firstterm_pos++;
        current_butterfly_lastterm_pos++;
      }
      section_start += butterflies_per_section_double;
    }
    butterflies_per_section_double = butterflies_per_section_double >> 1;
    butterflies_per_section = butterflies_per_section >> 1;
    crossover_sections_n = crossover_sections_n << 1;
  }

  // Copy to output
  inDataItr = in_data.data();
  for (unsigned i=0; i < NTT_LENGTH; i++) {
    //*inDataItr = modular_reduction(*inDataItr);
    LOG("Output of scalar_3: %d\n", *inDataItr);
    *outItr++ = modular_reduction(*inDataItr++, modulus);
  }

/*
#ifdef __X86SIM__
    printf("%s: %s, line %d\n", __FUNCTION__, X86SIM_KERNEL_NAME, __LINE__);
#endif
#ifndef  __X86SIM__
    printf("cycles : %llu, col : %d, row : %d\n", tile.cycles(), tile.global_id().col, tile.global_id().row);
#endif
    printf("%s: line %d\n", __FUNCTION__, __LINE__);
*/
}

