#include <stdio.h>

void abtpw(
    float *input_data,
    float *pw_w,  
    float *pw_b,
    float *pwoutput
){
    
	#pragma HLS INTERFACE m_axi depth=256 port=input_data offset=slave bundle=in
	#pragma HLS INTERFACE s_axilite port=input_data

	#pragma HLS INTERFACE m_axi depth=256 port=pw_w offset=slave bundle=wgt
	#pragma HLS INTERFACE s_axilite port=pw_w

	#pragma HLS INTERFACE m_axi depth=256 port=pw_b offset=slave bundle=bias
	#pragma HLS INTERFACE s_axilite port=pw_b

	#pragma HLS INTERFACE m_axi depth=256 port=pwoutput offset=slave bundle=out
	#pragma HLS INTERFACE s_axilite port=pwoutput

	#pragma HLS INTERFACE s_axilite port=return


    float input[36][20][8];
    float ws[8];


	#pragma HLS ARRAY_PARTITION variable=input complete dim=3
	#pragma HLS ARRAY_PARTITION variable=ws complete dim=1
    
    loop1: for (int k = 0; k < 8; k++){
    	loop2: for (int i = 0; i < 36; i++){
    		loop3: for (int j = 0;j < 20; j++){
            input[i][j][k] = *input_data++;
            }
        }
    }
    loop4: for (int i = 0; i < 8; i++){
    	ws[i] = *pw_w++;
    }
    
    loop5: for (int r = 0; r < 36; r++){
    	loop6: for (int c = 0; c < 20; c++){
		    float sum = 0.0;
		    loop7: for (int chi = 0; chi < 8; chi++){
                sum += input[r][c][chi] * ws[chi];
            }
            *pwoutput++ = sum + *pw_b;
        }
    }

}

