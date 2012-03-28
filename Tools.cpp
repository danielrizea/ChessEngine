/* 
 * File:   Tools.cpp
 * Author: Flip4it chess
 * 
 * Created on March 12, 2010, 9:30 PM
 */

#include <stdlib.h>
#include<stdio.h>

#include "Tools.h"

extern FILE* debug_file;

Tools::Tools() {
}

//Intoarce pozitia primului bit de 1;
unsigned int Tools::bitScan (U64 b)
{
    return magictable[((b&-b)*magic) >> 58];
}

/* Functii pentru generare mutari posibile pentru tipuri de piese.
 * In cazul pieselor glisante, sunt incluse inclusiv piesele de la limita capete, atat ale
 * engine-ului cat si ale inamicului.*/
    //RESCRISA in Chessboard::getPseudoMoves
//BITBOARD Tools::getPossibleMoves(int pozitie, PIECE piesa)
//{
////    printBitBoard(pieceMoves[piesa][pozitie]);
//    if(piesa==KNIGHT)
//	return KNIGHT_MOVES[pozitie];
//    return 0;
//}

//DEBUG: Afiseaza bitboard-ul (1 si 0) pe ecran.
void Tools::printBitBoard(BITBOARD board){

    int i,j;
    for(i=7;i>=0;i--){
        for(j=0;j<8;j++)
            if((board & ((U64)1<<(i*8+j))) !=0)
                fprintf(debug_file," 1 ");
            else
                fprintf(debug_file," . ");
                fprintf(debug_file,"\n");
    }
    fprintf(debug_file,"\n");
}


BITBOARD* Tools::generateKnightMoves()
{

//                noNoWe    noNoEa
//                +15  +17
//                 |     |
//    noWeWe  +6 __|     |__+10  noEaEa
//                  \   /
//                   >0<
//               __ /   \ __
//    soWeWe -10   |     |   -6  soEaEa
//                 |     |
//                -17  -15
//            soSoWe    soSoEa
    BITBOARD *moves;

    moves=(BITBOARD*)malloc(64*sizeof(BITBOARD));

    int i;
    BITBOARD board;
    for(i=0;i<64;i++)
    {
        board=0;
        //Daca se afla pe o coloana >=3

        if(i/8<=6)
            if(i%8 >= 2)
                board|=(U64)1<<(i+6);

        if(i/8<=6)
            if(i%8 <= 5)
                board|=(U64)1<<(i+10);
        if(i/8<=5)
            if(i%8 >=1)
                board|=(U64)1<<(i+15);
        if(i/8<=5)
            if(i%8 <= 6)
                board|=(U64)1<<(i+17);

        if(i>=6)
            if(i%8 <= 5)
                board|=(U64)1<<(i-6);
        if(i>=10)
            if(i%8 >= 2)
                 board|=(U64)1<<(i-10);
        if(i>=15)
            if(i%8 <= 6)
                board|=(U64)1<<(i-15);
        if(i>=17)
            if(i%8 >= 1)
                board|=(U64)1<<(i-17);

        moves[i]=board;
    }
    return moves;
}

BITBOARD* Tools::generateKingMoves(){



    BITBOARD *moves;

    moves=(BITBOARD*)malloc(64*sizeof(BITBOARD));

    int i;
    BITBOARD board;
    for(i=0;i<64;i++)
    {
        board=0;
//poate muta toate pozitiile
        if(i/8<=6)
            board|=(U64)1<<(i+8);

        if(i/8<=6)
            if(i%8<=6)
                board|=(U64)1<<(i+9);

        if(i/8<=6)
            if(i%8>=1)
                board|=(U64)1<<(i+7);

        if(i/8>=1)
            board|=(U64)1<<(i-8);

        if(i/8>=1)
            if(i%8<=6)
                board|=(U64)1<<(i-7);

        if(i/8>=1)
            if(i%8>=1)
                board|=(U64)1<<(i-9);


       

        moves[i]=board;
    }
    return moves;

}
// genereaza doar avansul 1 patratica in fata sau 2 (daca e la inceput)
BITBOARD* Tools::generateOnlyPownMoves(COLOR player){

    BITBOARD *moves;

    moves=(BITBOARD*)malloc(64*sizeof(BITBOARD));

    int i;
    BITBOARD board;
    for(i=0;i<64;i++)
    {
        board=0;
//poate muta toate pozitiile
        if(player==WHITE){
        if(i/8<=6)
            board|=(U64)1<<(i+8);

        if(i/8==1)
            board|=(U64)1<<(i+16);
       

        }
        else
        {
            if(i/8==6)
                board|=(U64)1<<(i-16);

            if(i/8>=1)
                board|=(U64)1<<(i-8);




        }

      moves[i]=board;
    }
    return moves;
}
BITBOARD* Tools::generatePownAttackMoves(COLOR player){

     BITBOARD *moves;

    moves=(BITBOARD*)malloc(64*sizeof(BITBOARD));

    int i;
    BITBOARD board;
    for(i=0;i<64;i++)
    {
        board=0;
//poate muta toate pozitiile
        if(player==WHITE){
        if(i/8<=6)
            if(i%8>=1)
            board|=(U64)1<<(i+7);

        if(i/8<=6)
            if(i%8<=6)
            board|=(U64)1<<(i+9);


        }
        else
        {
            if(i/8>=1)
                if(i%8>=1)
                board|=(U64)1<<(i-9);

            if(i/8>=1)
                if(i%8<=6)
                board|=(U64)1<<(i-7);




        }

      moves[i]=board;
    }
    return moves;


}