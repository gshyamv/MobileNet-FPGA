#include <stdio.h>

void abtconv(
    float *input_data,
    float *convw,
    float *convb,
    float *output
){

	#pragma HLS INTERFACE m_axi depth=256 port=input_data offset=slave bundle=inp
    #pragma HLS INTERFACE s_axilite port=input_data

    #pragma HLS INTERFACE m_axi depth=256 port=convw offset=slave bundle=wgt
    #pragma HLS INTERFACE s_axilite port=convw

    #pragma HLS INTERFACE m_axi depth=256 port=convb offset=slave bundle=bias
    #pragma HLS INTERFACE s_axilite port=convb

    #pragma HLS INTERFACE m_axi depth=256 port=output offset=slave bundle=out
    #pragma HLS INTERFACE s_axilite port=output

    #pragma HLS INTERFACE s_axilite port=return


    float convinput[40][24];
    float cweight[3][3];


	#pragma HLS ARRAY_PARTITION variable=convinput complete dim=2
	#pragma HLS ARRAY_PARTITION variable=cweight complete dim=2


    loop1: for (int i = 0; i < 40; i++) {
		#pragma HLS UNROLL
    	loop2: for (int j = 0; j < 24; j++) {
            convinput[i][j] = *input_data++;  
        }
    }

    loop3: for(int a = 0;a < 3; a++){
		#pragma HLS UNROLL
    	loop4: for(int b = 0; b < 3; b++){
            cweight[a][b] = *convw++;
        }
    }        

    loop5: for(int r=0; r<38; r++) {
    	loop6: for(int c=0; c<22; c++) {
        float sum = 0.0;
        loop7: for (int kr = 0; kr < 3; kr++) {
			#pragma HLS UNROLL
        	loop8: for (int kc = 0; kc < 3; kc++) {
                sum += convinput[r+kr][c+kc]* cweight[kr][kc];
                }
            }
            *output++ = sum + *convb;
        }
    }
}
