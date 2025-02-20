/* DOCUMENTATION
NAME : MANJUNATH R
DATE :30-01-2025
Discription : Takes a stego image (BMP file with hidden data) as input.
Validates the stego image file.
Extracts the hidden data from the stego image using the LSB technique.
Validates the magic string to ensure the correct stego image is being decoded.
Recovers the secret file extension, size, and data.
Saves the decoded data into an output file.*/
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <unistd.h>

// Function to read and validate decoding arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decoinfo)
{
    // Check if the stego image file has a .bmp extension
    if(strstr(argv[2],".bmp") != NULL)
    {
        if(strcmp(strstr(argv[2],".bmp"),".bmp")==0) // Validate .bmp extension
        {
            decoinfo->stego_image_fname=argv[2]; // Set stego image file name
        }
        else
        {
            printf("\n********** Invalid Extension ***********❌\n");
            return e_failure;
        }
    }
    else
    {
        printf("\n<<<<<<<<<<<<<<<< PROBLEM WITH THE ENCODDED FILE NAME DECODING STOPPED >>>>>>>>>❌\n");
        return e_failure;
    }

    // Set output file name (default is "out" if not provided)
    if(argv[3]==NULL)
    {
       strcpy(decoinfo->out_fname,"out"); // Default output file name
    } 
    else
    {
        strcpy(decoinfo->out_fname,argv[3]); // Use provided output file name
    }
    printf("\n<<<<<<<<<<<< READING AND VALIDATION OF FILES ARE SUCCESSFULL >>>>>>>>>>>✅\n");
    return e_success;
}

// Function to perform the decoding process
Status do_decoding(DecodeInfo *decoinfo)
{
    // Open the stego image file
    int open_file = open_file_src(decoinfo);
    sleep(3); // Simulate delay
    if(open_file != e_success)
    {
        printf("\n**************** ERROR IN OPENING FILE HEADER *******************❌\n");
        return e_failure;
    }
    printf("\n<<<<<<<<<<<<< FILE OPENING FOR DECODING IS SUCCESSFULL >>>>>>>>>>>>>>>>✅\n");

    // Skip the BMP header (first 54 bytes)
    int skip_hed=skip_bmp_header(decoinfo->fptr_stego_image);
    sleep(3); // Simulate delay
    if(skip_hed != e_success)
    {
        printf("\n**************** EROR IN SKIPING THE HEADER FOR DECODING SECRET *****************❌\n");
        return e_failure;
    }
    printf("\n<<<<<<<<<<<<<<< SKIPING BMP HEADER IS SUCCESSFULL >>>>>>>>>>>>>>>✅\n");

    // Decode the magic string size
    decoinfo->size_magic_string=decode_lsb_to_size(decoinfo->fptr_stego_image); // Decode size of magic string
    int magic_str=decode_magic_string(decoinfo->size_magic_string,decoinfo); // Decode magic string
    sleep(3); // Simulate delay
    if(magic_str!=e_success)
    {
        printf("\n****************  ERROR : INVALID MAGIC STRING *************❌\n");
        return e_failure;
    }
    printf("\n>>>>><<<<<<<<< DECODING MAGIC STRING IS SUCCESSFULL >>>>>>>>>>>>>>>✅\n");

    // Decode the secret file extension size
    int exten_size =decode_secret_file_ext_size(decoinfo); // Decode size of secret file extension
    sleep(3); // Simulate delay
    if(exten_size!=e_success)
    {
        printf("\n ****************** Erorr : not able find size of extension ***************\n");
        return e_failure;
    }
    printf("\n<<<<<<<<<<<<< SECRETE FILE EXTENSION SIZE IS FOUNDED SUCCESSFULLY >>>>>>>>>>>>>>>>>>>✅\n");

    // Decode the secret file extension
    int extn=decode_secret_file_extn(decoinfo); // Decode secret file extension
    sleep(3); // Simulate delay
    if(extn!=e_success)
    {
        printf("\n********************* EROR IN EXTENSION *****************❌\n");
        return e_failure;
    }
    printf("\n<<<<<<<<<<<<< SECRETE FILE EXTENSION FOUNDED SUCCESSFULLY >>>>>>>>>>>>>>>>>>>✅\n");

    // Decode the size of the secret file
    int size=decode_secret_file_size(decoinfo); // Decode size of secret file
    sleep(5); // Simulate delay
    if(size!=e_success)
    {
        printf("\n ****************** Erorr : not able find size of secrete file ***************❌\n");
        return e_failure;
    }
    printf("\n<<<<<<<<<<<<< SECRETE FILE SIZE IS FOUNDED SUCCESSFULLY >>>>>>>>>>>>>>>>>>>✅\n");

    // Decode the data of the secret file
    int dta= decode_secret_file_data(decoinfo); // Decode secret file data
    sleep(3); // Simulate delay
    if(dta!=e_success)
    {
        printf("\n********** NOT ABLE DECODE THE SECRETE FILE DATA ***************❌\n");
        return e_failure;
    }
    printf("\n<<<<<<<<<<<<< SECRETE FILE DATA FOUNDED SUCCESSFULLY >>>>>>>>>>>>>>>>>>>✅\n");
    return e_success;
}

// Function to skip the BMP header (first 54 bytes)
Status skip_bmp_header(FILE *fptr_stego_image)
{
    fseek(fptr_stego_image,54,SEEK_SET); // Move file pointer to skip 54 bytes
    return e_success;
}

// Function to open the stego image file
Status open_file_src(DecodeInfo *decoinfo)
{
    decoinfo->fptr_stego_image=fopen(decoinfo->stego_image_fname,"r"); // Open stego image file
    if ( decoinfo->fptr_stego_image== NULL) // Check for file open error
    {
    	printf("\n____________ERROR : FILE IS NOT EXIST_____________❌\n");
    	return e_failure;
    }
    return e_success;
}

// Function to decode size from LSB of 32 bytes
int decode_lsb_to_size(FILE *fptr_stego)
{
    int data=0;
    for(int i=0;i<32;i++) // Iterate through 32 bytes
    {
        char ch;
        fread(&ch,1,1,fptr_stego); // Read 1 byte
        ch=ch&1; // Extract LSB
        if(ch)
        {
            int temp=0;
            temp=1<<(31-i); // Shift bit to correct position
            data=data|temp; // Set bit in data
        }
    }
    return data; // Return decoded size
}

// Function to decode the magic string
Status decode_magic_string(int size,DecodeInfo *decoinfo)
{
    char magic_str[size],user_str[size];

    for(int i=0;i<size;i++) // Decode each character of magic string
    {
        magic_str[i]=decode_lsb_to_byte(decoinfo->fptr_stego_image); // Decode byte from LSB
    }
    printf("\n>>>>>>>>>>>ENTER THE MAGIC STRING : ");
    scanf(" %s",user_str); // Get user input for magic string
    if(strcmp(user_str,MAGIC_STRING)!=0) // Compare with expected magic string
    {
        printf("\n************* Entered wrong Magic string ***********❌\n");
        return e_failure;
    }
    printf("\n\n______________________Loading 🔃___________________\n");
    sleep(5); // Simulate delay
    printf("\n>>>>>>>>>>>>>>>>>>> MAGIC STRING IS MATCHING <<<<<<<<<<<<<<<<✅\n");
    return e_success;
}

// Function to decode a byte from LSB of 8 bytes
char decode_lsb_to_byte(FILE *fptr_stego)
{
    char data=0;
    for(int i=0;i<8;i++){ // Iterate through 8 bytes
      char ch;
       fread(&ch,1,1,fptr_stego); // Read 1 byte
       ch=ch&1; // Extract LSB
       if(ch){
        char temp=0;
       temp=1<<(7-i); // Shift bit to correct position
       data=data|temp; // Set bit in data
       }
    }
    return data; // Return decoded byte
}

// Function to decode the size of the secret file extension
Status decode_secret_file_ext_size(DecodeInfo *decoinfo)
{
    decoinfo->size_ext=decode_lsb_to_size(decoinfo->fptr_stego_image); // Decode size from LSB
    return e_success;
}

// Function to decode the secret file extension
Status decode_secret_file_extn(DecodeInfo *decoinfo)
{
    for(int i=0;i<4;i++) // Decode 4 characters of file extension
    {
        decoinfo->ext_secret_file[i]=decode_lsb_to_byte(decoinfo->fptr_stego_image); // Decode byte from LSB
    }
    decoinfo->ext_secret_file[4]='\0'; // Null-terminate the string
    strtok(decoinfo->out_fname,"."); // Remove existing extension from output file name
    strcat(decoinfo->out_fname,decoinfo->ext_secret_file); // Append decoded extension
    decoinfo->fptr_output=fopen(decoinfo->out_fname,"w"); // Open output file
    return e_success;
}

// Function to decode the size of the secret file
Status decode_secret_file_size(DecodeInfo *decoinfo)
{
    decoinfo->size_file=decode_lsb_to_size(decoinfo->fptr_stego_image); // Decode size from LSB
    return e_success;
}

// Function to decode the secret file data
Status decode_secret_file_data(DecodeInfo *decoinfo)
{
     char user_str[decoinfo->size_file];

    for(int i=0;i<decoinfo->size_file;i++) // Decode each byte of secret file data
    {
        user_str[i]=decode_lsb_to_byte(decoinfo->fptr_stego_image); // Decode byte from LSB
        fputc(user_str[i],decoinfo->fptr_output); // Write byte to output file
    }
    user_str[decoinfo->size_file]='\0'; // Null-terminate the string
    printf("\n->->->THE SECRETE DATA IS : < %s > ",user_str); // Print decoded data
    return e_success;
}