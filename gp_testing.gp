n = 22953686867719691230002707821868552601124472329079

lg2 = log(n)/log(2)
max_k = floor(lg2^2)
max_r = max(3, ceil(lg2^5))
nextR=1

r = 2
k = 1

{
  for (i=2, max_r,
  	r=i;
  	if (nextR == 0 || i >= max_r, break);
  	nextR = 0;
    for (j=2, max_k,
      k=j;
      if (nextR == 1 || j > max_k, break);
      nextR = n^j % i == 1 || n^j % i == 0)) 
}


r--

print(r)
