#ifndef _TOOLS_H_
#define _TOOLS_H_

class Tools
{
  public:
    static int CenterLine (const char *, const int, char *);
};

extern "C" {
char * roman  (const int x);
int    rndint (const int x);
double rnd    (void);
}
#endif
