
#ifndef ATOMS_H
#define ATOMS_H

enum AtomNo {           /* 原子名の変数に原子番号を代入 */
    START,
     H,                                                                 He,
    Li, Be,                                          B,  C,  N,  O,  F, Ne,
    Na, Mg,                                         Al, Si,  P,  S, Cl, Ar,
     K, Ca, Sc, Ti,  V, Cr, Mn, Fe, Co, Ni, Cu, Zn, Ga, Ge, As, Se, Br, Kr,
    Rb, Sr,  Y, Zr, Nb, Mo, Tc, Ru, Rh, Pd, Ag, Cd, In, Sn, Sb, Te,  I, Xe,
    Cs, Ba, 
            La, Ce, Pr, Nd, Pm, Sm, Eu, Gd, Tb, Dy, Ho, Er, Tm, Yb, Lu, 
                Hf, Ta,  W, Re, Os, Ir, Pt, Au, Hg, Tl, Pb, Bi, Po, At, Rn,
    Fr, Ra, 
            Ac, Th, Pa,  U, Np, Pu, Am, Cm, Bk, Cf, Es, Fm, Md, No, Lr, 
                Rf, Db, Sg, Bh, Hs, Mt,
    D,
    ATOMS
};

enum AbEN {
  Kedge, LIedge, LIIedge, LIIIedge, Edges
};

extern const char *AbENames[ Edges ];

struct AtomInf {    /* 各元素の情報 */
  const AtomNo AN;     /* 原子番号         */
  const char *AName;   /* 原子名 (文字列)  */
  const double M;      /* 原子量           */
};

const AtomInf A[ ATOMS ] = {  /* 各元素の情報 */
  /*  原子番号, 原子名, 原子量 */
  { START, (char*) " ",   0           },
  {     H, (char*) "H",   1.007900000 },
  {    He, (char*)"He",   4.002600000 },
  {    Li, (char*)"Li",   6.941000000 },
  {    Be, (char*)"Be",   9.012180000 },
  {     B, (char*) "B",  10.810000000 },
  {     C, (char*) "C",  12.011000000 },
  {     N, (char*) "N",  14.006700000 },
  {     O, (char*) "O",  15.999400000 },
  {     F, (char*) "F",  18.998403000 },
  {    Ne, (char*)"Ne",  20.179000000 },
  {    Na, (char*)"Na",  22.989770000 },
  {    Mg, (char*)"Mg",  24.305000000 },
  {    Al, (char*)"Al",  26.981450000 },
  {    Si, (char*)"Si",  28.085500000 },
  {     P, (char*) "P",  30.973760000 },
  {     S, (char*) "S",  32.060000000 },
  {    Cl, (char*)"Cl",  35.453000000 },
  {    Ar, (char*)"Ar",  39.948000000 },
  {     K, (char*) "K",  39.098300000 },
  {    Ca, (char*)"Ca",  40.080000000 },
  {    Sc, (char*)"Sc",  44.955900000 },
  {    Ti, (char*)"Ti",  47.880000000 },
  {     V, (char*) "V",  50.941500000 },
  {    Cr, (char*)"Cr",  51.996000000 },
  {    Mn, (char*)"Mn",  54.938000000 },
  {    Fe, (char*)"Fe",  55.847000000 },
  {    Co, (char*)"Co",  58.933200000 },
  {    Ni, (char*)"Ni",  58.960000000 },
  {    Cu, (char*)"Cu",  63.546000000 },
  {    Zn, (char*)"Zn",  65.380000000 },
  {    Ga, (char*)"Ga",  69.720000000 },
  {    Ge, (char*)"Ge",  72.590000000 },
  {    As, (char*)"As",  74.921600000 },
  {    Se, (char*)"Se",  78.960000000 },
  {    Br, (char*)"Br",  79.904000000 },
  {    Kr, (char*)"Kr",  83.800000000 },
  {    Rb, (char*)"Rb",  85.467800000 },
  {    Sr, (char*)"Sr",  87.620000000 },
  {     Y, (char*) "Y",  88.905900000 },
  {    Zr, (char*)"Zr",  91.220000000 },
  {    Nb, (char*)"Nb",  92.906400000 },
  {    Mo, (char*)"Mo",  95.940000000 },
  {    Tc, (char*)"Tc",  97.907000000 },
  {    Ru, (char*)"Ru", 101.070000000 },
  {    Rh, (char*)"Rh", 102.905500000 },
  {    Pd, (char*)"Pd", 106.420000000 },
  {    Ag, (char*)"Ag", 107.868000000 },
  {    Cd, (char*)"Cd", 112.410000000 },
  {    In, (char*)"In", 114.820000000 },
  {    Sn, (char*)"Sn", 118.690000000 },
  {    Sb, (char*)"Sb", 121.750000000 },
  {    Te, (char*)"Te", 127.600000000 },
  {     I, (char*) "I", 126.904500000 },
  {    Xe, (char*)"Xe", 131.290000000 },
  {    Cs, (char*)"Cs", 132.905400000 },
  {    Ba, (char*)"Ba", 137.330000000 },

  {    La, (char*)"La", 138.905500000 },
  {    Ce, (char*)"Ce", 140.120000000 },
  {    Pr, (char*)"Pr", 140.907700000 },
  {    Nd, (char*)"Nd", 144.240000000 },
  {    Pm, (char*)"Pm", 144.913000000 },
  {    Sm, (char*)"Sm", 150.360000000 },
  {    Eu, (char*)"Eu", 151.960000000 },
  {    Gd, (char*)"Gd", 157.250000000 },
  {    Tb, (char*)"Tb", 158.925400000 },
  {    Dy, (char*)"Dy", 162.500000000 },
  {    Ho, (char*)"Ho", 164.930400000 },
  {    Er, (char*)"Er", 167.260000000 },
  {    Tm, (char*)"Tm", 168.934200000 },
  {    Yb, (char*)"Yb", 173.040000000 },
  {    Lu, (char*)"Lu", 174.967000000 },

  {    Hf, (char*)"Hf", 178.490000000 },
  {    Ta, (char*)"Ta", 180.947900000 },
  {     W, (char*) "W", 183.850000000 },
  {    Re, (char*)"Re", 186.207000000 },
  {    Os, (char*)"Os", 190.200000000 },
  {    Ir, (char*)"Ir", 192.220000000 },
  {    Pt, (char*)"Pt", 195.080000000 },
  {    Au, (char*)"Au", 196.966500000 },
  {    Hg, (char*)"Hg", 200.590000000 },
  {    Tl, (char*)"Tl", 204.383000000 },
  {    Pb, (char*)"Pb", 207.200000000 },
  {    Bi, (char*)"Bi", 208.980400000 },
  {    Po, (char*)"Po", 208.982000000 },
  {    At, (char*)"At", 209.987000000 },
  {    Rn, (char*)"Rn", 222.018000000 },
  {    Fr, (char*)"Fr", 223.020000000 },
  {    Ra, (char*)"Ra", 226.025400000 },

  {    Ac, (char*)"Ac", 227.027800000 },
  {    Th, (char*)"Th", 232.038100000 },
  {    Pa, (char*)"Pa", 231.035900000 },
  {     U, (char*) "U", 238.051000000 },
  {    Np, (char*)"Np", 237.048200000 },
  {    Pu, (char*)"Pu", 239.052000000 },
  {    Am, (char*)"Am", 243.061000000 },
  {    Cm, (char*)"Cm", 247.070000000 },
  {    Bk, (char*)"Bk", 247.070000000 },
  {    Cf, (char*)"Cf", 251.080000000 },
  {    Es, (char*)"Es", 252.083000000 },
  {    Fm, (char*)"Fm", 257.095000000 },
  {    Md, (char*)"Md",   0.000000000 },
  {    No, (char*)"No",   0.000000000 },
  {    Lr, (char*)"Lr",   0.000000000 },

  {    Rf, (char*)"Rf",   0.000000000 },
  {    Db, (char*)"Db",   0.000000000 },
  {    Sg, (char*)"Sg",   0.000000000 },
  {    Bh, (char*)"Bh",   0.000000000 },
  {    Hs, (char*)"Hs",   0.000000000 },
  {    Mt, (char*)"Mt",   0.000000000 },

  {    D,  (char*)"D",    2.013553210 },
};

struct VICS {
  int AN;
  double AE[ 5 ];       // K, LI, LII, LIII, MI
  double C[ 5 ];
  double D[ 5 ];
};

void ReadVicF( void );

extern VICS Vic[ 100 ];
extern int Vics;

#endif
