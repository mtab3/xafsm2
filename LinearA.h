#ifndef LINEARA_H
#define LINEARA_H

class LA
{

public:
  LA( void );
  ~LA( void );

  double **newM( int n );
  double *newV( int n );
  int *newIV( int n );
  void deleteM( int n, double **M );
  void deleteV( double *V );
  void deleteIV( int *V );
  void clearM( int n, double **M );
  void clearV( int n, double *V );
  void setI( int n, double **I );
  void showM( const char *name, int n, double **M );
  void showV( const char *name, int n, double *V );
  void showIV( const char *name, int n, int *V );
  void showMi( const char *name, int n, double **M, int *idx );
  void showVi( const char *name, int n, double *V, int *idx );
  bool checkMerr( int n, double **M );
  bool checkVerr( int n, double *V );
  void copyM( int n, double **s, double **d );
  void copyV( int n, double *s, double *d );
  void mulMM( int n, double **M, double **N, double **R );
  void mulMV( int n, double **M, double *V, double *R );
  void invert( int PS, double **M, double **I );
  void makeLU( int n, double **M, double **L, double **U, int *idx );
  void solveLV( int n, double **L, double *V, double *R, int *idx );
  void solveUV( int n, double **U, double *V, double *R, int *idx );
};

#endif
