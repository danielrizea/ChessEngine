/*
 * File:   Book.h
 * Author: Flip4it chess
 *
 * Created on April 14, 2010, 21:34 PM
 */

#include "ChessBoard.h"
#include <stdio.h>


#ifndef _FDSF_H
#define	_FDSF_H


class Book {
public:
    //Obtine mutarea urmatoare, pornind de la tabla primita ca parametru;
    char* getNextMove (ChessBoard tabla, COLOR culoare);
    //Realizeaza initializarile necesare
    void INITIALIZE(void);
private:
    char BOOKFILE[300];
    char TOERFILE[300];
    char EPD[200];
    char FROM[10],TO[10];
    char ERROR;
    long RANDOM1[1120];
    long RANDOM2[1120];
    char _BORD[256];
    long *p_BORD;               // 32 bit redefinition BORD
    long HK1,HK2;
    int  XD,XE,AZ,RND;
    char FROM1[20],FROM2[20],TO1[20],TO2[20];
    //unsigned char _GB[100000];                   // TOERBOEK.BIN
    int  BOEKSTAT[20];

    FILE* FPB;


    void FIND_OPENING(void);
    void MAKE_HASHKEY(void);
    void BOEKIN(void);
    void BOEK_BEGIN(void);
};


#endif	/* _FDSF_H */