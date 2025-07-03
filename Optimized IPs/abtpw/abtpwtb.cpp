#include <stdio.h>

void abtpw(
    float *input_data,
    float *pw_w,  
    float *pw_b,
    float *pwoutput
);

int main(){
    
    float pwoutput[46080];  // Adjust size according to output dimensions
    float pwout[36][20][64];
    float temp[1] = {0};


    #include "C:\Users\VICTUS\Desktop\New Model\abtpw\abtpwin.txt"
    #include "C:\Users\VICTUS\Desktop\New Model\abtpw\abtpw_b.txt"
    #include "C:\Users\VICTUS\Desktop\New Model\abtpw\abtpw_w.txt"

        int it = 0;
        for (int i = 0; i < 1; i++) {
            for (int idx = 0; idx < 720; idx++) {
                pwoutput[720 * i + idx] =  pwb[i];
            }

        for (int j = 0; j < 1; j++) {
            float temp_output[720];
            abtpw(&pwin[j * 5760], &pww[(it++) * 8], &temp[0], temp_output);
            
            for (int idx = 0; idx < 720; idx++) {
                pwoutput[720 * i + idx] += temp_output[idx];
            }
        }
    }

 // For Testing

 /*   int i = 0;
    for (int c = 0; c < 64; c++) {
        for (int k = 0; k < 36; k++) {    
            for (int r = 0; r < 20; r++) {
            pwout[k][r][c] = pwoutput[i++];
            }
        }
    }

    int row = 1;
    int col = 1;
    int ch = 10;

    printf("\nValue of output[%d][%d][%d] = %f\n",row,col,ch,pwout[row][col][ch]);
    
*/


}

