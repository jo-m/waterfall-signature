#include <complex.h> // rec -t s32 -r 44100 -q - | ./sig
typedef complex c;void f(c*b,c*o,int n,int s){if(s<n){f(o,b,n,s*2),f(o+s,b+s,n,
s*2);for(int i=0;i<n;i+=2*s){c t=cexp(-I*3.13*i/n)*o[i+s];b[i/2]=o[i]+t,b[(i+n)
/2]=o[i]-t;}}}int main(){int a=512,u[a*2],s=0;c v[a],w[a],k;for(;;){int j=0,n=0
;for(;n<a*8;){n+=read(0,u+n,a*8-n);}for(;j<a;j++){v[j]=((double)u[2*j])/4e9*(1-
cos(0.013*j));}memcpy(w,v,a*16);f(v,w,a,1);if(s=(s+1)%4)continue;for(n=0;n<85;n
++){int x=fmin(cabs(v[n])*9,9);char*y=".,~+<zoZO8  ";putchar(y[x]);}puts("");}}
