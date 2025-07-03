#include <stdio.h>

void unidw(
    float *input_data,
    float *convw,
    float *convb,    
    float *dwoutput
){

float dw_W_buf[3][3][8];
float dwinput[38][22][8]; 


#pragma HLS INTERFACE m_axi depth=256 port=input_data offset=slave bundle=inp
#pragma HLS INTERFACE s_axilite port=input_data

#pragma HLS INTERFACE m_axi depth=256 port=convw offset=slave bundle=wgt
#pragma HLS INTERFACE s_axilite port=convw

#pragma HLS INTERFACE m_axi depth=256 port=convb offset=slave bundle=bias
#pragma HLS INTERFACE s_axilite port=convb

#pragma HLS INTERFACE m_axi depth=256 port=dwoutput offset=slave bundle=out
#pragma HLS INTERFACE s_axilite port=dwoutput

#pragma HLS INTERFACE s_axilite port=return

// Apply array partitioning for parallel access
#pragma HLS ARRAY_PARTITION variable=dw_W_buf complete dim=3
#pragma HLS ARRAY_PARTITION variable=dwinput complete dim=3

loop1: for (int cho = 0; cho < 8; cho++) {  // Output channels
	loop2: for (int kr = 0; kr < 3; kr++) {  // Kernel rows
		loop3: for (int kc = 0; kc < 3; kc++) {  // Kernel columns
            dw_W_buf[kr][kc][cho] = *convw++;
        }
    }
}

loop4: for (int i = 0; i < 8; i++) {
	loop5: for (int j = 0; j < 38; j++) {
		loop6: for (int k = 0;k < 22; k++){
            dwinput[j][k][i] = *input_data++;  
        }                 
    }
}

loop7: for (int cho = 0; cho < 8; cho++) {
	loop8: for (int r = 0; r < 36; r++) {
		loop9: for (int c = 0; c < 20; c++) {
            float sum = 0.0;  // Sum initialized for each output element
            loop10: for (int kr = 0; kr < 3; kr++) {
            	loop11: for (int kc = 0; kc < 3; kc++) {
                    sum += dwinput[r + kr][c + kc][cho] * dw_W_buf[kr][kc][cho];
                }
            }
                dwoutput[r * 20 * 8 + c * 8 + cho] = sum + convb[cho]; //instead of dwinput[r + kr][c + kc][cho] we tried this.
            }
        }
    }
}

