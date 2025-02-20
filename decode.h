#ifndef DECODE_H
#define DECODE_H

#include "types.h"


 typedef struct  _DECODEInfo
 {
    //STEGO FILE
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char image_data[8];


    //ouput file
    char out_fname[50];
    FILE *fptr_output;
    int size_ext;
    char ext_secret_file[5];
    int size_file;

    int size_magic_string;
}DecodeInfo;

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decoinfo);

//To Skip bmp header to decode data
Status skip_bmp_header(FILE *fptr_stego_image);

/* Get File pointers for i/p and o/p files */
Status open_file_src(DecodeInfo *decoinfo);

/* Decode Magic String */
Status decode_magic_string(int size,DecodeInfo *decoinfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decoinfo);

/* Decode a byte from LSB of image data array */
char decode_lsb_to_byte(FILE *fptr_src);

/* Decode a size from LSB of image data array */
int decode_lsb_to_size(FILE *fptr_src);

/* Decode secret file extenstion size */
Status decode_secret_file_ext_size(DecodeInfo *decoinfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decoinfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decoinfo);

/* Decode secret file  */
Status decode_secret_file_data(DecodeInfo *decoinfo);

#endif