#include <math.h>
#include <iostream.h>
#include <fstream.h>
#include "bitmapwriter.h"
#include <stdlib.h>
#include <time.h>
#include "functions.h"


#define INFINITY 10000000000000000000
#define MAXITS 100
#define LOWRANGE 0    //low value for interesting range of divergence iteration values
#define HIGHRANGE 60  //high value for interesting range of divergence iteration values
#define FUNCTYPE 'l'  //function type to be used for coloring
#define PI 3.1415926535897932384626433832795028841971693993751

  double (*aFunction)        (double, double);
  double (*bFunction)        (double, double);

int juliaTest(double, double, double, double);

int color(char funcType, int lowRange, int highRange, int numberOfIts );//function to assign color values based
                                    //on divergence iteration rate
int linearC( int numberOfIts, int lowRange, int highRange );      //|
int logarithemicC( int numberOfIts, int lowRange, int highRange );    //|_coloring helper functions
int exponentialC( int numberOfIts, int lowRange, int highRange );    //|
int arccosC( int numberOfIts, int lowRange, int highRange );      //|

void main(int argc, char *argv[])
{
  int pixelWidth, pixelHeight;
  double Ca, Cb;
  double factor;
  double Xmax;
  char functionType;
  unsigned char * nrdBuffer;
  unsigned long buffIndex=0;
  int row, col;


  cout << "Desired Image Width:  ";
  cin >> pixelWidth;
  cout << "Desired Image Height:  ";
  cin >> pixelHeight;
  cout << "Re(c):  ";
  cin >> Ca;
  cout << "Im(c):  ";
  cin >> Cb;
  cout << "X bound:  +/- ";
  cin >> Xmax;
  chooseFunc:
  cout << "Function to iterate (2,3,7, or e):  ";
  cin >> functionType;

  unsigned long bufferSize=52428800-(52428800%(3*pixelWidth)); //make sure the buffer = 0 (mod row*3)
  nrdBuffer = (unsigned char *) calloc(bufferSize,sizeof(unsigned char));
  if (nrdBuffer==NULL)
  {
    cerr << "I need 50 megs to run.  Please free up some memory.";
    return;
  }

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
    //cerr << "Invalid choice of functions.\n\n";
    //goto chooseFunc;
    cmNum theCmNum(
    break;
  }

  factor = 2*Xmax/pixelWidth;

  FILE *output;
  output = fopen("temp.nrd","wb");
  fwrite(&pixelHeight,4,1,output);
  fwrite(&pixelWidth,4,1,output);


  for (row=0; row<pixelHeight; row++)
  {
    for (col=0; col<pixelWidth; col++)
    {
      double Za=((double)col-(double)pixelWidth/2)*factor;
      double Zb=((double)row-(double)pixelHeight/2)*factor;
      nrdBuffer[buffIndex]= juliaTest(Za,Zb, Ca, Cb); buffIndex++;//red value
      nrdBuffer[buffIndex]= nrdBuffer[buffIndex-1]; buffIndex++;//green value
      nrdBuffer[buffIndex]= nrdBuffer[buffIndex-1]; buffIndex++;//blue value
    }
    if (buffIndex>=bufferSize)
    {
      fwrite(nrdBuffer,buffIndex,sizeof(unsigned char),output);
      buffIndex=0;
    }
  }
  fwrite(nrdBuffer,buffIndex,sizeof(unsigned char),output);

  fclose(output);
  free(nrdBuffer);
  nrd2bmp("temp.nrd", argv[1]);
  cerr << "\a";
  system(argv[1]);
}



int juliaTest(double Za, double Zb, double Ca, double Cb)
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

  /*if (i<MAXITS)
  {

  }
  else return 255;
  */
  return color('e', LOWRANGE, HIGHRANGE, i);
}

int color(char funcType, int lowRange, int highRange, int numberOfIts ){
  if( numberOfIts <= lowRange ){
    return 0;    //it diverges to quickly so make it black
  }
  if( numberOfIts >= highRange ){
    return 255;    //it takes to long so it might as well be orbital, i.e. make it white
  }

  switch(funcType)  //figure out what kind of function to use to gradiate
  {
  case 'l':  //linear
    return linearC( numberOfIts, lowRange, highRange );
    break;
  case 'g':  //logarithemic
    return logarithemicC( numberOfIts, lowRange, highRange );
    break;
  case 'e':  //exponential
    return exponentialC( numberOfIts, lowRange, highRange );
    break;
  case 'c':  //arccos
    return arccosC( numberOfIts, lowRange, highRange );
    break;
  default:
    return 255;  //no valid color function type was specified so make em all black (only devergers make it this far)
    break;
  }



  //return 2;  //not 5, because then the code would be bad
}//color

int linearC( int numberOfIts, int lowRange, int highRange ){
  return ((numberOfIts - lowRange)/(lowRange - highRange));
}

int logarithemicC( int numberOfIts, int lowRange, int highRange ){
  double vertPos = log(  (  ((double)numberOfIts - (double)lowRange)/((double)highRange - (double)lowRange)  )+.1   );
  double scale = (vertPos - log((double).1))/log((double)1.1) - log((double)0.1);
  return scale*255;
}

int exponentialC( int numberOfIts, int lowRange, int highRange ){
  double horzPost = ((double)(numberOfIts - lowRange))/((double)(highRange-lowRange));
  double vertPost = exp((double)(horzPost-log((double)0.1))/(log((double)2)-log((double).1)));
  double scale = (vertPost - .1)/((double)1.9); //2 - .1 = 1.9
  return scale*255;
}
int arccosC( int numberOfIts, int lowRange, int highRange ){
  double scale = acos(  1-2*(  ((double)numberOfIts-(double)lowRange)/((double)(highRange - lowRange)  ))  );
  return scale*255/(PI);
}



/*
int juliaTest(double Za, double Zb, double Ca, double Cb)
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

  if (i<MAXITS)
  {
    if (i>=RANGE) return 255;
    else return acos(((double)(RANGE-2*i)/RANGE))*255/(2*PI);
  }
  else return 255;
}*/