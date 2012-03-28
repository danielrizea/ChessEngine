/* 
 * File:   Tools.h
 * Author: Flip4it chess

 *
 * Created on March 12, 2010, 9:30 PM
 */
#include "stuff.h"


#ifndef _TOOLS_H
#define	_TOOLS_H



class Tools {
public:
    Tools();

    //Intoarce pozitia primului bit de 1(cel mai putin) din paramatrul de 64 biti primit ca parametru
    static unsigned int bitScan(U64);

    //Intoarce un BITBOARD care are biti de 1 setati pentru pozitiile in care se poate muta
    //piesa 'piesa' din pozaitia 'pozitie'
    //RESCRISA in Chessboard::getPseudoMoves
    //static BITBOARD getPossibleMoves(int pozitie,PIECE piesa);

    //DEBUG: Afiseaza bitboard-ul (1 si 0) pe ecran.
    static void printBitBoard(BITBOARD board);

    //Functii pentru generare mutari posibile pentru tipuri de piese
    static BITBOARD* generateKnightMoves();

    static BITBOARD* generateKingMoves();
// doar mutarile 
    static BITBOARD* generateOnlyPownMoves(COLOR player);
    //
    static BITBOARD* generatePownAttackMoves(COLOR player);
    
};


//constante pentru bitScan
const U64 magic=(U64)0x022fdd63cc95386dULL; // the 4061955.
const unsigned int magictable[64] =
{
    0,  1,  2, 53,  3,  7, 54, 27,
    4, 38, 41,  8, 34, 55, 48, 28,
   62,  5, 39, 46, 44, 42, 22,  9,
   24, 35, 59, 56, 49, 18, 29, 11,
   63, 52,  6, 26, 37, 40, 33, 47,
   61, 45, 43, 21, 23, 58, 17, 10,
   51, 25, 36, 32, 60, 20, 57, 16,
   50, 31, 19, 15, 30, 14, 13, 12,
};




#endif	/* _TOOLS_H */

