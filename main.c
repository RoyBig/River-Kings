#include <msp430x14x.h>
#include "lcd.h"
#include "portyLcd.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define BUTTON1 (P4IN & BIT4)
#define BUTTON2 (P4IN & BIT5)
#define BUTTON3 (P4IN & BIT6)
#define BUTTON4 (P4IN & BIT7)

char screen[2][32];
int jump;
int points;
int time1;
int lifes;
int immortality;
int speedness;
int doubles;
int count_immortality;
int count_time;
int count_doubles;
int mnoznik;
int bestplayer1;
int bestplayer2;
bool gracz;

bool gameEnded = false;
bool showedEndScreen = false;

bool playerPosFlag = false;
bool titleScreenFlag = false;

bool buttonPressed = false;
bool buttonPressed4 = false;

void initClock();

void initButtons() {
    P4DIR &= ~(BIT4 | BIT5 | BIT6 | BIT7);
}

void printText(char text[]) {
     for(int i = 0; i < strlen(text); i++){
          SEND_CHAR(text[i]);
     }
}
void initAllComponents();

void initObjects(void) {
    SEND_CMD(CG_RAM_ADDR);
    unsigned char fish[]= {0, 0, 4, 22, 15, 22, 4, 0}; // ryba - 8
    unsigned char smallfish[]= {0, 0, 0, 0, 12, 28, 12, 0}; //ma�a rybka - 9
    unsigned char bonus1[]= {0, 0, 10, 31, 31, 14, 4, 0}; //dodatkowe �ycia - 10
    unsigned char bonus2[]={0, 21, 21, 31, 21, 31, 14, 0}; //nie�miertelno�c - 11
    unsigned char oponent1[]= {0, 0, 21, 14, 31, 14, 21, 0}; //bomba - 12
    unsigned char oponent2[]={0, 0, 14, 21, 23, 17, 14, 0}; //zegarek - 13
    unsigned char oponent3[]={0, 14, 31, 31, 10, 10, 10, 0}; //meduza -14
    unsigned char bonus3[]={0, 0, 3, 21, 11, 21, 3, 0}; //x2 punkty - 15

    for(int i=0; i<8; i++) {
        SEND_CHAR(fish[i]);
    }
    for(int i=0; i<8; i++) {
        SEND_CHAR(smallfish[i]);
    }
    for(int i=0; i<8; i++) {
        SEND_CHAR(bonus1[i]);
    }
    for(int i=0;i<8;i++){
        SEND_CHAR(bonus2[i]);
    }
    for(int i=0; i<8; i++) {
        SEND_CHAR(oponent1[i]);
    }
    for(int i=0; i<8; i++) {
        SEND_CHAR(oponent2[i]);
    }
    for(int i=0; i<8; i++) {
        SEND_CHAR(oponent3[i]);
    }
    for(int i=0; i<8; i++) {
        SEND_CHAR(bonus3[i]);
    }

}

void initScreen() {
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 17; j++) {
            screen[i][j] = ' ';
        }
    }
}

void initTitleScreen(){
    char firstRow[] = "RIVER KING";
    char secondRow[] = "P1 PRESS 1";
    char thirdRow[] = "P2 PRESS 2";
    clearDisplay();
    SEND_CMD(DD_RAM_ADDR);
    printText(firstRow);
    for(int i=0;i<16;i++)
    {
        SEND_CMD(DD_RAM_ADDR2+i);
        SEND_CHAR(8);
        Delayx100us(3000);
        SEND_CMD(DD_RAM_ADDR2+i);
        SEND_CHAR(' ');
    }
 
    Delayx100us(20000);
    clearDisplay();
    SEND_CMD(DD_RAM_ADDR);
    printText(secondRow);
    SEND_CMD(DD_RAM_ADDR2);
    printText(thirdRow);

    while(!titleScreenFlag) {
        if(!BUTTON1) {
            gracz=0;
            titleScreenFlag = true;
        }
        if(!BUTTON2) {
            gracz=1;
            titleScreenFlag = true;
        }
    }
}

void generateObject() {
    int choice = 0;
    choice = rand() % 1000;
    if(choice < 125)
    {
        screen[0][16] = ' ';
        screen[1][16] = 9;
    }
    else if(choice < 250)
    {
        screen[0][16] = 9;
        screen[1][16] = ' ';
    }
    else if(choice < 252)
    {
        screen[0][16] = ' ';
        screen[1][16] = 10;
    }
    else if(choice < 254)
    {
        screen[0][16] = 10;
        screen[1][16] = ' ';
    }
    else if(choice < 256)
    {
        screen[0][16] = ' ';
        screen[1][16] = 11;
    }
    else if(choice < 258)
    {
        screen[0][16] = 11;
        screen[1][16] = ' ';
    }
    else if(choice < 320 && screen[0][15]!=12 && screen[1][15]!=14)
    {
        screen[0][16] = ' ';
        screen[1][16] = 12;
    }
    else if(choice < 383 && screen[1][15]!=12 && screen[0][15]!=14)
    {
        screen[0][16] = 12;
        screen[1][16] = ' ';
    }
    else if(choice < 388)
    {
        screen[0][16] = ' ';
        screen[1][16] = 13;
    }
    else if(choice < 393)
    {
        screen[0][16] = 13;
        screen[1][16] = ' ';
    }
    else if(choice < 424 && screen[1][15]!=12 && screen[0][15]!=14)
    {
        screen[0][16] = ' ';
        screen[1][16] = 14;
    }
    else if(choice < 457 && screen[0][15]!=12 && screen[1][15]!=14)
    {
        screen[0][16] = 14;
        screen[1][16] = ' ';
    }
    else if(choice < 467)
    {
        screen[0][16] = ' ';
        screen[1][16] = 15;
    }
    else if(choice < 477)
    {
        screen[0][16] = 15;
        screen[1][16] = ' ';
    }
    else
    {
        screen[0][16] = ' ';
        screen[1][16] = ' ';
    }
}

void moveDisplayRight() {
    generateObject();
    for(int i = 0; i < 16; i++) {
        screen[0][i] = screen[0][i+1];
        screen[1][i] = screen[1][i+1];
    }
    if(screen[0][6] == 14)
    {
        screen[0][6]= ' ';
        screen[1][6]= 14;
    }
    else if(screen[1][6] == 14)
    {
        screen[0][6]= 14;
        screen[1][6]= ' ';
    }

}

void display() {
    clearDisplay();
    SEND_CMD(DD_RAM_ADDR);
    for(int i = 0; i < 16; i++) {
        if(screen[0][i] == 'x') {
            SEND_CHAR(8);
        }
        else {
            SEND_CHAR(screen[0][i]);
        }
    }
    SEND_CMD(DD_RAM_ADDR2);
    for(int i = 0; i < 16; i++) {
        if(screen[1][i] == 'x') {
            SEND_CHAR(8);
        }
        else {
            SEND_CHAR(screen[1][i]);
        }
    }
}

void startGame() {
    while(1) {
        if(!playerPosFlag) {
            jump = 0;
        }
        else {
            jump = 1;
        }
    }
}

void getNumber(unsigned int num) {
    char charVal;
    if(num < 10) {
        charVal = num + '0';
        SEND_CHAR(charVal);
    }
    else {
        getNumber(num / 10);
        charVal = (num%10) + '0';
        SEND_CHAR(charVal);
    }
}

void initEndScreen(unsigned int result) {
    clearDisplay();
    SEND_CMD(DD_RAM_ADDR);
    char resultLabel[] = "Your score: ";
    printText(resultLabel);
    getNumber(result);
    if (gracz == 0)
    {
        SEND_CMD(DD_RAM_ADDR2);
        char resultLabel1[] = "Best score 1: ";
        if (result > bestplayer1)
        {
            bestplayer1 = result;
        }
        printText(resultLabel1);
        getNumber(bestplayer1);
    }
    else
    {
        SEND_CMD(DD_RAM_ADDR2);
        char resultLabel2[] = "Best score 2: ";
        if(result>bestplayer2)
        {
            bestplayer2 = result;
        }
        printText(resultLabel2);
        getNumber(bestplayer2);
    }
    Delayx100us(50000);
    clearDisplay();
    SEND_CMD(DD_RAM_ADDR);
    char first[] = "PLAYER CHANGE: 3";
    printText(first);
    SEND_CMD(DD_RAM_ADDR2);
    char second[] = "RESTART: 4 G ";
    printText(second);
    int work = 1;
    while (work)
    {
        if (BUTTON3)
        {
            buttonPressed = 0;
        }
        if (!BUTTON3 && !buttonPressed)
        {
            gracz = !gracz;
            buttonPressed = 1;
        }
        else if (!BUTTON4)
        {
            work = 0;
        }
        SEND_CMD(DD_RAM_ADDR2+12);
        getNumber(gracz+1);
    }
}


void updateDiodesDown(int lifes) {
    switch(lifes) {
    case 2:
        P2OUT |= BIT1;
        break;
    case 1:
        P1OUT &= ~BIT5;
        break;
    case 0:
        P1OUT &= ~BIT6;
        gameEnded = true;
        break;
    }
}
void updateDiodesUp(int lifes) {
    switch(lifes) {
    case 3:
        P2OUT &= ~BIT1;
        break;
    case 2:
        P1OUT |= BIT5;
        break;
    }
}

void initDiodes() {
    P1DIR |= BIT5;
    P1DIR |= BIT6;
    P2DIR |= BIT1;
}

void lightUpDiodes() {
    P1OUT |= BIT5;
    P1OUT |= BIT6;
    P2OUT &= ~BIT1;
}

void initAllComponents(){
    clearDisplay();

    playerPosFlag = false;
    jump = 0;
    lifes = 3;
    points = 0;
    immortality = 0;
    speedness = 0;
    doubles = 1;
    mnoznik = 10;

    initScreen();
    lightUpDiodes();
}

int main() {
    WDTCTL=WDTPW+ WDTHOLD;
    InitPortsLcd();
    InitLCD();
    clearDisplay();
    initButtons();
    initClock();
    initObjects();
    initDiodes();

    srand(time(NULL));

    initTitleScreen();
    initAllComponents();
    startGame();
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void) {
    if (!gameEnded && titleScreenFlag) {

        if (!BUTTON1 && !buttonPressed) {
            playerPosFlag = !playerPosFlag;
            buttonPressed = true;
        }
        else if (BUTTON1) {
            buttonPressed = false;
        }
        time1++;
        if (time1 % (10 / (speedness + 1)) == 0) {
            count_immortality--;
            count_time--;
            count_doubles--;
            mnoznik++;
            if (count_time <= 0)
            {
                speedness = 0;
            }
            if (count_immortality <= 0)
            {
                immortality = 0;
            }
            if (count_doubles <= 0)
            {
                doubles = 1;
            }
            clearDisplay();
            moveDisplayRight();
            if (jump == 0) {
                if (screen[0][0] == 9) {
                    points = points + ((mnoznik / 10) * doubles);
                }
                else if (screen[0][0] == 10) {
                    if (lifes < 3)
                    {
                        lifes++;
                        updateDiodesUp(lifes);
                    }
                }
                else if (screen[0][0] == 11) {
                    immortality = 1;
                    count_immortality = 32;
                }
                else if ((screen[0][0] == 12 || screen[0][0] == 14) && !immortality) {
                    lifes--;
                    updateDiodesDown(lifes);
                }
                else if (screen[0][0] == 13) {
                    speedness = 1;
                    count_time = 64;
                }
                else if (screen[0][0] == 15) {
                    doubles = 2;
                    count_doubles = 64;
                }
                screen[0][0] = 'x';
                screen[1][0] = ' ';
            }
            if (jump == 1) {
                if (screen[1][0] == 9) {
                    points += ((mnoznik / 10) * doubles);
                }
                else if (screen[1][0] == 10) {
                    if (lifes < 3)
                    {
                        lifes++;
                        updateDiodesUp(lifes);
                    }
                }
                else if (screen[1][0] == 11) {
                    immortality = 1;
                    count_immortality = 32;
                }
                else if ((screen[1][0] == 12 || screen[1][0] == 14) && !immortality) {
                    lifes--;
                    updateDiodesDown(lifes);
                }
                else if (screen[1][0] == 13) {
                    speedness = 1;
                    count_time = 64;
                }
                else if (screen[1][0] == 15) {
                    doubles = 2;
                    count_doubles = 64;
                }
                screen[0][0] = ' ';
                screen[1][0] = 'x';
            }
            display();
            time1 = 0;
        }
    }

    else if (titleScreenFlag) {
        if (!showedEndScreen) {
            initEndScreen(points);
            showedEndScreen = true;
        }

        if (!BUTTON4 && !buttonPressed4) {
            gameEnded = false;
            initAllComponents();
            showedEndScreen = false;
            buttonPressed4 = true;
        }
        else if (BUTTON4) {
            buttonPressed4 = false;
        }
    }
}

void initClock() {
    BCSCTL1 |= XTS;
    do {
        IFG1 &= ~OFIFG;
        for (int i = 0xFF; i > 0; i--);
    }
    while ((IFG1 & OFIFG) == OFIFG);
    BCSCTL1 |= DIVA_1;
    BCSCTL2 |= SELM0 | SELM1;

    TACTL = TASSEL_1 + MC_1 +ID_3;
    CCTL0 = CCIE;
    CCR0=10000;
    _EINT();
}
