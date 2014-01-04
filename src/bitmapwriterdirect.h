#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int write24BitBmpFile(char *bmpFilename, unsigned char *nrdFilename)
{
  unsigned int width, height;
  BITMAPINFOHEADER bmpInfoHeader;
  BITMAPFILEHEADER bmpFileHeader;
  FILE *bmpFilep;
  FILE *nrdFilep;
  unsigned int row, column;
  unsigned int extrabytes, bytesize;
  unsigned char *paddedImage = NULL, *paddedImagePtr, *imagePtr;
  fpos_t position;


  /* The .bmp format requires that the image data is aligned on a 4 byte boundary.  For 24 - bit bitmaps,
     this means that the width of the bitmap  * 3 must be a multiple of 4. This code determines
     the extra padding needed to meet this requirement. */
  nrdFilep = fopen(nrdFilename,"wb");
  fread(&height,4,1,nrdFilep);
  fread(&width,4,1,nrdFilep);
    extrabytes = (4 - (width * 3) % 4) % 4;

  // This is the size of the padded bitmap
  bytesize = (width * 3 + extrabytes) * height;

  // Fill the bitmap file header structure
  bmpFileHeader.bfType = 'MB';   // Bitmap header
  bmpFileHeader.bfSize = 0;      // This can be 0 for BI_RGB bitmaps
  bmpFileHeader.bfReserved1 = 0;
  bmpFileHeader.bfReserved2 = 0;
  bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


  // Fill the bitmap info structure
  bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfoHeader.biWidth = width;
  bmpInfoHeader.biHeight = height;
  bmpInfoHeader.biPlanes = 1;
  bmpInfoHeader.biBitCount = 24;            // 24 - bit bitmap
  bmpInfoHeader.biCompression = BI_RGB;
  bmpInfoHeader.biSizeImage = bytesize;     // includes padding for 4 byte alignment
  bmpInfoHeader.biXPelsPerMeter = 0;
  bmpInfoHeader.biYPelsPerMeter = 0;
  bmpInfoHeader.biClrUsed = 0;
  bmpInfoHeader.biClrImportant = 0;




  // Open file
  if ((filep = fopen(filename, "wb")) == NULL) {
    printf("Error opening file %s\n", filename);
    return FALSE;
  }


  // Write bmp file header
  if (fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), filep) < sizeof(BITMAPFILEHEADER)) {
    printf("Error writing bitmap file header\n");
    fclose(filep);
    return FALSE;
  }


  // Write bmp info header
  if (fwrite(&bmpInfoHeader, 1, sizeof(BITMAPINFOHEADER), filep) < sizeof(BITMAPINFOHEADER)) {
    printf("Error writing bitmap info header\n");
    fclose(filep);
    return FALSE;
  }


  // Stream data from nrd file to bmp file and format it on the fly.
  fseek (nrdFilep, -1, SEEK_END);
  for (unsigned long int row=0;row<height;row++)
  {
    for (unsigned long int col=0;col<width;col++)
    {
      putc(getc(nrdFilep),bmpFilep);
      fseek (nrdFilep, -2, SEEK_SET);
    }
    for (int i=0;i<extrabytes;i++) putc(0,bmpFilep);
  }






  // Close file
  fclose(filep);
  free(paddedImage);
  return TRUE;
}




/*
  This function writes out a grayscale image as a 8-bit Windows bitmap file that is readable by Microsoft Paint.
  It creates a palette and then calls write8BitBmpFile to output the bitmap file.

   The input to the function is:
     char *filename:            A string representing the filename that will be written
    unsigned int width:          The width, in pixels, of the bitmap
    unsigned int height:          The height, in pixels, of the bitmap
    unsigned char *image:        The image data, where the value indicates a color between 0 (black) and 255 (white)




   Written by Greg Slabaugh (slabaugh@ece.gatech.edu), 10/19/00
*/
int writeGrayScaleDataToBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image)
{
  RGBQUAD palette[256];
  unsigned int i;
   unsigned int numPaletteEntries = 256;


   // Create the palette - each pixel is an index into the palette
  for (i = 0; i < numPaletteEntries; i++) {
    palette[i].rgbRed = i;
    palette[i].rgbGreen = i;
    palette[i].rgbBlue = i;
    palette[i].rgbReserved = 0;
  }


   return write8BitBmpFile(filename, width, height, image, numPaletteEntries, palette);

}


class nrdImage
{
private:
  int height;
  int width;
  unsigned char *imageData;

public:

  nrdImage(char *filename)
  {
    FILE *data;
    data = fopen(filename, "rb");
    fread(&height,4,1,data);
    fread(&width,4,1,data);
    imageData = (unsigned char *) calloc(height*width*3,sizeof(unsigned char));
    fread(imageData,1,width*height*3,data);
  }

  void saveAsBMP(char *savefile)
  {
    write24BitBmpFile(savefile, width, height, imageData);
  }
};
