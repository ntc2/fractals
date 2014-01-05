#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <iostream>

using namespace std;

#define TWENTYFOURMB 50000000 //25165824
#define DEBUG true
#define COUNT false
#define NRDHEADERSIZE 8
//#define cerr cout

int nrd2bmp(char *nrdFilename, char *bmpFilename)
{
  //cerr << "HI!";
  unsigned int width,
    height;
  BITMAPINFOHEADER bmpInfoHeader;
  BITMAPFILEHEADER bmpFileHeader;
  //create pointers to be used in streaming of files for reading
  FILE *bmpFilep;
  FILE *nrdFilep;
  //unsigned int row, column;
  unsigned int extrabytes, bytesize;
  //long position, oldPosition, blockSize, bufferSize;
  fpos_t cursor;
  unsigned char * readBuffer, * writeBuffer;
  //unsigned long bufferIndex=2;
  unsigned long nrdFilesize;

  /* The .bmp format requires that the image data is aligned on a 4 byte boundary.  For 24 - bit bitmaps,
     this means that the width of the bitmap  * 3 must be a multiple of 4. This code determines
     the extra padding needed to meet this requirement. */
  ////////////////////////////////////////////////////////////
  //FILE * fopen (const char * filename, const char * mode);//
  ////////////////////////////////////////////////////////////
  //Open a file.
  //Opens the file which name is stored in the filename string
  //and returns a pointer to the file (stream). Operations
  //allowed to the file returned are defined by the mode parameter.
  ////////////////////////////////////////////////////////////
  //the mode rb is:
  //r = Open a file for reading. The file must exist.
  //b = Binary mode. End of file is reached at last byte of the file.
  //No conversions
  nrdFilep = fopen(nrdFilename,"rb");
  //fread() is explained later on
  fread(&height,4,1,nrdFilep);
  fread(&width,4,1,nrdFilep);

  //initialized to zero on tip from david
  //before it was uninitialized and possibly starting on bogus values
  long writeIndex = 0;
  long readBufferSize = TWENTYFOURMB - (TWENTYFOURMB%(3*width)); //set it just under 24 megs memory, and also evenly divisible by row of RGB pixels (3*width)

  if(DEBUG) cerr << "readBufferSize: " << readBufferSize << "\n";

  extrabytes = (4 - (width * 3) % 4) % 4;
  long rowsInReadBuffer=readBufferSize/(3*width);
  long writeBufferSize = readBufferSize+extrabytes*rowsInReadBuffer; //addition is for padding.
  nrdFilesize= 8 + 3*width*height;
  long largeBuffersNeeded=nrdFilesize/readBufferSize;

  if(DEBUG) cerr << "largeBuffersNeeded are " << largeBuffersNeeded << endl;

  long smallBufferSize = nrdFilesize - largeBuffersNeeded*readBufferSize - 8; //the minus 8 is to account for the first 8 int spaces

  //superficial variable
  long fileIndex = nrdFilesize;
  //changed to long to avoid "roll over" error on decrement
  //david caught error
  long readBufferRowIndex = rowsInReadBuffer; //start at end...will proceed to beginning...
  long readBufferColIndex = 0;


  ////////////////////////////////////////////////////////////////////////////////
  //*********************ALLOCATE**BUFFERS***********************************// //
  ///////////////////////////////////////////////////////////////////////////// //
  readBuffer = (unsigned char *) calloc(readBufferSize, sizeof(unsigned char)); //calloc(how many elements, size of elements);
  if (readBuffer==NULL)                              //
  {                                          //
    cerr << "No mo' memory for readBuffer.\n";                                //
    return false;                                //
  }                                        //
  writeBuffer = (unsigned char *) calloc(writeBufferSize, sizeof(unsigned char));//
  if (writeBuffer==NULL)                              //
  {                                        //
    cerr << "No mo' memory for writeBuffer.\n";                  //
    return false;                                  //
  }                                        //
  ////////////////////////////////////////////////////////////////////////////////
  if (DEBUG) cerr << "all variables declared okay\n";
  if (DEBUG) cerr << "size of unsigned char is " << sizeof(unsigned char) << endl;

  // This is the size of the padded bitmap
  bytesize = (width * 3 + extrabytes) * height;

  //this is windows.h stuff
  // Fill the bitmap file header structure
  bmpFileHeader.bfType = 'BM';   // Bitmap header
  bmpFileHeader.bfSize = 0;      // This can be 0 for BI_RGB bitmaps
  bmpFileHeader.bfReserved1 = 0;
  bmpFileHeader.bfReserved2 = 0;
  bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  //this is windows.h stuff
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


  /*
  cursor=nrdFilesize;
  */

  ////////////////////////////////////////////////////////////
  //int  fseek ( FILE * stream , long offset , int origin );//
  ////////////////////////////////////////////////////////////
  //Reposition stream's position indicator.
  //Sets the position indicator associated with the stream to
  //a new position defined by an offset from the location given by the origin parameter.
  //possible origins are:
  //Initial position from where offset is applied.
  //It can be any of these constants defined in stdio.h:
  //SEEK_SET (0) Beginning of file.
  //SEEK_CUR (1) Current position of the file pointer.
  //SEEK_END (2) End of file
  fseek( nrdFilep, 0, SEEK_END );

  //long ftell(FILE * stream)
  //returns the current position of the pointer in the FILE called stream
  if(DEBUG) cerr << "end of file is at " << ftell( nrdFilep ) << " according to ftell\n";

  //if (DEBUG) printf("cursor starting out at: %d \n",(unsigned long)cursor);
  //if (DEBUG) printf("cursor starting out at: %d \n",(unsigned long)cursor);

  for (unsigned int i=0;i<largeBuffersNeeded; i++)
  {
    if(DEBUG) cerr << "cursor is (top): " << ftell( nrdFilep ) << endl;//(unsigned long)cursor << "\n";

    //move back to read position
    /*
    cursor -= readBufferSize;
    */
    //move the pointer in stream nrdFilep back readBufferSize bytes from current position
    fseek( nrdFilep, -readBufferSize, SEEK_CUR );
    //set the current value for future return with fsetpos
    //int  fgetpos (FILE * stream , fpos_t * position);
    fgetpos( nrdFilep, &cursor );

    if(DEBUG) cerr << "cursor got decremented by readBufferSize and is:  "
      << cursor << "\n";

    //not sure if this is right???
    //because there was not a previous call to fgetpos before i added it
    /*
    fsetpos(nrdFilep,&cursor);
    */

    /////////////////////////////////////////////////////////////////////////
    //int  fread (void * buffer, size_t size, size_t count, FILE * stream);//
    /////////////////////////////////////////////////////////////////////////
    //Read block of data from a stream.
    //Read count number of items each one with a size of size bytes from the
    //stream and stores it in the specified buffer.
    //Stream's postion indicator is increased by the number of bytes readed.
    //Total amount of bytes read is (size x count).
    /////////////////////////////////////////////////////////////////////////
    //Return Value.
    //The total number of items readed is returned.
    //If this number differs from the requested amount (count parameter)
    //an error has occured or End Of File has been reached. To determine
    //what happened call feof or ferror.
    if( (fread(readBuffer, sizeof(unsigned char), readBufferSize, nrdFilep)) != readBufferSize )
    {
      cerr << "FATAL ERROR HAS OCCURRED: there were fewer bytes in the nrd file then\n"
        << "requested by the filereader (fread())";
    }
    cerr << "The readbuffer has just been written\n";

    if(DEBUG) cerr << "loaded the readBuffer and fileIndex is: " << fileIndex << "\n";

    // loop through rows from last to first
    //changed loop condition from readBufferRowIndex>0 to readBufferRowIndex>=0
    for (readBufferRowIndex=rowsInReadBuffer-1;readBufferRowIndex>=0;readBufferRowIndex--)
    {
      if(DEBUG && COUNT) cerr << "starting top loop, row index: " << readBufferRowIndex << "\n";
      //loop through cols from first to last within each row.
      for (readBufferColIndex=0;readBufferColIndex<width;readBufferColIndex++)
      {
        if(DEBUG && COUNT)
        {
          if( (readBufferRowIndex*3*width + readBufferColIndex*3 + 2)%1000 == 0 )
          {
            cerr << "trying to assign: "
            << readBufferRowIndex*3*width + readBufferColIndex*3 + 2 << "\n";
          }
        }
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width
          + readBufferColIndex*3 + 2]; writeIndex++;//transverse  the RGB --> BGR
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width
          + readBufferColIndex*3 + 1]; writeIndex++;
        writeBuffer[writeIndex]=readBuffer[readBufferRowIndex*3*width
          + readBufferColIndex*3 + 0]; writeIndex++;
      }

      //makes padding out put a little more reasonable
      bool padBool = true;
      for (unsigned int j=0;j<extrabytes;j++)
      {
        if(DEBUG && COUNT && padBool) cerr << "adding padding\n";
        writeBuffer[writeIndex]=0;
        writeIndex++;//add padding
        padBool = false;  //one shout out from padding loop is enough!
      }
      if(DEBUG && COUNT) cerr << "filled a buffer time to start over! \n";
    }

    //might aid comparison
    long first = (writeIndex);
    long second = (writeBufferSize - 1);

    //this is silly. it shoud be eqaul but not minus 1.  one an index another is a size
    //change later if care
    if ( first != second ) cerr << "YIPES!  writeIndex!=writeBufferSize-1:  "
      << first << "!=" << second << endl;

    //fwrite is explained below
    fwrite(writeBuffer,sizeof(unsigned char),writeBufferSize,bmpFilep);
    cerr << "we just wrote a buffer of size " << writeBufferSize << " bytes\n";
    cerr << "it was the " << i << " buffer of " << largeBuffersNeeded << endl;
    if(DEBUG)cerr << "cursor is <bottom> " << (unsigned long)cursor << "\n";


    fileIndex -= writeBufferSize;
    ///////////////////////////////////////////////////////////////
    //int  fsetpos ( FILE * stream , const   fpos_t * position );//
    ///////////////////////////////////////////////////////////////
    //Reposition file pointer to a saved location.
    //Sets the file pointer associated with stream to a new position.
    //The position parameter is a value previously obtained by a call to fgetpos.
    //After a call to this function, the End-Of-File indicator of the
    //stream is cleared and any effect of a previous call to ungetc is undone.
    //The next operation with the stream after a call to fsetpos can be
    //either for input or output.
    ///////////////////////////////////////////////////////////////
    //data was read and thus cursor was moved so it must be moved
    //back for the cockeyed backwards file reading
    fsetpos( nrdFilep, &cursor );
    //this could be the culprit
    /*
    cursor -= readBufferSize;
    */

    //ithink this is the problem.  the old writes were to out of bounds on second loop
    writeIndex=0;
  }

  writeIndex=0;
  /*
  cursor=8;
  */
  fseek( nrdFilep, NRDHEADERSIZE, SEEK_SET );

  /*
  fsetpos(nrdFilep,&cursor);
  */
  rowsInReadBuffer=smallBufferSize/(width*3);
  if (smallBufferSize>0)
  {
    cerr << largeBuffersNeeded << endl;
    fread(readBuffer,sizeof(unsigned char),smallBufferSize,nrdFilep);
    if(DEBUG) cerr << "loaded the readBuffer and fileIndex is: " << fileIndex << "\n";
    //changed loop condition from readBufferRowIndex>0 to readBufferRowIndex>=0
    for (readBufferRowIndex=rowsInReadBuffer-1;readBufferRowIndex>=0;readBufferRowIndex--)// loop through rows from last to first
    {
      if(DEBUG && COUNT) cerr << "starting top loop, row index: " << readBufferRowIndex << "\n";
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
      if(DEBUG && COUNT) cerr << "finished one iteration! \n";
    }
    /////////////////////////////////////////////////////////////////////////////////////
    //size_t  fwrite ( const void * buffer, size_t size, size_t count, FILE * stream );//
    /////////////////////////////////////////////////////////////////////////////////////
    //Write a block of data to a stream.
    //Writes count number of items, each one with a size of size bytes, from the memory
    //block pointed by buffer to the current position in the stream.
    //Stream's postion indicator is increased by the number of bytes written.
    //On files opened in text mode some translations may occur with carriage-return
    //and line-feed characters.
    //The total number of bytes to be written is (size x count). Normally the parameter
    //size should contain the size of each item (char, int, long, structures ...) to be
    //written and count the number of these items. But this is not absolute, you can
    //specify any combination of numbers which result of (size x count) match the size
    //in bytes of the block to be written.
    /////////////////////////////////////////////////////////////////////////////////////
    //Return Value.
    //Number of full items (ie count, not total bytes) successfully written. This may be less than the
    //specified in count parameter if an error occurred.
    if( (fwrite(writeBuffer, sizeof(unsigned char), smallBufferSize+rowsInReadBuffer*extrabytes,
      bmpFilep)) != smallBufferSize+rowsInReadBuffer*extrabytes )
      {
        cerr << "FATAL ERROR HAS OCCURRED: data written by fwrite() is not equal to that requested\n";
        //<< "requested by the filereader (fread())";
      }

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
