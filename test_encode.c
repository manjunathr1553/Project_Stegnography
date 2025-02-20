/* DOCUMENTATION
NAME : MANJUNATH R
DATE :30-01-2025
Discription : This project is a Steganography Tool implemented in C, 
designed to encode secret data into BMP images and decode the hidden data from stego images. 
The tool uses the Least Significant Bit (LSB) technique to embed secret information (such as text files) into the pixel data of BMP images without visibly altering the image. 
It also supports decoding the hidden data from the stego image.*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;  // Structure to hold encoding information
    DecodeInfo decoInfo; // Structure tKo hold decoding information

    // Check the operation type (encode or decode) based on command-line arguments
    int ret = check_operation_type(argv);

    if (ret == e_encode) // If the operation is encoding
    {
        printf("\n>>>>>>>>>>>>>> ENCODING STARTED <<<<<<<<<<<<<<✅\n");
        sleep(5); // Simulate a delay (for effect or debugging)
        // Read and validate encoding arguments from command line
        int ret1 = read_and_validate_encode_args(argv, &encInfo);
        if (ret1 != e_success) // If validation fails
        {
            printf("\n.............Encoding Process failed due to improper filenames..............❌\n");
            return e_failure; // Exit with failure status
        }        

        // Perform the encoding process
        ret1 = do_encoding(&encInfo);
        if (ret1 == e_success) // If encoding is successful
        {
            printf("\n..........................................loading 🔃...................................................\n");
            sleep(5); // Simulate a delay (for effect or debugging)
            printf("\n>>>>>>>>>>>>>>>>> ENCODING PROCESS IS COMPLETED SUCCESSFULLY <<<<<<<<<<<<<<<<<<✅\n");
        }
    }

    else if (ret == e_decode) // If the operation is decoding
    {
        printf("\n>>>>>>>>>>>>>> DECODING STARTED <<<<<<<<<<<<<<✅\n");
        // Read and validate decoding arguments from command line
        sleep(5);
        int ret2 = read_and_validate_decode_args(argv, &decoInfo);
        if (ret2 != e_success) // If validation fails
        {
            printf("\n...............Decoding Process failed due to improper filenames.....................❌\n");
            return e_failure; // Exit with failure status
        }
        // Perform the decoding process
        ret2 = do_decoding(&decoInfo);
        if (ret2 == e_success) // If decoding is successful
        {
            printf("\n..........................................loading 🔃...................................................\n");
            sleep(5); // Simulate a delay (for effect or debugging)
            printf("\n>>>>>>>>>>>>>>>>> DECODING PROCESS IS COMPLETED SUCCESSFULLY <<<<<<<<<<<<<<<<<<✅\n");
        }
    }
    else
    {
        printf("\n**** ERROR : INVALID INPUTS *******❌\n" );
    }

    return 0; // Exit with success status
}