/* DOCUMENTATION
NAME : MANJUNATH R
DATE :30-01-2025
Discription :Takes a BMP image and a secret file (text file) as input.
Validates the input files (checks for correct extensions like .bmp and .txt).
Embeds the secret data into the BMP image using the LSB technique.
Generates a stego image (output BMP file) containing the hidden data.
Supports encoding of:
Magic string (for validation during decoding).
Secret file extension and size.
Secret file data.
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<string.h>
#include<unistd.h>

// Function to check the operation type (encode or decode) based on command-line arguments
OperationType check_operation_type(char *argv[]){
   if(! strcmp(argv[1],"-e")) // Check if the argument is "-e" for encoding
   { 
        return e_encode; 
   } 
    else if(!strcmp(argv[1],"-d")) // Check if the argument is "-d" for decoding
    {
       return e_decode;
    }
    else if(argv[1]==NULL) // If no argument is provided
    {
      printf("ERROR : INVALID ARGUMENTS ...❌\n");
      return e_unsupported;
   }
   else // If the argument is neither "-e" nor "-d"
   {
      printf("...............Unsupported type.................❌\n");
      return e_unsupported;
    }
}

// Function to read and validate encoding arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
   // Check if the source image file has a .bmp extension and the secret file has a .txt extension
   if((strstr(argv[2],".bmp")!= NULL) && (strstr(argv[3],".txt")!=NULL ))
   {
      if(strcmp(strstr(argv[2],".bmp"),".bmp")==0) // Validate .bmp extension
      {
         encInfo->src_image_fname=argv[2]; // Set source image file name
      }
      else
      {
         printf("\n.........Invalid Extensions .............❌\n");
         return e_failure;
      }
   }
   else
   {
      printf("\n.................SOURCE FILE NAME NOT GIVEN..............❓\n");
      return e_failure; 
   }

   // Validate the secret file extension (.txt or .c)
   if((strcmp(strstr(argv[3],".txt"),".txt")==0) || (strcmp(strstr(argv[3],".c"),".c")==0) || (strcmp(strstr(argv[3],".txt"),".txt")==0))
   {
      encInfo->secret_fname=argv[3]; // Set secret file name
   }
   else
   {
      printf(">>>>>>>>>>>>>>>>Invalid Extension................❌\n");
      return e_failure;
   } 

   // Validate the output file extension (.bmp) or set a default name
   if(argv[4]!=NULL)
   {
      if(strcmp(strstr(argv[4],".bmp"),".bmp")==0)
         encInfo->stego_image_fname=argv[4]; // Set output file name
      else
      {
         printf(">>>>>>>>>>>>>>Output file should be bmp ❌>>>>>>>>>>>>>>");
         return e_failure;
      }
   }
   else
   {
      encInfo->stego_image_fname="output.bmp"; // Default output file name
      printf("\n>>>>>>>>>>>>>>>> The output file is created with name of 'output.bmp'<<<<<<<<<<<✅\n");
   }
   printf("\n.......READING AND VALIDATION FILES ARE SUCCESSFULL........✅\n");    
   return e_success;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Open source image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if (encInfo->fptr_src_image == NULL) // Check for file open error
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file ❌ %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Open secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL) // Check for file open error
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file ❌ %s\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Open stego image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    if (encInfo->fptr_stego_image == NULL) // Check for file open error
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file ❌ %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }

    return e_success; // Return success if all files are opened
}

// Function to perform the encoding process
Status do_encoding(EncodeInfo *encInfo)
{
   // Open files for encoding
   if(open_files(encInfo)!=e_success)
   {
      printf(">>>>>>>>ERROR : PROBLEM IN OPENING FILES ❌.................\n");
      return e_failure;
   }
   printf("\n>>>>>> FILE OPENING IS SUCCESSFULL <<<<<<<✅\n");
  
   sleep(5); // Simulate delay
   // Check if the source image has enough capacity to encode the secret file
   if(check_capacity(encInfo)!=e_success) 
   {
      printf("......Soure file having the less capacity......❌\n");
      return e_failure;
   }
   printf("\n>>>>>>> FILE CAPACITY CHECKING SUCCESSFULL <<<<<<<✅\n");

   sleep(5); // Simulate delay
   // Copy BMP header from source to stego image
   int copybmp=copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);
   int pos1,pos2;
   if(copybmp!=e_success)
   {
      printf("................header copying is not done..................❌\n");
      return e_failure;
   }
   pos1=ftell(encInfo->fptr_src_image);
   pos2=ftell(encInfo->fptr_stego_image);
   if(pos1==pos2)
   printf("\n>>>>>>>>>>> BMP HEADER COPIED SUCCESSFULLY <<<<<<<<<<<<<✅\n");

   sleep(5); // Simulate delay
   // Encode magic string and its length
   int encode_magic=encode_magic_string(MAGIC_STRING,encInfo);
   if(encode_magic!=e_success)
   {
    printf("\n.........Magic string length and data not added...........❌\n");
    return e_failure;
   }
   pos1=ftell(encInfo->fptr_src_image);
   pos2=ftell(encInfo->fptr_stego_image);
   if(pos1==pos2)
   printf("\n>>>>>>>>> MAGICSTRING AND LENGTH OF MAGIC STRING SUCCESSFULLY ENCODED <<<<<<<<<<<<<<✅\n");

   sleep(5); // Simulate delay
   // Encode secret file extension and its size
   char *file_exten = strstr(encInfo->secret_fname,".");
   int file_e=encode_secret_file_extn(file_exten, encInfo);
   if(file_e == e_failure)
   {
        printf("\n.................Encoding of secret file ext and size is not done.............❌\n");
        return e_failure;
   }
   pos1=ftell(encInfo->fptr_src_image);
   pos2=ftell(encInfo->fptr_stego_image);
   if(pos1==pos2)
   {
      printf("\n>>>>>>>>>ENCODING FILE EXTENSION IS DONE <<<<<<<<<<<<<<<<<<✅\n");
   }
   
   // Encode secret file size
   sleep(5); // Simulate delay
   int secret_file_size=encode_secret_file_size(encInfo->size_secret_file, encInfo);
   if(secret_file_size != e_success){
        printf("\n...............Secret file size not encoded................❌\n");
        return e_failure;
    }
     pos1=ftell(encInfo->fptr_src_image);
    pos2=ftell(encInfo->fptr_stego_image);
    if(pos1==pos2)
      printf("\n >>>>>>>>>> SECRET FILE SIZE IS SUCCESSFULLY ENCODED <<<<<<<<<<<<✅\n");

   sleep(5); // Simulate delay
   // Encode secret file data
   rewind(encInfo->fptr_secret);
   int file_data= encode_secret_file_data(encInfo);
    if(file_data!=e_success){
      printf("\n.............Secret file data not encoded..................❌\n");
      return e_failure;
    }
    pos1=ftell(encInfo->fptr_src_image);
    pos2=ftell(encInfo->fptr_stego_image);
    if(pos1==pos2)
    printf("\n >>>>>>>>>> SECRET FILE DATA IS SUCCESSFULLY ENCODED <<<<<<<<<<<<✅\n");
    sleep(5); // Simulate delay
    // Copy remaining data from source to stego image
   int copy_rem=copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);
   if(copy_rem!=e_success){
      printf("\n.......Remaining data of source file is not copied.......❌\n");
      return e_failure;
   }
   pos1=ftell(encInfo->fptr_src_image);
   pos2=ftell(encInfo->fptr_stego_image);
   if(pos1==pos2)
      printf("\n>>>>>>>>>>>>>> REMAINING DATA OF SOURCE FILE IS COPIED TO OUTPUT FILE SUCCESSFULLY <<<<<<<<<<<<✅\n");
   return e_success;
}

// Function to check if the source image has enough capacity to encode the secret file
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image); // Get source image size
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret); // Get secret file size
    if(encInfo->size_secret_file==0) // Check if secret file is empty
      return e_failure;
    else if(encInfo->image_capacity > ((32 + 32 + 4 +sizeof(MAGIC_STRING) + encInfo->size_secret_file)*8)) // Check capacity
      return e_success;
}

// Function to get the size of a file
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END); // Move to the end of the file
     int size=ftell(fptr); // Get file size
     if(size==0) // Check if file is empty
     {
     printf("\n........File is empty.........❌\n");
     return 0;
     }
     return size;
}

/* Function to get the size of a BMP image */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET); // Move to the 18th byte to read width
    fread(&width, sizeof(int), 1, fptr_image); // Read width
    fread(&height, sizeof(int), 1, fptr_image); // Read height
    return width * height * 3; // Return image capacity (width * height * 3 bytes per pixel)
}

// Function to copy BMP header (54 bytes) from source to destination
Status copy_bmp_header(FILE * fptr_src_image,FILE *fptr_dest_image)
{
    char arr[54];
    rewind(fptr_src_image); // Move to the start of the source file
    rewind(fptr_dest_image); // Move to the start of the destination file
    fread(arr,54,1,fptr_src_image); // Read 54 bytes from source
    fwrite(arr,54,1,fptr_dest_image); // Write 54 bytes to destination
    return e_success;
}

// Function to encode magic string and its length
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int size=strlen(MAGIC_STRING); // Get magic string length
    char brr[32],arr[8];
    fread(brr,32,1,encInfo->fptr_src_image); // Read 32 bytes from source
    encode_size_to_lsb(size,brr); // Encode size into LSB
    fwrite(brr,32,1,encInfo->fptr_stego_image); // Write 32 bytes to stego image
    for(int i=0;i<size;i++) // Encode each character of magic string
    {
        fread(arr,8,1,encInfo->fptr_src_image); // Read 8 bytes from source
        encode_byte_to_lsb(magic_string[i], arr); // Encode character into LSB
        fwrite(arr,8,1,encInfo->fptr_stego_image); // Write 8 bytes to stego image
    }
    return e_success;
}

// Function to encode a byte into LSB of image buffer
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=7;i>=0;i--){
    unsigned char mask=(((unsigned)(data>>i))&1); // Extract bit
    image_buffer[7-i]=(image_buffer[7-i]&(~1)); // Clear LSB
    image_buffer[7-i]=(image_buffer[7-i]|mask); // Set LSB
}
}

// Function to encode size into LSB of image buffer
Status encode_size_to_lsb(int data,char *image_buffer){
    for(int i=31;i>=0;i--)
    {
        unsigned char mask=((data>>i)&1); // Extract bit
        image_buffer[31-i]=(image_buffer[31-i]&(~1)); // Clear LSB
        image_buffer[31-i]=(image_buffer[31-i]|mask); // Set LSB
    }
}

// Function to encode secret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
   int size=strlen(file_extn); // Get file extension length
   char arr[32];
   fread(arr,32,1,encInfo->fptr_src_image); // Read 32 bytes from source
   encode_size_to_lsb(size,arr); // Encode size into LSB
   fwrite(arr,32,1,encInfo->fptr_stego_image); // Write 32 bytes to stego image

   // Encode file extension
   char a[8];
   for(int i=0;i<4;i++)
   {
   fread(a,8,1,encInfo->fptr_src_image); // Read 8 bytes from source
   encode_byte_to_lsb(file_extn[i],a); // Encode character into LSB
   fwrite(a,8,1,encInfo->fptr_stego_image); // Write 8 bytes to stego image
   }
   return e_success;
}

// Function to encode secret file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char arr[32];
    fread(arr,32,1,encInfo->fptr_src_image); // Read 32 bytes from source
    encode_size_to_lsb(file_size ,arr); // Encode size into LSB
    fwrite(arr,32,1,encInfo->fptr_stego_image); // Write 32 bytes to stego image
    return e_success;
}

// Function to encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
   int size=encInfo->size_secret_file; // Get secret file size
   char arr[size],a[8];
   fread(arr,size,1,encInfo->fptr_secret); // Read secret file data
   for(int i=0;i<size;i++) // Encode each byte of secret file
   {
      fread(a,8,1,encInfo->fptr_src_image); // Read 8 bytes from source
      encode_byte_to_lsb(arr[i],a); // Encode byte into LSB
      fwrite(a,8,1,encInfo->fptr_stego_image); // Write 8 bytes to stego image
   }
    return e_success;
}

// Function to copy remaining image data from source to stego image
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
   while( (fread(&ch,1,1,fptr_src))){ // Read each byte from source
    fwrite(&ch,1,1,fptr_dest); // Write each byte to destination
    }
    return e_success;
}