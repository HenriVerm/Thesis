
#ifndef FUNCTION_INCLUDES_H
#define FUNCTION_INCLUDES_H

// Modulus
#define MODULUS 7681
#define OMEGA 3383  

// NTT Size
#define NTT_LENGTH 4
#define NO_LAYERS 2
#define FACTORS_LENGTH (2 << (NO_LAYERS))

// Barret multiplication
#define K 16 // To remain in uint16 : Posibilities for (M,K) = (1,13);(2,14);(4,15),(8,16); Best (1,13) ig want eignk komen ze allemaal op zelfde neer, maar dan geen mult

#endif
