
#ifndef FUNCTION_INCLUDES_H
#define FUNCTION_INCLUDES_H

// Modulus
#define MODULUS 3329 // 7681
#define OMEGA 17 // 256-th root   // 3383

// NTT Size
#define NO_OF_NTTS 1
#define NTT_LENGTH 256
#define NO_OF_LAYERS 8
#define FACTORS_LENGTH (1 << (NO_OF_LAYERS - 1))

// Barret multiplication
#define K 16 // To remain in uint16 : Posibilities for (M,K) = (1,13);(2,14);(4,15),(8,16); Best (1,13) ig want eignk komen ze allemaal op zelfde neer, maar dan geen mult

// DEBUG?
//#define DEBUG







// Printf
#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#endif
