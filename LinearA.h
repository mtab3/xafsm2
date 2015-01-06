#ifndef LINEARA_H
#define LINEARA_H

class LA
{

public:
  LA( void );
  ~LA( void );

  void clearM( int n, double **M );
  void clearV( int n, double *V );
  void setI( int n, double **I );
  void showM( const char *name, int n, double **M );
  void showV( const char *name, int n, double *V );
  bool checkMerr( int n, double **M );
  bool checkVerr( int n, double *V );
  void copyV( int n, double *s, double *d );
  void invert( int PS, double **M, double **I );
  void makeLU( int n, double **M, int *L );

};

#endif
