#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "functions.h"
#include <string.h>
//#include <string>
#include <sstream>
#include <png++/png.hpp>

using namespace std;

// #define TOLERANCE .001
double tolerance;
#define INFINITY 100000000000000 //10000000000000000000
//changed max its to a variable to allow runtime modification
/*
#define MAXITS 100
*/
//computer doesnt agree that its such a hot idea so ill try again later
//const int MAXITS = 100;
int maxIts;

#define PI 3.1415926535897932384626433832795028841971693993751
#define NATHANFLAG false
#define SLOWMODE true

double (*aFunction) (double, double);
double (*bFunction) (double, double);
int (*testFunction) (double, double *, double, double *, int, double);

png::rgb_pixel juliaTest(double, double, double, double);

void usage(char *argv[]) {
  fprintf(stderr,
          "usage: %s WIDTH_PIXELS REAL0 IMAG0 MAX_REAL_ABS FUN TEST TOLERANCE ITS\n\n"
          "- Pixel height of image is the same a given width, WIDTH_PIXELS.\n"
          "- The REAL0 and IMAG0 define the constant 'c' in 'z |-> f(z) + c'.\n"
          "- The MAX_REAL_ABS determines the horizontal range of the picture;\n"
          "  the vertical range is chosen to be the same and the picture is\n"
          "  centered at the origin.\n"
          "- The FUN determines 'f' in 'z |-> f(z) + c; the choices are\n"
          "  2, 3, 5, 7, or e.\n"
          "- The TEST is 'a' or 'o' and determines how recurrence is checked;\n"
          "  with 'a' (and) both coordinates must recur and with 'o' (or) only one.\n"
          "- The TOLERANCE determines when two coordinates are close enough\n"
          "  to be considered recurrent. Tested values are 0.01 and 0.001.\n"
          "- The ITS is the max number of iterations to try before deciding\n"
          "  the current point does not escape to infinity. Tested value is 100.\n",
          argv[0]);
  exit(2);
}

//added to avoid monotonous recompile. number represents scale factor for arccos directed
//greyscale application
int cosScale = 255;
//added to avoid monotonous recompile. allows choice between grey scale and and contrast
bool grayScale = false;


int main(int argc, char *argv[])
{

  if(NATHANFLAG){cerr << "pow(10000000000000000000, 2) is " << pow(10000000000000000000, 2)
    << endl;}
  if(NATHANFLAG){cerr << "abs of -1 is " << abs( -1 ) << endl;}
  if(NATHANFLAG){cerr << "abs of .98 - 1.34 is " << fabs( .98 - 1.34 ) << endl;}
  //set gray scale on or off and adjust scale on gray scale application
  //grayScale = argv[2];
  //cosScale = argc;
  //once again getting beat on by c++

  int pixelWidth, pixelHeight;
  double Ca, Cb;
  double factor;
  double Xmax;
  char functionType;
  int row, col;
  char testType;
  //int grayOrNot=0;

  if (argc != 9) {
    fprintf(stderr, "Wrong number of arguments!\n\n");
    usage(argv);
  }
  sscanf(argv[1], "%d", &pixelWidth);
  pixelHeight= pixelWidth;
  sscanf(argv[2], "%lf", &Ca);
  sscanf(argv[3], "%lf", &Cb);
  sscanf(argv[4], "%lf", &Xmax);
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
  sscanf(argv[5], "%c", &functionType);
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
    usage(argv);
  }

  sscanf(argv[6], "%c", &testType);
  switch(testType)
  {
  case 'a':
    testFunction = andTest;
    break;
  case 'o':
    testFunction = orTest;
    break;
  default:
    fprintf(stderr, "Invalid TEST: %c\n", testType);
    usage(argv);
  }

  sscanf(argv[7], "%lf", &tolerance);
  sscanf(argv[8], "%d", &maxIts);

  png::image< png::rgb_pixel > image(pixelWidth, pixelHeight);

  factor = 2*Xmax/pixelWidth;

  for (row=0; row<pixelHeight; row++)
  {
    for (col=0; col<pixelWidth; col++)
    {
      double Za=((double)col-(double)pixelWidth/2)*factor;
      if(NATHANFLAG){cerr << "Za is " << Za << endl;}
      double Zb=((double)row-(double)pixelHeight/2)*-factor;
      if(NATHANFLAG){cerr << "Zb is " << Zb << endl;}
      image[row][col] = juliaTest(Za, Zb, Ca, Cb);
    }
  }

  //build a output file name based on input parameters
  cerr<< "\nabout to start the badness";
  string out;
  char * outfile;
  //cerr << l << "hello";
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
  theSS << ".M_" ;
  theSS << Xmax;
  theSS << ".F_";
  theSS << functionType;
  theSS << ".T_";
  theSS << testType;
  theSS << ".T_";
  theSS << tolerance;
  theSS << ".I_";
  theSS << maxIts;
  theSS <<  ".png";
  out=theSS.str();
  //convert string to char*
  outfile = &out[0];
  cerr << outfile;

  image.write(outfile);
  return 0;
  //nrd2bmp("temp.nrd", outfile);
  //silent mode!!!!!!!
  //cerr << "\a";
  /*
  system(argv[1]);
  */
  //tells the system to open the newly created bmp file but seems to
  //cause serious problems (full system halt!!!)
}

png::rgb_pixel juliaTest(double Za, double Zb, double Ca, double Cb)
{
  int r;
  double aHistory[maxIts+1];
  double bHistory[maxIts+1];
  aHistory[0]=Za;
  bHistory[0]=Zb;
  long i=0;
  //while the abs(Z) < "infinity" && we're still iterating
  //note that both sides of inequality are squared for efficiency
  while (( pow(Za,2) + pow(Zb,2) <= pow(INFINITY,2)) && (i<maxIts))//the square of infinity ¶:)
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
        if (testFunction(Za, aHistory, Zb, bHistory, j, tolerance))
        {
          return png::rgb_pixel(255, (35*j)%256, 0);
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

  if (i<maxIts)
  {
    if( grayScale ){
      r=( (((unsigned char)( acos((double)(maxIts-2*i)/maxIts)*cosScale/(PI))*25 )% 256 ) );}
    else{r = 0;}
    return png::rgb_pixel(r, r, r);
  }
  else
  {
    return png::rgb_pixel(255, (10*i)%256, 0);
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
