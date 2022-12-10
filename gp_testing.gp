n = 17;
a = 2;
r = 4;
p = (x+a)^n;
p2 = x^n + a

q = x^4 - 1;

R1 = Mod(p,q);
R2 = Mod(p2,q);

RR1 = Mod(R1, n);
RR2 = Mod(R2, n);

RR1==RR2