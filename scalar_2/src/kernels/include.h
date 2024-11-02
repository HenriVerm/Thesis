
#ifndef FUNCTION_INCLUDES_H
#define FUNCTION_INCLUDES_H

#define NTT_LENGTH 4
#define NO_LAYERS 2
#define FACTORS_LENGTH NTT_LENGTH>>1

#define MODULUS 7681 // To remain in uint16 : Posibilities for (M,K) = (1,13);(2,14);(4,15),(8,16); Best (1,13) ig want eignk komen ze allemaal op zelfde neer, maar dan geen mult 
#define K 16

#endif
