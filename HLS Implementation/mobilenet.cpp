#include <stdio.h>
#include "math.h"

#define EPSILON 1e-3

void mobilenet(
    float *input_data,
    float *output_data
){

#pragma HLS INTERFACE m_axi depth=256 port=input_data offset=slave bundle=inpd
#pragma HLS INTERFACE s_axilite port=input_data

#pragma HLS INTERFACE m_axi depth=256 port=output_data offset=slave bundle=inpd
#pragma HLS INTERFACE s_axilite port=output_data

#pragma HLS INTERFACE s_axilite port=return bundle=LeNet


float input_data_buf[1][40][24][1];  // Adjusted for padding
float conv1_W_buf[3][3][1][8];     // Reconstructed weights
float conv1_out[1][38][22][8];   // Output size with stride (2, 2)
float conv1_b_buf[8]; 
float bn1_out[1][38][22][8];   // Output size with stride (2, 2)
float relu_out[1][38][22][8];
float conv1d_W_buf[3][3][8][1];
float conv1d_out[1][36][20][8]; 
float bn2_out[1][36][20][8];
float relu_out_1[1][36][20][8];
float conv1p_W_buf[1][1][8][16];
float conv1p_out[1][36][20][16];
float bn3_out[1][36][20][16];
float relu_out_2[1][36][20][16];

float conv2_W_buf[3][3][16][32];
float bn4_out[1][34][18][32];   
float relu_out_3[1][34][18][32];   
float conv2d_W_buf[3][3][32][1];
float conv2d_out[1][32][16][32];
float bn5_out[1][32][16][32];
float relu_out_4[1][32][16][32];
float bn6_out[1][32][16][64];



// Input Loading


for (int i = 0; i < 40; i++) {
    for (int j = 0; j < 24; j++) {
            input_data_buf[0][i][j][0] = *input_data++;                   
    }
}                                         

// 2nd layer : Conv2D layer

#include "C:\Users\VICTUS\Desktop\New Model\conv1_b.txt"
#include "C:\Users\VICTUS\Desktop\New Model\conv1_w.txt"


int k = 0;
 for (int kr = 0; kr < 3; kr++) {  // Kernel rows
    for (int kc = 0; kc < 3; kc++) {  // Kernel columns
        for (int chi = 0; chi < 1; chi++) {  // Input channels (assuming 1 for grayscale)
            for (int cho = 0; cho < 8; cho++) {  // Output channels
                conv1_W_buf[kr][kc][chi][cho] = conv1_w[k];
                k++;
            }
        }
    }
}

for (int chi = 0; chi < 1; chi++) {
    for (int r = 0; r < 38; r++) {
        for (int c = 0; c < 22; c++) {
            for (int cho = 0; cho < 8; cho++) {
            conv1_out[chi][r][c][cho] = 0;
            }
        }
    }
}

for (int cho = 0; cho < 8; cho++) {
    for (int r = 0; r < 38; r++) {
        for (int c = 0; c < 22; c++) {
            float sum = 0.0;  // Sum initialized for each output element
            for (int kr = 0; kr < 3; kr++) {
                for (int kc = 0; kc < 3; kc++) {
                    sum += input_data_buf[0][r + kr][c + kc][0] * conv1_W_buf[kr][kc][0][cho];
                }
            }
            // Add bias after summation
            conv1_out[0][r][c][cho] = sum + conv1_b[cho];
        }
    }
}


// 2nd layer : Conv2D successfully completed !!


// 3rd layer : BatchNormalisation

#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\conv2d_1.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\conv2d_2.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\conv2d_3.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\conv2d_4.txt"


	
for(int r=0;r<38;r++){
    for(int c=0;c<22;c++){
        for(int co=0;co<8;co++){
            float normalized_value=(conv1_out[0][r][c][co]- batch_mean_1[co])/sqrt(batch_variance_1[co]+EPSILON);
            float scaled_value=gamma_1[co]*normalized_value+beta_1[co];
            bn1_out[0][r][c][co]=scaled_value;
        }
    }
}


// 3rd layer : BatchNormalisation successfully completed !!



// 4th layer : ReLu layer

for (int i = 0; i < 38; i++) {
    for (int j = 0; j < 22; j++) {
        for (int m = 0; m < 8; m++) {
            relu_out[0][i][j][m] = (bn1_out[0][i][j][m]>0) ? bn1_out[0][i][j][m] : 0;
        }
    }
}

// 4th layer : ReLu layer successfully completed !!




// 5th layer : Depthwise Convolution layer 

#include "C:\Users\VICTUS\Desktop\New Model\conv1d_b.txt"
#include "C:\Users\VICTUS\Desktop\New Model\conv1d_w.txt"

int i = 0;
for (int kr = 0; kr < 3; kr++) {  // Kernel rows
    for (int kc = 0; kc < 3; kc++) {  // Kernel columns
        for (int cho = 0; cho < 8; cho++) {  // Output channels
            conv1d_W_buf[kr][kc][cho][0] = conv1d_w[i];
            i++;
        }
    }
}

for (int r = 0; r < 36; r++) {
    for (int c = 0; c < 20; c++) {
        for (int cho = 0; cho < 8; cho++) {
            conv1d_out[0][r][c][cho] = 0;
        }
    }
}


for (int cho = 0; cho < 8; cho++) {
    for (int r = 0; r < 36; r++) {
        for (int c = 0; c < 20; c++) {
            float sum = 0.0;  // Sum initialized for each output element
            for (int kr = 0; kr < 3; kr++) {
                for (int kc = 0; kc < 3; kc++) {
                    sum += relu_out[0][r + kr][c + kc][cho] * conv1d_W_buf[kr][kc][cho][0];
                }
            }
            // Add bias after summation
                //printf(" x %f \t", sum);
            conv1d_out[0][r][c][cho] = sum + conv1d_b[cho];
            //printf(" %f \t", conv1_out[cho][r][c]);
        }
        // printf("\n");
    }
}

// 5th layer : Depthwise Convolution Successfully completed !!



// 6th layer : Batch Normalisation

#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convd1_1.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convd1_2.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convd1_3.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convd1_4.txt"

for(int r=0;r<36;r++){
    for(int c=0;c<20;c++){
        for(int co=0;co<8;co++){
            float normalized_value=(conv1d_out[0][r][c][co]- batch_mean_2[co])/sqrt(batch_variance_2[co]+EPSILON);
            float scaled_value=gamma_2[co]*normalized_value+beta_2[co];
            bn2_out[0][r][c][co]=scaled_value;
        }
    }
}

// 6th layer : Batch Normalisation ends



// 7th Layer : ReLU after 2nd BatchNorm
for (int i = 0; i < 36; i++) {
    for (int j = 0; j < 20; j++) {
        for (int m = 0; m < 8; m++) {
            relu_out_1[0][i][j][m] = (bn2_out[0][i][j][m]>0) ? bn2_out[0][i][j][m] : 0;
        }
    }
}

// 7th Layer completed

// 8th Layer : Pointwise Convolution (1x1)

#include "C:\Users\VICTUS\Desktop\New Model\conv2_b.txt"
#include "C:\Users\VICTUS\Desktop\New Model\conv2_w.txt"


for (int i = 0; i < 36; i++){
	for (int j = 0; j < 20; j++){
		for (int m = 0; m < 16; m++){
			conv1p_out[0][i][j][m] = 0;
		}
	}
}

int a = 0;
for (int j = 0; j < 8; j++){
    for (int i = 0; i < 16; i++){
        conv1p_W_buf[0][0][j][i] = conv2_w[a];
        a++;
    }
}

for(int r=0; r<36; r++) {
	for(int c=0; c<20; c++) {
		for(int cho=0; cho<16; cho++) {
			float sum = 0.0;    
			for(int chi=0; chi<8; chi++) {
                sum += relu_out_1[0][r][c][chi]* conv1p_W_buf[0][0][chi][cho];
            }
            conv1p_out[0][r][c][cho] = sum + conv2_b[cho];
        }
    }
}


// 8th layer Successfully completed !!

// 9th layer :Batch normalisation


#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convp1_1.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convp1_2.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convp1_3.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convp1_4.txt"

for(int r=0;r<36;r++){
    for(int c=0;c<20;c++){
        for(int co=0;co<16;co++){
            float normalized_value=(conv1p_out[0][r][c][co]- batch_mean_3[co])/sqrt(batch_variance_3[co]+EPSILON);
            float scaled_value=gamma_3[co]*normalized_value+beta_3[co];
            bn3_out[0][r][c][co]=scaled_value;
        }
    }
}

// layer 9 completed

// layer 10

for (int i = 0; i < 36; i++) {
    for (int j = 0; j < 20; j++) {
        for (int m = 0; m < 16; m++) {
            relu_out_2[0][i][j][m] = (bn3_out[0][i][j][m] > 0) ? bn3_out[0][i][j][m] : 0;
        }
    }
}

// layer 10 completed

// layer 11

#include "C:\Users\VICTUS\Desktop\New Model\conv3_b.txt"
#include "C:\Users\VICTUS\Desktop\New Model\conv3_w.txt"


float conv3_W_buf[3][3][16][32];
float conv3_out[1][34][18][32];

int o = 0;
 for (int kr = 0; kr < 3; kr++) {  // Kernel rows
    for (int kc = 0; kc < 3; kc++) {  // Kernel columns
        for (int chi = 0; chi < 16; chi++) {  // Input channels (assuming 1 for grayscale)
            for (int cho = 0; cho < 32; cho++) {  // Output channels
                conv3_W_buf[kr][kc][chi][cho] = conv3_w[o];
                o++;
                                //conv1_W_buf[kr][kc][chi][cho] = 2.0;
            }
        }
    }
}


for (int chi = 0; chi < 1; chi++) {
    for (int r = 0; r < 34; r++) {
        for (int c = 0; c < 18; c++) {
            for (int cho = 0; cho < 32; cho++) {
                conv3_out[chi][r][c][cho] = 0;
            }
        }
    }
}

for(int r=0; r<34; r++) {
	for(int c=0; c<18; c++) {
		for(int cho=0; cho<32; cho++) {
			float sum = 0.0;
             for (int kr = 0; kr < 3; kr++) {
                for (int kc = 0; kc < 3; kc++) {    
		        	for(int chi=0; chi<16; chi++) {
                        sum += relu_out_2[0][r+kr][c+kc][chi]* conv3_W_buf[kr][kc][chi][cho];
                     }
                }
            }
        conv3_out[0][r][c][cho] = sum + conv3_b[cho];
        }
   }
}


// layers 11 completed successfully

// layer 12 

#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\conv2d1_1.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\conv2d1_2.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\conv2d1_3.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\conv2d1_4.txt"

for(int r=0;r<34;r++){
    for(int c=0;c<18;c++){
        for(int co=0;co<32;co++){
            float normalized_value=(conv3_out[0][r][c][co]- batch_mean_4[co])/sqrt(batch_variance_4[co]+EPSILON);
            float scaled_value=gamma_4[co]*normalized_value+beta_4[co];
            bn4_out[0][r][c][co]=scaled_value;
        }
    }
}


// layer 12 completed

// layer 13

for (int i = 0; i < 34; i++) {
    for (int j = 0; j < 18; j++) {
        for (int m = 0; m < 32; m++) {
            relu_out_3[0][i][j][m] = (bn4_out[0][i][j][m] > 0) ? bn4_out[0][i][j][m] : 0;
        }
    }
}

// layer 13 completed

// layer 14 

#include "C:\Users\VICTUS\Desktop\New Model\conv2d_b.txt"
#include "C:\Users\VICTUS\Desktop\New Model\conv2d_w.txt"

i = 0;
for (int kr = 0; kr < 3; kr++) {  // Kernel rows
    for (int kc = 0; kc < 3; kc++) {  // Kernel columns
        for (int cho = 0; cho < 32; cho++) {  // Output channels
            conv2d_W_buf[kr][kc][cho][0] = conv2d_w[i];
            i++;
        }
    }
}

for (int r = 0; r < 32; r++) {
    for (int c = 0; c < 16; c++) {
        for (int cho = 0; cho < 32; cho++) {
            conv2d_out[0][r][c][cho] = 0;
        }
    }
}


for (int cho = 0; cho < 32; cho++) {
    for (int r = 0; r < 32; r++) {
        for (int c = 0; c < 16; c++) {
            float sum = 0.0;  // Sum initialized for each output element
            for (int kr = 0; kr < 3; kr++) {
                for (int kc = 0; kc < 3; kc++) {
                    sum += relu_out_3[0][r + kr][c + kc][cho] * conv2d_W_buf[kr][kc][cho][0];
                }
            }
            // Add bias after summation
                //printf(" x %f \t", sum);
            conv2d_out[0][r][c][cho] = sum + conv2d_b[cho];
            //printf(" %f \t", conv1_out[cho][r][c]);
        }
        // printf("\n");
    }
}

// layer 14 ends

// layer 15 starts

#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convd2_1.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convd2_2.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convd2_3.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convd2_4.txt"

for(int r=0;r<32;r++){
    for(int c=0;c<16;c++){
        for(int co=0;co<32;co++){
            float normalized_value=(conv2d_out[0][r][c][co]- batch_mean_5[co])/sqrt(batch_variance_5[co]+EPSILON);
            float scaled_value=gamma_5[co]*normalized_value+beta_5[co];
            bn5_out[0][r][c][co]=scaled_value;
        }
    }
}


// layer 15 ends

// layer 16 starts

for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 16; j++) {
        for (int m = 0; m < 32; m++) {
            relu_out_4[0][i][j][m] = (bn5_out[0][i][j][m] > 0) ? bn5_out[0][i][j][m] : 0;
        }
    }
}



// layer 16 ends

// layer 17 starts


float conv2p_out[1][32][16][64];
float conv2p_W_buf[1][1][32][64];

#include "C:\Users\VICTUS\Desktop\New Model\conv4_b.txt"
#include "C:\Users\VICTUS\Desktop\New Model\conv4_w.txt"


for (int i = 0; i < 32; i++){
	for (int j = 0; j < 16; j++){
		for (int m = 0; m < 64; m++){
			conv2p_out[0][i][j][m] = 0;
		}
	}
}

a = 0;
for (int j = 0; j < 32; j++){
    for (int i = 0; i < 64; i++){
        conv2p_W_buf[0][0][j][i] = conv4_w[a];
        a++;
    }
}

for(int r=0; r<32; r++) {
	for(int c=0; c<16; c++) {
		for(int cho=0; cho<64; cho++) {
			float sum = 0.0;    
			for(int chi=0; chi<32; chi++) {
                sum += relu_out_4[0][r][c][chi]* conv2p_W_buf[0][0][chi][cho];
            }
            conv2p_out[0][r][c][cho] = sum + conv4_b[cho];
        }
    }
}



// layer 17 ends

// layer 18 

#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convp2_1.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convp2_2.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convp2_3.txt"
#include "C:\Users\VICTUS\Desktop\New Model\Bnorm\convp2_4.txt"

for(int r=0;r<32;r++){
    for(int c=0;c<16;c++){
        for(int co=0;co<64;co++){
            float normalized_value=(conv2p_out[0][r][c][co]- batch_mean_6[co])/sqrt(batch_variance_6[co]+EPSILON);
            float scaled_value=gamma_6[co]*normalized_value+beta_6[co];
            bn6_out[0][r][c][co]=scaled_value;
        }
    }
}

// layer 18 ends

// layer 19 

float relu_out_5[1][32][16][64];

for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 16; j++) {
        for (int m = 0; m < 64; m++) {
            relu_out_5[0][i][j][m] = (bn6_out[0][i][j][m] > 0) ? bn6_out[0][i][j][m] : 0;
        }
    }
}

// layer 19 ends

// layer 20

float globalOutput[1][64];

for (int m = 0; m < 64; m++) {
    float sum = 0.0;
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 16; j++) {
            sum = sum + relu_out_5[0][i][j][m]; 
        }
    }
    globalOutput[0][m] = sum/512;
}

//layer 20 ends

//layer 21

float denseout[1][256];
float weights[64][256];

#include "C:\Users\VICTUS\Desktop\New Model\dnse_b.txt"
#include "C:\Users\VICTUS\Desktop\New Model\dnse_w.txt"

int n = 0;
for (int j = 0; j < 64; j++){
    for (int m = 0; m < 256; m++){
			weights[j][m] = dnse_w[n];
            n++;
    }
}



for (int out_neuron = 0; out_neuron < 256; out_neuron++) {
        float sum = 0.0;
        for (int in_neuron = 0; in_neuron < 64; in_neuron++) {
            sum += globalOutput[0][in_neuron] * weights[in_neuron][out_neuron];
        }
        // Add bias to the result
        denseout[0][out_neuron] = sum + dnse_b[out_neuron];
}

for (int i = 0; i < 256; i++) {
    denseout[0][i] = (denseout[0][i] > 0) ? denseout[0][i] : 0;
}

//layer 21 ends

//layer 22 : Final layer !

float output[1][12];
float weights_1[256][12];

#include "C:\Users\VICTUS\Desktop\New Model\dnse1_b.txt"
#include "C:\Users\VICTUS\Desktop\New Model\dnse1_w.txt"

int c = 0;
for (int j = 0; j < 256; j++){
    for (int m = 0; m < 12; m++){
			weights_1[j][m] = dnse1_w[c];
            c++;
    }
}



for (int out_neuron = 0; out_neuron < 12; out_neuron++) {
        float sum = 0.0;
        for (int in_neuron = 0; in_neuron < 256; in_neuron++) {
            sum += denseout[0][in_neuron] * weights_1[in_neuron][out_neuron];
        }
        // Add bias to the result
        output[0][out_neuron] = sum + dnse1_b[out_neuron];
}

for (int i = 0; i < 12; i++) {
    output[0][i] = (output[0][i] > 0) ? 1 : 0;
}

for (int i = 0; i < 12; i++) {
    *output_data++ = output[0][i];
}

}
