#include <iostream.h>
#include <fstream.h>
#include "bitmapwriter.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "functions.h"


#define INFINITY 10000000000000000000
#define MAXITS 100
#define PI 3.1415926535897932384626433832795028841971693993751

  double (*aFunction)        (double, double);
  double (*bFunction)        (double, double);

nrdRGB juliaTest(double, double, double, double);

void main(int argc, char *argv[])
{

  int pixelWidth, pixelHeight;
  double Ca, Cb;
  double factor;
  double Xmax;
  char functionType;
  nrdRGB * nrdBuffer;
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

  unsigned long bufferSize=(52428800-(52428800%(3*pixelWidth)))/3; //make sure the buffer = 0 (mod row*3)
  nrdBuffer = (nrdRGB *) calloc(bufferSize,sizeof(nrdRGB));
  cerr << sizeof(nrdRGB) << "\n";
  if (nrdBuffer==NULL)
  {
    cerr << "I need 50 megs to run.  Please free up some memory.";
    return;
  }
  cerr << "Let's get the show on the road, boys!";

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

  cerr << "we's about to start the FILE shit";
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
      double Zb=((double)row-(double)pixelHeight/2)*-factor;
      nrdBuffer[buffIndex]= juliaTest(Za,Zb, Ca, Cb); buffIndex++;//RGB value;
    }
    if (buffIndex>=bufferSize)//changing /3 to /1 :), it used to be 3 because
    {
      cerr << "We's in the if(buffIndex >= shit";
      fwrite(nrdBuffer,buffIndex,sizeof(nrdRGB),output);
      buffIndex=0;
    }
  }
  fwrite(nrdBuffer,buffIndex,sizeof(nrdRGB),output);

  fclose(output);
  free(nrdBuffer);
  nrd2bmp("temp.nrd", argv[1]);
  cerr << "\a";
  system(argv[1]);
}



nrdRGB juliaTest(double Za, double Zb, double Ca, double Cb)
{
  nrdRGB returnValue(0,0,0);
  double aHistory[MAXITS+1];
  double bHistory[MAXITS+1];
  aHistory[0]=Za;
  bHistory[0]=Zb;
  long i=0;
  while (( pow(Za,2) + pow(Zb,2) <= pow(INFINITY,2)) && (i<MAXITS))   //while the abs(Z) < "infinity" && we're still iterating
  {                                //(note that both sides of inequality are squared for
    i++;                              //efficiency.)
    aHistory[i] = aFunction(Za,Zb) + Ca;
    bHistory[i] = bFunction(Za,Zb) + Cb;
    Za = aHistory[i];
    Zb = bHistory[i];
    for (int j=0;j<i;j++)
    {
      if ((Za==aHistory[j])&&(Zb==bHistory[i]))
      {
        returnValue.R=255;
        returnValue.G=j;
        returnValue.B=0;
        return returnValue;
      }
    }
  }

  if (i<MAXITS)
  {
    returnValue.R=(unsigned char)acos(((double)(MAXITS-2*i)/MAXITS))*255/(PI);
    returnValue.G=returnValue.R;
    returnValue.B=returnValue.R;
    return returnValue;
  }
  else
  {
    returnValue.R=255;
    returnValue.G=255;
    returnValue.B=255;
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