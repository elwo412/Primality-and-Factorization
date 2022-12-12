#include <pari/pari.h>

long
isprimeSelfridge(GEN x) { return (plisprime(x,0)==gen_1); }

int
BSW_isprime(GEN x)
{
  pari_sp av = avma;
  long l, res;
  GEN F, p, e;

  if (BSW_isprime_small(x)) return 1;
  F = auxdecomp((addsi(-(1),(x))), 0);
  l = ((long)(((pari_ulong*)((((GEN*) (F))[1])))[0] & ((1UL<<((1L<<(2 +3)) - 1 - 7))-1)))-1; p = (((GEN**)(F))[1][l]); e = (((GEN**)(F))[2][l]); F=(((GEN*) (F))[1]);
  if (cmpii(powgi(p, shifti(e,1)), x)<0)
    res = isprimeSelfridge(mkvec2(x,vecslice(F,1,l-1)));
  else if (BSW_psp(p))
    res = isprimeSelfridge(mkvec2(x,F));
  else
    res = isprimeAPRCL(x);
  avma = av; return res;
}

long
isprime(GEN x)
{
  return BSW_psp(x) && BSW_isprime(x);
}