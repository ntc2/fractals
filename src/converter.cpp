#include <iostream.h>
#include <fstream.h>
#include "bitmapwriter.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

//#define cerr cout
//note only output sent to cout can be piped from the commad line

void main( int argc, char * argv[] )
{
  if( argc != 4 )
  {
    cerr << "usage: converter.exe <nrd file to convert> <bmp file to write to> "
      << "<1 for open in viewer 0 otherwise>\n";
    return;
  }

  cerr << "going to convert " << argv[1] << " and save as " << argv[2] << endl;

  nrd2bmp( argv[1], argv[2] );

  cerr << "conversion complete\n";

  //and where whould we be without the microwave bell
  cerr << "\a";
  ///////////////////////////////////////////////////////////////
  //int  strcmp ( const char * string1, const char * string2 );//
  ///////////////////////////////////////////////////////////////
  //Compare two strings.
  //Compares string1 to string2 character by character.
  //This function starts comparing the first character of each
  //string. If they are equal to each other continues with the
  //following pair until the characters differ or until end of
  //string is reached.
  //returns 0 if string one is lexigraphically equal to string two
  if( !strcmp( argv[3], "1") )  //if( argv[3] == "1")
  {
    cerr << "opening " << argv[2] << " in viewer\n";
    system( argv[2] );
  }
}//main