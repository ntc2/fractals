#include <iostream>

using namespace std;

#define FLAG false
#define INFINITY 100000000000000

double aSquare(double Za, double Zb)
{
  return (pow(Za,2) - pow(Zb,2));
}

double bSquare(double Za, double Zb)
{
  return (2*Za*Zb);
}

double aCube(double Za, double Zb)
{
  return (pow(Za,3) - 3*Za*pow(Zb,2));
}

double bCube(double Za, double Zb)
{
  return (3*pow(Za,2)*Zb - pow(Zb,3));
}

double aHepta(double Za, double Zb)
{
  return (pow(Za,7)-21*pow(Za,5)*pow(Zb,2)+35*pow(Za,3)*pow(Zb,4) - 7*Za*pow(Zb,6));
}

double bHepta(double Za, double Zb)
{
  return (-7*pow(Za,6)*Zb+35*pow(Za,4)*pow(Zb,3)-21*pow(Za,2)*pow(Zb,5) +pow(Zb,7));
}

double ae(double Za, double Zb)
{
  if(FLAG){ cerr << "exp(Za)*cos(Zb) is " << exp(Za)*cos(Zb) << endl;}

  double number; //= exp(Za);
  //check to make sure that number is not so small as to cause an underflow
  //.000000000000001 = 10^-15 < 10^-17 ~= 308^.5 = smallest (largest) double exponent
  if( Za > 50 )
  {
    return INFINITY + 1;
  }
  if( Za < -50 )
  {
    return 0;
  }
  else{ number = exp( Za ); }
  return (number*cos(Zb));//(exp(Za)*cos(Zb));
}

double be(double Za, double Zb)
{
  if(FLAG){ cerr << "exp(Za)*sin(Zb) is " << exp(Za)*sin(Zb) << endl;}

  double number; //= exp(Za);
  //check to make sure that number is not so small as to cause an underflow
  //.000000000000001 = 10^-15 < 10^-17 ~= 308^.5 = smallest (largest) double exponent
  //this is no longer relevant
  if( Za > 50 )
  {
    return INFINITY + 1;
  }
  if( Za < -50 )
  {
    return 0;
  }
  else{ number = exp( Za ); }
  return (number*sin(Zb));
}

////////////////////////////////////////////////////////////////
// Recurrence tests.

int andTest(double Za, double * aHistory,
            double Zb, double * bHistory,
            int j, double tolerance) {
  return ((fabs(Za - aHistory[j]) < tolerance) &&
          (fabs(Zb - bHistory[j]) < tolerance));
}

int orTest(double Za, double * aHistory,
           double Zb, double * bHistory,
           int j, double tolerance) {
  return ((fabs(Za - aHistory[j]) < tolerance) ||
          (fabs(Zb - bHistory[j]) < tolerance));
}
