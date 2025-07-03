#include <stdio.h>
#include <stdlib.h>

// Declare Mobilenet function
void mobilenet(
    float *input_data,
    float *output_data
);

int main() {

    float output_data[12];

    #include "C:\Users\VICTUS\Desktop\New Model\img_id.txt"

    mobilenet(flattened_input, output_data);    

    for (int b=0; b < 12; b++) {
        printf("%f \t",output_data[b]);
    } 

    return 0;
}