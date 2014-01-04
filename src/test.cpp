#include <stdio.h>
#include <math.h>
#include <iostream.h>
#define PIXWIDTH 1024
#define PIXHEIGHT 768
#define INFINITY 1000
#define MAXITS 1000

char *julia(const int &, const int &, const double &, const double &);
void printMatrix(char [768][PIXWIDTH][3]);
void normalize(int &i);
int scale(int in) {return in*255/MAXITS;};
//char currPixel[3];

int main()
{
  char pixelArray[PIXHEIGHT][PIXWIDTH][3];
  double Ca=0;
  double Cb=0;

  cout << "Re(C):  ";
  cin >> Ca;
  cout << "Im(C):  ";
  cin >> Cb;

  for(int row=0;row<PIXWIDTH;row++)
  {
    for(int col=0;col<PIXHEIGHT;col++)
    {
      pixelArray[row][col][0]=*julia(row,col, Ca, Cb);
    }
  }

  printMatrix(pixelArray);
  //bitmapMatrix(&pixelArray);
  return 0;
}

char *julia(const int &row, const int &col, const double &Ca, const double &Cb)
{
  char currPixel[3];
  currPixel[0]=255;
  currPixel[1]=0;
  currPixel[2]=0;
  int i=0;  //how many loops it takes for value to explode to infinity
  double a=0; //temp variable for Za
  double b=0; //temp variable for Zb
  double Za=scale(row); //scale row and col to reasonable starting values...
  double Zb=scale(col);

  while( (sqrt(pow(Za,2) + pow(Zb,2)) < INFINITY) && (i<MAXITS))
  {
    a = pow(Za,2) - pow(Zb,2) + Ca;
    b = 2*Za*Zb + Cb;
    Za = a;
    Zb = b;
    i++;
  }
  normalize(i);//normalize to 0-255 range...
  currPixel[1]=i;
  return currPixel;
}

void normalize (int &i)
{
  i*= (255/MAXITS);
}

void printMatrix(const char pixelArray[PIXHEIGHT][PIXWIDTH][3])
{
  for(int row=0;row<PIXWIDTH;row++)
  {
    for(int col=0;col<PIXHEIGHT;col++)
    {
      if(pixelArray[row][col][1]<MAXITS) {cout << "1";}
      else {cout << "0";};
    }
    cout << endl;
  }
}
