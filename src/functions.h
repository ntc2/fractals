//functions for manipulating complex numbers
class cmNum{
  public:
    double a;
    double b;

    cmNum( double _a, double _b ){
      a = _a;
      b = _b;
    }
};  //cmNum class represents a complex number as its real and complex parts

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
  return (exp(Za)*cos(Zb));
}

double be(double Za, double Zb)
{
  return (exp(Za)*sin(Zb));
}


//nathan garbage function
cmNum comMultiply( cmNum comNum1, cmNum comNum2 ){  //function to multiply two arbitrary complex numbers
  cmNum returnValue(comNum1.a*comNum2.a - comNum1.b*comNum2.b, comNum1.a*comNum2.b + comNum1.b*comNum2.a);//constructs a cmNum
  return returnValue;
}//comMultiply

//function to raise a complex number to an arbitrary integer power
cmNum comPow( cmNum comNum, int power ){

  if( power < 1 ){
    cerr << "your either boring or stupid, were going with the latter over here\n";
  }

  if( power == 1 ){
    return comNum;
  }

  return comMultiply( comNum, comPow( comNum, power - 1 ) );
}//comPow