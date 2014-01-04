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

class nrdRGB
{
  public:
  unsigned char R;
  unsigned char G;
  unsigned char B;
  nrdRGB(unsigned char Rin, unsigned char Gin, unsigned char Bin) {R=Rin;G=Gin;B=Bin;}
};