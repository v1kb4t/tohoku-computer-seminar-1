#include<stdio.h>

int main() {
  char c = 'a';
  
  char s[4] = "abc";
  int i;


 

  for(i=0; i<3; i++)
	  printf("%c", s[i] + 'A'-'a');
}
