#include <iostream.h>
#include <fstream.h>
#include "bitmapwriter.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "functions.h"
#include <string.h>
//#include <string>
#include <sstream>

#define TOLERANCE .01
#define INFINITY 100000000000000 //10000000000000000000
//changed max its to a variable to allow runtime modification
/*
#define MAXITS 100
*/
//computer doesnt agree that its such a hot idea so ill try again later
const int MAXITS = 100;

#define PI 3.1415926535897932384626433832795028841971693993751
#define NATHANFLAG false
#define SLOWMODE true


  double (*aFunction)        (double, double);
  double (*bFunction)        (double, double);

nrdRGB juliaTest(double, double, double, double);

//added to avoid monotonous recompile. number represents scale factor for arccos directed
//greyscale application
int cosScale = 255;
//added to avoid monotonous recompile. allows choice between grey scale and and contrast
bool grayScale = false;


void main(int argc, char *argv[])
{

  if(NATHANFLAG){cerr << "pow(10000000000000000000, 2) is " << pow(10000000000000000000, 2)
    << endl;}
  if(NATHANFLAG){cerr << "abs of -1 is " << abs( -1 ) << endl;}
  if(NATHANFLAG){cerr << "abs of .98 - 1.34 is " << abs( .98 - 1.34 ) << endl;}
  //set gray scale on or off and adjust scale on gray scale application
  //grayScale = argv[2];
  //cosScale = argc;
  //once again getting beat on by c++

  int pixelWidth, pixelHeight;
  double Ca, Cb;
  double factor;
  double Xmax;
  char functionType;
  nrdRGB * nrdBuffer;
  unsigned long buffIndex=0;
  int row, col;
  //int grayOrNot=0;


  cout << "Desired Image Width:  ";
  cin >> pixelWidth;
  //cout << "Desired Image Height:  ";
  //cin >> pixelHeight;
  pixelHeight= pixelWidth;
  cout << "Re(c):  ";
  cin >> Ca;
  cout << "Im(c):  ";
  cin >> Cb;
  cout << "X bound:  +/- ";
  cin >> Xmax;
  /*let the spaghetti code begin
  grayOnOff:
  cout << "Gray scale on <1> or off <0> ?  ";
  cin >> grayOrNot;
  if( grayOrNot == 1 )
  {
    grayScale = true;
  }

  else if( grayOrNot != 0 )
  {
    cerr << "invalid gray scale choice\n";
    goto grayOnOff;
  }
  */
  cout << "Enter gray scale scaling factor between 0 and 255 inclusive  ";
  cin >> cosScale;

  chooseFunc:
  cout << "Function to iterate (2,3,7, or e):  ";
  cin >> functionType;

  unsigned long bufferSize=(52428800-(52428800%(3*pixelWidth)))/3; //make sure the buffer = 0 (mod row*3)
  nrdBuffer = (nrdRGB *) calloc(bufferSize,sizeof(nrdRGB));
  cerr << sizeof(nrdRGB) << "\n";
  if (nrdBuffer==NULL)
  {
    cerr << "I need 50 megs to run.  Please free up some memory.";
    return;
  }
  //cerr << "Let's get the show on the road, boys!\n";

  switch(functionType)
  {
  case '2':
    aFunction=aSquare;
    bFunction=bSquare;
    break;
  case '3':
    aFunction=aCube;
    bFunction=bCube;
    break;
  case '7':
    aFunction=aHepta;
    bFunction=bHepta;
    break;
  case 'e':
    aFunction=ae;
    bFunction=be;
    break;
  default:
    cerr << "Invalid choice of functions.\n\n";
    goto chooseFunc;
  }

  factor = 2*Xmax/pixelWidth;

  cerr << "we are about to start the construction of the nrd FILE\n";
  FILE *output;
  output = fopen("temp.nrd","wb");
  fwrite(&pixelHeight,4,1,output);
  fwrite(&pixelWidth,4,1,output);
  cerr << "we passed the fwrite";


  for (row=0; row<pixelHeight; row++)
  {
    for (col=0; col<pixelWidth; col++)
    {
      double Za=((double)col-(double)pixelWidth/2)*factor;
      if(NATHANFLAG){cerr << "Za is " << Za << endl;}
      double Zb=((double)row-(double)pixelHeight/2)*-factor;
      if(NATHANFLAG){cerr << "Zb is " << Zb << endl;}
      nrdBuffer[buffIndex]= juliaTest(Za,Zb, Ca, Cb); buffIndex++;//RGB value;
    }
    if (buffIndex>=bufferSize / 1)//changing /3 to /1 :), it used to be 3 because
    {
      cerr << " bufferIndex is >= bufferSize; ie a buffer has been"
          <<" filled during nrd file creation\n";
      fwrite(nrdBuffer,buffIndex,sizeof(nrdRGB),output);
      buffIndex=0;
    }
  }
  fwrite(nrdBuffer,buffIndex,sizeof(nrdRGB),output);

  fclose(output);
  free(nrdBuffer);

  //build a output file name based on input parameters
  cerr<< "\nabout to start the badness";
  string out;
  char * outfile;
  //cerr << l << "hello";
  if( argv[1]==NULL )
  {
    cerr<<"\n it has begun";
    //Stringsream allows srings to be manipulated like strings
    //still think java is easier
    stringstream theSS;
    //concatination isnt all it could be
    theSS << "P_" << pixelWidth;
    theSS << ".R_";
    theSS << Ca;
    theSS << ".I_" ;
    theSS << Cb;
    theSS << ".W_" ;
    theSS << Xmax;
    theSS << ".F_" << functionType;
    theSS <<  ".bmp";
    out=theSS.str();
    //convert string to char*
    outfile = &out[0];
    cerr << outfile;
  }

  //user entered a outputfile so lets use it
  else{ outfile = argv[1]; }


  nrd2bmp("temp.nrd", outfile);
  //silent mode!!!!!!!
  //cerr << "\a";
  /*
  system(argv[1]);
  */
  //tells the system to open the newly created bmp file but seems to
  //cause serious problems (full system halt!!!)
}



nrdRGB juliaTest(double Za, double Zb, double Ca, double Cb)
{
  nrdRGB returnValue(0,0,0);
  double aHistory[MAXITS+1];
  double bHistory[MAXITS+1];
  aHistory[0]=Za;
  bHistory[0]=Zb;
  long i=0;
  //while the abs(Z) < "infinity" && we're still iterating
  //note that both sides of inequality are squared for efficiency
  while (( pow(Za,2) + pow(Zb,2) <= pow(INFINITY,2)) && (i<MAXITS))//the square of infinity ¶:)
  {
    i++;
    aHistory[i] = aFunction(Za,Zb) + Ca;
    bHistory[i] = bFunction(Za,Zb) + Cb;
    Za = aHistory[i];
    Zb = bHistory[i];

    //bool slowMode = false;
    if(SLOWMODE)
    {
      for (int j=0;j<i;j++)
      {
        if(false){cerr << "checking for stable point, " << i << ", " << j << endl;}
        //if ((Za==aHistory[j])&&(Zb==bHistory[i]))
        if ( ( fabs((float)(Za - aHistory[j])) < TOLERANCE )
          && ( fabs((float)(Zb - bHistory[i])) < TOLERANCE) )
        {
          //cerr << "found a stable point, orbit value is " << j << endl;
          returnValue.R=255;
          returnValue.G=(35*j)%256;   //maybe this will be alittle more interesting
          returnValue.B=0;
          return returnValue;
        }
      }
    }
    //this was pretty stupid
    /*
    else
    {
      returnValue.R=255;
      returnValue.G=0;
      returnValue.B=0;
      return returnValue;
    }
    */
  }

  if (i<MAXITS)
  {
    if( grayScale ){returnValue.R
      =( (((unsigned char)( acos((double)(MAXITS-2*i)/MAXITS)*cosScale/(PI))*25 )% 256 ) );}
    else{returnValue.R = 0;}
    returnValue.G=returnValue.R;
    returnValue.B=returnValue.R;
    return returnValue;
  }
  else
  {
    returnValue.R=255;
    returnValue.G=(10*i)%256;
    returnValue.B=0;
    return returnValue;
  }
}





/*
nrdRGB juliaTest(double Za, double Zb, double Ca, double Cb)
{

  double i=0;
  while (( pow(Za,2) + pow(Zb,2) <= pow(INFINITY,2)) && (i<MAXITS))   //while the abs(Z) < "infinity" && we're still iterating
  {                                  //(note that both sides of inequality are squared for
    i++;                              //efficiency.)
    double a = aFunction(Za,Zb) + Ca;
    double b = bFunction(Za,Zb) + Cb;
    Za = a;
    Zb = b;
  }

  nrdRGB returnWhite(255,255,255);
  nrdRGB returnBlack(0,0,0);

  if (i==MAXITS) return returnWhite;
  else return returnBlack;
}
*/