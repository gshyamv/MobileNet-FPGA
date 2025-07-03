#include <stdio.h>

void abtconv(
    float *input_data,
    float *convw,
    float *convb,
    float *output
);

int main(){

    #include "C:\Users\VICTUS\Desktop\New Model\abtconv\abtconvin.txt"
    #include "C:\Users\VICTUS\Desktop\New Model\abtconv\abtconv_w.txt"
    #include "C:\Users\VICTUS\Desktop\New Model\abtconv\abtconv_b.txt"

    float cvout[26752];
    float output[38][22][32];
    float temp[1] = {0};


    int it = 0;
    for(int ch = 0; ch < 32; ch++){
        for (int idx = 0; idx < 836; idx++) {
                cvout[836 * ch + idx] = convb[ch];
        }
        for (int j = 0; j < 16; j++) {
            float temp_output[836];  
            abtconv(&convin[j * 960], &convw[(it++) * 9], &temp[0], temp_output);

            for (int idx = 0; idx < 836; idx++) {
                cvout[836 * ch + idx] += temp_output[idx];
            }
        }
    }

    int i = 0;
    for (int ch = 0; ch < 32; ch++) {
        for (int r = 0; r < 38; r++) {    
            for (int c = 0; c < 22; c++) {
            output[r][c][ch] = cvout[i++];
            }
        }
    }
}
