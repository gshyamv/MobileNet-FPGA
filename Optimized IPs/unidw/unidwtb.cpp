#include <stdio.h>

void unidw(
    float *input_data,
    float *convw,
    float *convb,    
    float *dwoutput
);

int main() {

    float cvout[1][36][20][32];
    int offset_dwin = 6688;  // Offset for each input data section
    int offset_dwweights = 72; // Offset for each weights section
    int offset_dwbias = 8;     // Offset for each bias section
    float output[5760*4];

    #include "C:/Users/VICTUS/Desktop/New Model/ndw/ndwin_2.txt"
    #include "C:/Users/VICTUS/Desktop/New Model/ndw/ndwbias_2.txt"
    #include "C:/Users/VICTUS/Desktop/New Model/ndw/ndw_weights2.txt"


    unidw(&dwin[0], &dwweights[0*offset_dwweights],&dwbias[0*offset_dwbias], &output[0]);
    unidw(&dwin[1*offset_dwin], &dwweights[1*offset_dwweights],&dwbias[1*offset_dwbias], &output[1*5760]);
    unidw(&dwin[2*offset_dwin], &dwweights[2*offset_dwweights],&dwbias[2*offset_dwbias], &output[2*5760]);
    unidw(&dwin[3*offset_dwin], &dwweights[3*offset_dwweights],&dwbias[3*offset_dwbias], &output[3*5760]);
    

    int index = 0;
    for (int section = 0; section < 4; section++){
        for (int r = 0; r < 36; r++) {
            for (int c = 0; c < 20; c++) {
                for (int ch = 0; ch < 8; ch++) {
                cvout[0][r][c][section * 8 + ch] = output[index++];
                }
            }
        }
    }

    int row = 31;
	int col = 14;
	int ch = 30;

	printf("\nValue of output[%d][%d][%d] = %f\n",row,col,ch,cvout[0][row][col][ch]);

    return 0;
}
