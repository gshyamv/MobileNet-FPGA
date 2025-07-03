#include <stdio.h>

void abtbn(
    float *input_data,
    float *mean,
    float *variance,
    float *gamma,
    float *beta,    
    float *bnoutput 
);

int main() {

    float bnout[1][38][22][64];
    int iter = 8; 
   
    #include "C:/Users/VICTUS/Desktop/New Model/abtbn/bn_in.txt"
    #include "C:/Users/VICTUS/Desktop/New Model/abtbn/convp2_1.txt"
    #include "C:/Users/VICTUS/Desktop/New Model/abtbn/convp2_2.txt"
    #include "C:/Users/VICTUS/Desktop/New Model/abtbn/convp2_3.txt"
    #include "C:/Users/VICTUS/Desktop/New Model/abtbn/convp2_4.txt"
    
    for(int ly = 0; ly < iter; ly++) {
        float output[6688];
        abtbn(&bnin[ly * 6688], &batch_mean_6[8*ly],&batch_variance_6[8*ly],&gamma_6[8*ly],&beta_6[8*ly], output);

        int index = 0;
        for (int ch = 0; ch < 8; ch++) {
            for (int r = 0; r < 38; r++) {
                for (int c = 0; c < 22; c++) {
                    bnout[0][r][c][ly * 8 + ch] = output[index++];
                }
            }
        }
    }

    int row = 31;
	int col = 15;
	int ch = 63;

	printf("\nValue of output[%d][%d][%d] = %f\n",row,col,ch,bnout[0][row][col][ch]);

    return 0;
}
