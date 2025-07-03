#include <stdio.h>
#include <math.h> 

#define EPSILON 1e-3

void abtbn(
    float *input_data,
    float *mean,
    float *variance,
    float *gamma,
    float *beta,    
    float *bnoutput 
){

    #pragma HLS INTERFACE m_axi depth=256 port=input_data offset=slave bundle=inp
    #pragma HLS INTERFACE s_axilite port=input_data

    #pragma HLS INTERFACE m_axi depth=256 port=mean offset=slave bundle=mean
    #pragma HLS INTERFACE s_axilite port=mean

    #pragma HLS INTERFACE m_axi depth=256 port=variance offset=slave bundle=var
    #pragma HLS INTERFACE s_axilite port=variance

    #pragma HLS INTERFACE m_axi depth=256 port=gamma offset=slave bundle=gma
    #pragma HLS INTERFACE s_axilite port=gamma

    #pragma HLS INTERFACE m_axi depth=256 port=beta offset=slave bundle=bta
    #pragma HLS INTERFACE s_axilite port=beta

    #pragma HLS INTERFACE m_axi depth=256 port=bnoutput offset=slave bundle=out
    #pragma HLS INTERFACE s_axilite port=bnoutput

    #pragma HLS INTERFACE s_axilite port=return


    loop1: for(int co = 0; co < 8; co++) {
        loop2: for(int r = 0; r < 38; r++) {
            loop3: for(int c = 0; c < 22; c++) {
                float normalized_value = (*input_data++ - mean[co]) / sqrt(variance[co] + EPSILON);
                float scaled_value = gamma[co] * normalized_value + beta[co];
                *bnoutput++ = scaled_value;
            }
        }
    }
}