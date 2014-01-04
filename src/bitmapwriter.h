#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream.h>

#define TWENTYFOURMB 25165824
#define DEBUG false

int nrd2bmp(char *nrdFilename, char *bmpFilename)
{
  cerr << "HI!";
  unsigned int width, height;
  BITMAPINFOHEADER bmpInfoHeader;
  BITMAPFILEHEADER bmpFileHeader;
  FILE *bmpFilep;
  FILE *nrdFilep;
  //unsigned int row, column;
  unsigned int extrabytes, bytesize;
  //long position, oldPosition, blockSize, bufferSize;
  fpos_t cursor;
  unsigned char * readBuffer, * writeBuffer;
  unsigned long bufferIndex=2;
  unsigned long nrdFilesize;

  /* The .bmp format requires that the image data is aligned on a 4 byte boundary.  For 24 - bit bitmaps,
     this means that the width of the bitmap  * 3 must be a multiple of 4. This code determines
     the extra padding needed to meet this requirement. */
  nrdFilep = fopen(nrdFilename,"rb");
  fread(&height,4,1,nrdFilep);
  fread(&width,4,1,nrdFilep);

  unsigned long writeIndex;
  unsigned long readBufferSize = TWENTYFOURMB - (TWENTYFOURMB%(3*width)); //set it just under 24 megs memory, and also evenly divisible by row of RGB pixels (3*width)
  if(DEBUG) cerr << "readBufferSize: " << readBufferSize << "\n";
  extrabytes = (4 - (width * 3) % 4) % 4;
  unsigned long rowsInReadBuffer=readBufferSize/(3*width);
  unsigned long writeBufferSize = readBufferSize+extrabytes*rowsInReadBuffer; //addition is for padding.
  nrdFilesize= 8 + 3*width*height;
  unsigned long largeBuffersNeeded=nrdFilesize/readBufferSize;
  unsigned long smallBufferSize = nrdFilesize - largeBuffersNeeded*readBufferSize - 8; //the minus 8 is to account for the first 8 int spaces
  unsigned long fileIndex = nrdFilesize;
  unsigned long readBufferRowIndex = rowsInReadBuffer; //start at end...will proceed to beginning...
  unsigned long readBufferColIndex = 0;

  readBuffer = (unsigned char *) calloc(readBufferSize, sizeof(unsigned char)); //calloc(how many elements, size of elements);
  if (readBuffer==NULL)
  {
    cerr << "No mo' memory for readBuffer.\n";
    return false;
  }
  writeBuffer = (unsigned char *) calloc(writeBufferSize, sizeof(unsigned char));
  if (writeBuffer==NULL)
  {
    cerr << "No mo' memory for writeBuffer.\n";
    return false;
  }

  if (DEBUG) cerr << "all variables declared okay\n";

  // This is the size of the padded bitmap
  bytesize = (width * 3 + extrabytes) * height;

  // Fill the bitmap file header structure
  bmpFileHeader.bfType = 'BM';   // Bitmap header
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





  if ((bmpFilep = fopen(bmpFilename, "wb")) == NULL)  // Open file
  {
    printf("Error opening file %s\n", bmpFilename);
    return FALSE;
  }



  if (fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), bmpFilep) < sizeof(BITMAPFILEHEADER))// Write bmp file header
  {
    printf("Error writing bitmap file header\n");
    fclose(bmpFilep);
    return FALSE;
  }



  if (fwrite(&bmpInfoHeader, 1, sizeof(BITMAPINFOHEADER), bmpFilep) < sizeof(BITMAPINFOHEADER))// Write bmp info header
  {
    printf("Error writing bitmap info header\n");
    fclose(bmpFilep);
    return FALSE;
  }




    //
  //endPosition = nrdFilesize;
  //cursor=endPosition;  //cursor to track harddisk position

  if (DEBUG) cerr << "Got to our loop yaaaay!! \n";


  cursor=nrdFilesize;
  if (DEBUG) printf("cursor starting out at: %d \n",(unsigned long)cursor);
  for (unsigned int i=0;i<largeBuffersNeeded; i++)
  {
    if(DEBUG) cerr << "cursor is (top): " << (unsigned long)cursor << "\n";
    cursor -= readBufferSize;  //move back to read position
    if(DEBUG) cerr << "cursor got decremented by readBufferSize \n";
    fsetpos(nrdFilep,&cursor);
    fread(readBuffer, readBufferSize, sizeof(unsigned char), nrdFilep); //fread(where you're putting it, how many elements, size of element, file pointer
    if(DEBUG) cerr << "loaded the readBuffer and fileIndex is: " << fileIndex << "\n";

    for (readBufferRowIndex=rowsInReadBuffer-1;readBufferRowIndex>0;readBufferRowIndex--)// loop through rows from last to first
    {
      if(DEBUG) cerr << "starting top loop, row index: " << readBufferRowIndex << "\n";
      for (readBufferColIndex=0;readBufferColIndex<width;readBufferColIndex++) //loop through cols from first to last within each row.
      {
        if(DEBUG) cerr << "trying to assing: " << readBufferRowIndex*3*width + readBufferColIndex*3 + 2 << "\n";
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width + readBufferColIndex*3 + 2]; writeIndex++;//transverse  the RGB --> BGR
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width + readBufferColIndex*3 + 1]; writeIndex++;
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width + readBufferColIndex*3 + 0]; writeIndex++;
      }
      for (unsigned int j=0;j<extrabytes;j++)
      {
        writeBuffer[writeIndex]=0;writeIndex++;//add padding
      }
      if(DEBUG) cerr << "finished one iteration! \n";
    }
    if (writeIndex!=writeBufferSize-1) cerr << "YIPES!  writeIndex!=writeBufferSize-1:  " << writeIndex << "!=" << writeBufferSize << endl;
    fwrite(writeBuffer,writeBufferSize,sizeof(unsigned char),bmpFilep); cerr << "we just wrote a buffer\n";
    if(DEBUG)cerr << "cursor is " << (unsigned long)cursor << "\n";
    cursor -= readBufferSize;

  }

  writeIndex=0;
  cursor=8;
  fsetpos(nrdFilep,&cursor);
  rowsInReadBuffer=smallBufferSize/(width*3);
  if (smallBufferSize>0)
  {
    cerr << largeBuffersNeeded << endl;
    fread(readBuffer,smallBufferSize,sizeof(unsigned char),nrdFilep);
    if(DEBUG) cerr << "loaded the readBuffer and fileIndex is: " << fileIndex << "\n";
    for (readBufferRowIndex=rowsInReadBuffer-1;readBufferRowIndex>0;readBufferRowIndex--)// loop through rows from last to first
    {
      if(DEBUG) cerr << "starting top loop, row index: " << readBufferRowIndex << "\n";
      for (readBufferColIndex=0;readBufferColIndex<width;readBufferColIndex++) //loop through cols from first to last within each row.
      {
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width + readBufferColIndex*3 + 2]; writeIndex++;//transverse  the RGB --> BGR
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width + readBufferColIndex*3 + 1]; writeIndex++;
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width + readBufferColIndex*3 + 0]; writeIndex++;
      }
      for (unsigned int j=0;j<extrabytes;j++)
      {
        writeBuffer[writeIndex]=0;writeIndex++;//add padding
      }
      if(DEBUG) cerr << "finished one iteration! \n";
    }
    fwrite(writeBuffer,smallBufferSize+rowsInReadBuffer*extrabytes,sizeof(unsigned char),bmpFilep);
    if(DEBUG)cerr << "cursor is " << (unsigned long)cursor << "\n";
  }



  free(writeBuffer);
  free(readBuffer);
  fclose(bmpFilep);  // Close file
  fclose(nrdFilep);
  return TRUE;

//bail: ;//goto bail will cause program to kill itself without doing damage
}


/*

  cerr << "welcome to the loop!";
    oldPosition=position;
    position -= bufferSize;
    if (position<8) position=8; //changed 8's to 7's
    blockSize = oldPosition-position;
    cursor=position;
    fsetpos(nrdFilep,&cursor);
    fread(buffer,blockSize,sizeof(unsigned char),nrdFilep);
    cerr << "HI! FOR LOOPS ROCK!";
    for (int row=blockSize/(3*width);row>0;row--)
    {
      cerr << "HI! NESTED FOR LOOPS ROCK!  " << row;
      for (unsigned int col=0; col<width;col++)
      {
      //  cerr << "HI! DOUBLE NESTED ROOPS ARE NEAT!  " << bufferIndex;
        putc(buffer[bufferIndex],bmpFilep);bufferIndex--;
        putc(buffer[bufferIndex],bmpFilep);bufferIndex--;
        putc(buffer[bufferIndex],bmpFilep);bufferIndex+=5;
      }
      for (unsigned int i=0;i<extrabytes;i++) putc(0,bmpFilep);
    }
    cerr << "++ on the buffer!";
    */