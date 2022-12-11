inline(ismersenne);
ismersenne(x)=ispseudoprime(2^x-1);
parforprime(p=1,999,ismersenne(p),c,if(c,print(p)))
prodmersenne(N)=
{ my(R=1);
parforprime(p=1,N,
ismersenne(p),
c,
if(c, R*=p));
R;
}
prodmersenne(1000)