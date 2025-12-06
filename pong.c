#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <math.h>

void video(unsigned char n){
    asm {
        mov ah, 0
        mov al, n
        int 10h
    }
}

void pixel(int x, int y, unsigned char color){
    asm {
        mov ah, 0Ch
        mov al, color
        mov cx, x
        mov dx, y
        mov bh, 0
        int 10h
    }
}

void marco(){
    int i;
    for(i = 0; i < 640; i++){
        pixel(i, 0, 1);
        pixel(i, 199, 1);
    }
    for(i = 0; i < 200; i++){
        pixel(0, i, 1);
        pixel(639, i, 1);
    }
}

void barra(int x, int y, int len, int color){
    int i;
    for(i = 0; i < len; i++){
        pixel(x + i, y, color);
    }
}

void pelota(int cx, int cy, int r, int color){
    int i;
    for(i = 0; i < 360; i++){
        pixel(cx + r * cos(i * M_PI/180),
              cy + r * sin(i * M_PI/180),
              color);
    }
}

int main(){
    while(1){
        int barraLen;
        int radio;
        int barraX;
        int barraY = 180;
        int oldBarraX;
        int bx, by;
        int dx = 1, dy = -1;
        int ch;
        float velList[] = {1.5, 2.0, 2.5};
        int totalVel = 3;


        clrscr();

        printf("Longitud de la barra (20-200): ");
        scanf("%d", &barraLen);
        if(barraLen < 20) barraLen = 20;
        if(barraLen > 200) barraLen = 200;

        printf("Radio de la pelota (2-20): ");
        scanf("%d", &radio);
        if(radio < 2) radio = 2;
        if(radio > 20) radio = 20;

        video(6);

        barraX = (640 - barraLen) / 2;
        bx = 320;
        by = 100;

        marco();
        barra(barraX, barraY, barraLen, 1);

        while(1){
            oldBarraX = barraX;

            if(kbhit()){
                ch = getch();
                if(ch == 27){
                    video(3);
                    return 0;
                }
                if(ch == 0 || ch == 224){
                    ch = getch();
                    if(ch == 75 && barraX > 2) barraX -= 5;
                    if(ch == 77 && barraX + barraLen < 638) barraX += 5;
                }
            }

            barra(oldBarraX, barraY, barraLen, 0);
            barra(barraX, barraY, barraLen, 1);

            pelota(bx, by, radio, 0);

            bx += dx * velList[rand() % totalVel];
            by += dy * velList[rand() % totalVel];

            if(bx - radio <= 1 || bx + radio >= 638) dx = -dx;
            if(by - radio <= 1) dy = -dy;

            if(by + radio >= barraY - 1){
                if(bx >= barraX && bx <= barraX + barraLen){
                    dy = -dy;
                } else {
                    video(3);
                    printf("\nGAME OVER\n");
                    printf("Presiona una tecla para reiniciar...\n");
                    getch();
                    break;
                }
            }

            pelota(bx, by, radio, 1);
            delay(5);
        }
    }

    video(3);
    return 0;
}

