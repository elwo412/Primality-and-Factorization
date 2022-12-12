/* used as a running time benchmark, seen here: https://pari.math.u-bordeaux.fr/archives/pari-users-2002/msg00001.html */
aks6(n)=
{
local(r,l,a,bornesup,X);
if( ispower(n),
	return(0);break,
	r=2; l=log(n)/log(2);
	while( 1==1,		
		if( gcd(r,n)==1, if( znorder(Mod(n,r))<(l^2),r++,break), r++)
		);
	for( a=2, r, if( gcd(a,n) != 1, return(0);break));
	if( n<5690035 && !n>r, return(1);break);
	bornesup=floor(sqrt(eulerphi(r))*l);
	X = Mod(Mod(1,n)*x,x^r-1);
    	for( a=1, bornesup,
        	if( (X+a)^n != X^n+a, return(0);break);
    	   );
	return(1);
)
}