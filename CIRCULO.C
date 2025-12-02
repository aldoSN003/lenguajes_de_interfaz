#include <stdio.h>
#include <math.h>
#include <conio.h> /* Required for kbhit, getch */
#include <dos.h>   /* Required for delay */

/* Set Video Mode via BIOS Interrupt 10h */
void video(unsigned char n){
  asm {
    MOV AH, 0;
    MOV AL, n;
    INT 10H;
  }
}

/* Write Pixel via BIOS Interrupt 10h */
void pixel(int x, int y, unsigned char color){
  asm {
    MOV AH, 0x0C; /* Function 0Ch: Write Graphics Pixel */
    MOV AL, color;
    MOV CX, x;    /* Column */
    MOV DX, y;    /* Row */
    MOV BH, 0;    /* Page number */
    INT 10H;
  }
}

/* Draw Circle */
void circulo(int x, int y, int r, unsigned char color){
  int i;
  for(i = 0; i < 360; i++){
    /* Cast to double for math.h functions */
    pixel(x + r * cos(i * M_PI / 180.0), y + r * sin(i * M_PI / 180.0), color);
  }
}

/* Draw Sine Wave */
void seno(){
  int i;
  double yy;

  for(i = 0; i <= 359; i++){
    /* 0.01745 is approx PI/180 */
    yy = sin(((i + 1) * 3) * 0.01745) * 20 + 50; 
    pixel(i, (int)yy, 1);
  }
}

/* Horizontal Line */
void lineaH(int x, int y, int longitud, unsigned char c){
  int i;
  /* FIX: Loop goes from x to x + length */
  for(i = x; i < x + longitud; i++){
    pixel(i, y, c);
  }
}

int main(){  
  int i;
  int longitud = 50;
  
  /* Mode 6: 640x200 CGA (2 colors: Black and White) */
  video(6); 

  /* Example usage of shapes */
  circulo(320, 50, 20, 1);
  seno();

  /* Animation Loop */
  /* While no key is pressed... */
  while(!kbhit()){
    
    /* Move Right */
    for(i = 0; i <= 639 - longitud; i++){
      if(kbhit()) break; /* Check exit condition inside loop */
      
      lineaH(i - 1, 100, longitud, 0); /* Erase previous (Color 0) */
      lineaH(i, 100, longitud, 1);     /* Draw new (Color 1) */
      delay(5);                        /* Turbo C delay is in milliseconds */
    }

    /* Move Left */
    for(i = 639 - longitud; i >= 0; i--){
      if(kbhit()) break;
      
      lineaH(i + 1, 100, longitud, 0); /* Erase previous */
      lineaH(i, 100, longitud, 1);     /* Draw new */
      delay(5);
    }
  }

  getch(); /* Wait for key */
  video(3); /* Return to text mode (Mode 3) before exiting */
  return 0;
}