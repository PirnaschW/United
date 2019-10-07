#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "Tools.h"

int Tools::CenterLine (const char * line, const int max, char * buffer)
{
  int z;

  z = (max - strlen(line)) / 2;
  return(sprintf(buffer,"%*s%-*s",z,"",max-z,line));
}

extern "C" {

static char * roman_digit (int x, char c1, char c5, char c10);
char * roman (const int x)
{
  char * p1, * p2, * p3;
  static char buffer [22];
  if (x > 10000 || x < 0) return "0";
  sprintf(buffer,"%.*s%s%s%s",
          x/1000,"MMMMMMMMMM",
          p1=roman_digit((x/100)%10,'C','D','M'),
          p2=roman_digit((x/ 10)%10,'X','L','C'),
          p3=roman_digit( x     %10,'I','V','X'));
  free(p1); free(p2); free(p3);
  return buffer;
}

static char * roman_digit (int x, char c1, char c5, char c10)
{
  int off = 0;
  char * buffer = (char *) malloc(5), filler [4];

  filler[0] = filler[1] = filler[2] = c1;
  if (x % 5 == 4) sprintf(buffer,"%c%c",c1,x==4?c5:c10);
  else {
    if (x > 4) buffer[off++] = c5;
    sprintf(buffer+off,"%.*s",x%5,filler);
  }
  return buffer;
}
int rndint (const int x)
{
  return ((int) (rnd() * (double) x) + 1);
}


double rnd (void)
/* Linearer Kongruenzgenerator nach Afflerbach
   x[i+1] = x[i] * 27132 + 1   (mod 62748517), z[i] = x[i] / 62748517
   Periodenlaenge 62748517, Beyer-Quotienten 0.969, 0.922, 0.819
   Bestmoegliche Verteilung fuer Modul m < 1E9 */
{
  static unsigned long xi = 0;
  int i;
  unsigned long z = 0;

  if (xi == 0) xi = (unsigned long) time(NULL);

  for (i=30; i>0; i--) {
    z <<= 1;
    if (xi & (1L << (i-1))) z += 27132L;
    while (z > 62748516L) z -= 62748517L;
  }
  xi = z + 1L;
  return ((double) xi / 62748517.0);
}

} // end extern "C"
