#include<pthread.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<math.h>
#define MAX 1000000
#define tmat uint8_t
#include<string.h>

/*
Code to assert that the morphism from table_D_morphisms.txt given under (p, q) is square-free, square-free modulo p and square-free modulo q

Compilation : gcc -O3 -Wno-unused-result -o table_D_blue.o table_D_blue.c
Execution   : ./table_D_blue p q with p < q (and (p,q) being a blue cell of the associated table)
*/

typedef int (*fc) (int*, int);
int mot[1000000], mot2[1000000], mp[1000000], mq[1000000], code[3], i, k, s, x, size, p, q, nb, mnb, pos, ppcm;
char ch[100000], pair[100];
FILE *in;


int pgcd(int a, int b){
 int l;
 if((a < 0) || (b <= 0)){ printf("!!! pgcd: a=%d b=%d \n", a, b); exit(1); }
 while(a){
  l = a;
  a = b%a;
  b = l;
 }
 return b;
}

int eq(int *g, int *d, int l){
 for(; l--; ) if(g[l] != d[l]) return 0;
 return 1;
}

int square(int *m, int i){
 int x = 1, l = i+1;
 for(; 2*x <= l; x++) if(eq(m+l-2*x, m+l-x, x)) return 0;
 return 1;
}//*/

int subseq(int *m, int i, int s, int *h){
 int z;
 if(i%s) return 1;
 for(z = 0; z*s <= i; z++) h[z] = m[z*s];
 return square(h, z-1);
}//*/

int fo(int *m, int i){
 return square(mot2, i) && subseq(mot2, i, p, mp) && subseq(mot2, i, q, mq);
}//*/

void usage(){
 printf("Usage: table_D_blue.o p q\n");
 printf("with p < q\n");
 exit(0);
}

int main(int ac, char **av){
 if(ac != 3) usage();
 p = atoi(av[1]);
 q = atoi(av[2]);
 if(p >= q) usage();
 ppcm = p*q/pgcd(p,q);
 sprintf(pair, "%d %d\n", p, q);
 in = fopen("table_D_morphisms.txt", "r");
 for(fgets(ch, 100, in); strcmp(pair, ch); fgets(ch, 100, in));
 fgets(ch, 80000, in);
 size = strlen(ch)-1;
 if(size%ppcm){ printf("size=%d ppcm=%d\n", size, ppcm); exit(0); }
 for(k = 0; k < 3; k++){
  for(i = 0; i < size; i++) mot[k*size+i] = ch[i]-'0';
  fgets(ch, 80000, in);
 }
 fclose(in);
 printf("p=%d q=%d ppcm=%d size=%d=%d*ppcm\n", p, q, ppcm, size, size/ppcm);
 for(*code = x = 0; ;) if(!square(code, x)){
  ici: while(code[x] == 2) if(!x--){ printf("morphism OK\n"); exit(0); }
  code[x]++;
 }else{
  s = code[x];
  for(k = 0; k < size; k++){
   mot2[pos = k+x*size] = mot[k+s*size];
   if(x && !fo(mot2, pos)){ printf("morphism NOT square-free\n"); exit(0); }
  }
  if(x == 2) goto ici; else code[++x] = 0;
 }
}
