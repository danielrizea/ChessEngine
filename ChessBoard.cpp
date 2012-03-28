/* 
 * File:   ChessBoard.cpp
 * Author: Flip4it chess
 * 
 * Created on March 10, 2010, 9:11 PM
 */

#include "ChessBoard.h"
#include "Tools.h"
#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//DEBUG
extern FILE* debug_file;

extern Game joc;

ChessBoard::ChessBoard() {

}


 /*Actualizeaza ChessBoard-ul curent in conformitate cu MOVE.
 */
void ChessBoard::updateBoard(MOVE mutare, COLOR culoare_curenta){

    //TODO
    //return this->clone();

    PIECE myPiece;
    BITBOARD bitPoz;
    BITBOARD pieceBoard;
    char poz;
    int x;
    int rocada=0;   //se considera ca mutarea nu a fost rocada

   
    //determinarea piesei care este mutata 
    if(mutare[0]=='N')     
        myPiece=KNIGHT;
    else
        if(mutare[0]=='B')
            myPiece=BISHOP;
        else
            if(mutare[0]=='R')
                myPiece=ROOK;
            else
                if(mutare[0]=='Q')
                    myPiece=QUEEN;
                else
                    if(mutare[0]=='K')
                        myPiece=KING;
                    else
                        if(mutare[0]!='O')
                            myPiece=PAWN;
                        else    //rocada
                            rocada=1;
    if(rocada==0)   //nu e rocada
    {
    //piese diferite de pioni
    if(myPiece!=PAWN)
    {
        if(mutare[1]>='a' && mutare [1]<='h')
        {
            if(mutare[2]>='1' && mutare[2]<='8')                    //mutare simpla a unei pese diferite de pion (Ne3)
            {
                poz=8*(mutare[2]-'0'-1)+mutare[1]-'a';            //determinarea pozitiei pe care trb sa aj piesa in bitboard
                //TO DO verificat daca pozitia unde trebuie mutata e libera
                bitPoz=getPseudoMoves(poz,myPiece);   /*returneaza bitboardul cu toate
                                                                 *pozitiile in care o piesa de tip dat
                                                                 *se poate muta din pozitia data*/
                //DEBUG:Tools::printBitBoard(bitPoz);//***
                pieceBoard=board[culoare_curenta][myPiece];     //bitboard pt piesa curenta
                //DEBUG:Tools::printBitBoard(pieceBoard); //***
                if((pieceBoard & bitPoz) != 0)             //mutare valida
                    board[culoare_curenta][myPiece]=(pieceBoard & ~(pieceBoard & bitPoz)) | ((U64) 1<<poz);   //da noua tabla, cea de dupa mutare
                else                                            //mutare gresita
                    return ;	//ERROR
                //TO DO verificare sah/mat
            }
            else    //mutare a unei piese dif de pion in cazul in care ex 2 variante cu piese pe coloane dif (Ngf3)
            {
                if(mutare[2]!='x')  //nu e mutare cu doua variante pe coloane dif si captura
                {
                    poz=8*(mutare[3]-'0'-1)+mutare[2]-'a';            //determinarea pozitiei pe care trb sa aj piesa in bitboard
                    //TO DO verif daca poz resp e libera
                    bitPoz=getPseudoMoves(poz,myPiece);   /*returneaza bitboardul cu toate
                                                                 *pozitpoziile in care o piesa de tip dat
                                                                 *se poate muta din pozitia data*/
                    pieceBoard=board[culoare_curenta][myPiece];     //bitboard pt piesa curenta
                    if((pieceBoard & bitPoz) != 0)
                    {
                        x=Tools::bitScan(pieceBoard);
                        if(x%8==mutare[1]-'a')    //e pe coloana care trebuie
                            board[culoare_curenta][myPiece]=(pieceBoard & ~(pieceBoard & ((U64) 1<<x)))|((U64) 1<<poz);
                        else
                        {
                            if((bitPoz & (pieceBoard & ~(pieceBoard & ((U64) 1<<x)))) != 0) //exista a doua varianta de mutare => comanda nu e gresita
                                board[culoare_curenta][myPiece]=(pieceBoard & ((U64) 1<<x))|((U64) 1<<poz);
                            else    //nu exista o a doua varianta valida
                                return ; //ERROR
                        }
                    }
                    else    //nu exista nicio varianta valida
                        return ;    //ERROR
                    //to do sah/mat
                }
                else    //mutare cu 2 var pe coloane dif si cu captura (Ngxf3)
                {
                    //update tabla de culoare curenta
                    poz=8*(mutare[4]-'0'-1)+mutare[3]-'a';            //determinarea pozitiei pe care trb sa aj piesa in bitboard
                    //TO DO verif daca poz resp e libera
                    bitPoz=getPseudoMoves(poz,myPiece);   /*returneaza bitboardul cu toate
                                                                 *pozitpoziile in care o piesa de tip dat
                                                                 *se poate muta din pozitia data*/
                    pieceBoard=board[culoare_curenta][myPiece];     //bitboard pt piesa curenta
                    if((pieceBoard & bitPoz) != 0)
                    {
                        x=Tools::bitScan(pieceBoard);
                        if(x%8==mutare[1]-'a')    //e pe coloana care trebuie
                            board[culoare_curenta][myPiece]=(pieceBoard & ~(pieceBoard & ((U64) 1<<x)))|((U64) 1<<poz);
                        else
                        {
                            if((bitPoz & (pieceBoard & ~(pieceBoard & ((U64) 1<<x)))) != 0) //exista a doua varianta de mutare => comanda nu e gresita
                                board[culoare_curenta][myPiece]=(pieceBoard & ((U64) 1<<x))|((U64) 1<<poz);
                            else    //nu exista o a doua varianta valida
                                return ; //ERROR
                        }
                    }
                    else    //nu exista nicio varianta valida
                        return ;    //ERROR

                //update tabla de culoare opusa
                    //se afla tipul piesei capturate si se modifica bitboadr-ul corespunzator tipului respectiv de piesa
                    if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                        board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                    else
                        if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                            board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                        else
                            if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                    board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                        board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                                    else    //rege - nu poate fi capturat => mutare ilegala
                                        return ;    //ERROR
                                    
                    //TO DO sah/mat
                }
            }
        }
        else
        {
            if(mutare[1]>='1' && mutare [1]<='8')   //mutare a unei piese dif de pion in cazul in care ex 2 variante cu piese pe linii dif (N5f3 sau N5xf3)
            {
                if(mutare[2]!='x')  //nu e cu captura N5f3
                {
                    poz=8*(mutare[3]-'0'-1)+mutare[2]-'a';            //determinarea pozitiei pe care trb sa aj piesa in bitboard
                    //TO DO verif daca poz resp e libera
                    bitPoz=getPseudoMoves(poz,myPiece);   /*returneaza bitboardul cu toate
                                                                 *pozitiile in care o piesa de tip dat
                                                                 *se poate muta din pozitia data*/
                    pieceBoard=board[culoare_curenta][myPiece];     //bitboard pt piesa curenta
                    if((pieceBoard & bitPoz) != 0)
                    {
                        x=Tools::bitScan(pieceBoard);
                        if(x/8+1==mutare[1]-'0')    //am gasit piesa pe linia de pe care se specifica ca se face mutare
                            board[culoare_curenta][myPiece]=(pieceBoard & ~(pieceBoard & ((U64) 1<<x)))|((U64) 1<<poz);
                        else
                        {
                            if((bitPoz & (pieceBoard & ~(pieceBoard & ((U64) 1<<x)))) != 0) //exista a doua varianta de mutare => comanda nu e gresita
                                board[culoare_curenta][myPiece]=(pieceBoard & ((U64) 1<<x))|((U64) 1<<poz);
                            else
                            {
                                //return *this;
                            }
                        }
                    }
                    else
                        return ;    //ERROR
                }
                else    //mutare cu 2 var pe linii dif cu captura (N5xf3)
                {
                    //update tabla de culoare curenta
                    poz=8*(mutare[4]-'0'-1)+mutare[3]-'a';            //determinarea pozitiei pe care trb sa aj piesa in bitboard
                    //TO DO verif daca poz resp e libera
                    bitPoz=getPseudoMoves(poz,myPiece);   /*returneaza bitboardul cu toate
                                                                 *pozitiile in care o piesa de tip dat
                                                                 *se poate muta din pozitia data*/
                    pieceBoard=board[culoare_curenta][myPiece];     //bitboard pt piesa curenta
                    if((pieceBoard & bitPoz) != 0)
                    {
                        x=Tools::bitScan(pieceBoard);
                        if(x/8+1==mutare[1]-'0')    //am gasit piesa pe linia de pe care se specifica ca se face mutare
                            board[culoare_curenta][myPiece]=(pieceBoard & ~(pieceBoard & ((U64) 1<<x)))|((U64) 1<<poz);
                        else
                        {
                            if((bitPoz & (pieceBoard & ~(pieceBoard & ((U64) 1<<x)))) != 0) //exista a doua varianta de mutare => comanda nu e gresita
                                board[culoare_curenta][myPiece]=(pieceBoard & ((U64) 1<<x))|((U64) 1<<poz);
                            else
                            {
                                //return *this;
                            }
                        }
                    }
                    else
                        return ;    //ERROR

                //update tabla de culoare opusa
                    //se afla tipul piesei capturate si se modifica bitboadr-ul corespunzator tipului respectiv de piesa
                    if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                        board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                    else
                        if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                            board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                        else
                            if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                    board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                        board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                                    else    //rege - nu poate fi capturat => mutare ilegala
                                        return ;    //ERROR

                    //TO DO sah/mat
                }
            }
            else    //mutare simpla cu captura (Nxe3)
            {
                poz=8*(mutare[3]-'0'-1)+mutare[2]-'a';            //determinarea pozitiei pe care trb sa aj piesa in bitboard
                //TO DO verificat daca pozitia unde trebuie mutata e libera
                bitPoz=getPseudoMoves(poz,myPiece);   /*returneaza bitboardul cu toate
                                                                 *pozitiile in care o piesa de tip dat
                                                                 *se poate muta din pozitia data*/
                //DEBUG:Tools::printBitBoard(bitPoz);//***
                pieceBoard=board[culoare_curenta][myPiece];     //bitboard pt piesa curenta
                //DEBUG:Tools::printBitBoard(pieceBoard); //***
                if((pieceBoard & bitPoz) != 0)             //mutare valida
                    board[culoare_curenta][myPiece]=(pieceBoard & ~(pieceBoard & bitPoz)) | ((U64) 1<<poz);   //da noua tabla, cea de dupa mutare
                else                                            //mutare gresita
                    return ;	//ERROR

                //update tabla de culoare opusa
                    //se afla tipul piesei capturate si se modifica bitboadr-ul corespunzator tipului respectiv de piesa
                    if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                        board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                    else
                        if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                            board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                        else
                            if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                    board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                        board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                                    else    //rege - nu poate fi capturat => mutare ilegala
                                        return ;    //ERROR

                //TO DO verificare sah/mat
            }
        }
    }
    else    // pion
    {
        pieceBoard=board[culoare_curenta][myPiece];     //bitboard pt piesa curenta

        if(strlen(mutare)==2 || (strlen(mutare)==3 && mutare[2]=='+') ||(strlen(mutare)==3 && mutare[2]=='#')  )    //mutare simpla pt pion (d5)
        {
	    //fprintf(debug_file,"Mutare simpla pion!");
            poz=8*(mutare[1]-'0'-1)+mutare[0]-'a';
            //TO DO verificat daca pozitia unde trebuie mutata e libera

            if(culoare_curenta==WHITE)  //pionul merge in sus pe tabla
            {
                if((pieceBoard & (U64)1<<(poz-8)) !=0)   //inseamna ca pionul care muta pe poz data se muta doar cu o patratica
                    board[culoare_curenta][myPiece]=((pieceBoard & ~((U64)1<<(poz-8))) | ((U64) 1<<poz));
                else    //inseamna ca pionul face prima mutare si poate sari un patrat
                    board[culoare_curenta][myPiece]=((pieceBoard & ~((U64)1<<(poz-16))) | ((U64) 1<<poz));
            }
            else    //pionul merge in jos pe tabla
            {
                if((pieceBoard & (U64)1<<(poz+8)) !=0)   //inseamna ca pionul care muta pe poz data se muta doar cu o patratica
                    board[culoare_curenta][myPiece]=((pieceBoard & ~((U64)1<<(poz+8))) | ((U64) 1<<poz));
                else    //inseamna ca pionul face prima mutare si poate sari un patrat
                    board[culoare_curenta][myPiece]=((pieceBoard & ~((U64)1<<(poz+16))) | ((U64) 1<<poz));
            }
        }
        else
            if(strlen(mutare)==4 || (strlen(mutare)==5 && mutare[4]=='+') || (strlen(mutare)==5 && mutare[4]=='#'))
            {
                if(mutare[1]=='x')  //(exd5)
                {
                    poz=8*(mutare[3]-'0'-1)+mutare[2]-'a';
                    if(mutare[0]<mutare[2]) //(axb3) => se ia de la patratul respectiv cu un pion ce vine din stanga
                    {
                        if(culoare_curenta==WHITE)  //pionul merge in sus pe tabla
                        {
                            //se actualizeaza bitboardul pt pionul jucatorului ce se afla la mutare
                            board[culoare_curenta][myPiece]=((pieceBoard & ~((U64)1<<(poz-9))) | ((U64) 1<<poz));

                            //se afla ce piesa a fost capturata (in caz ca nu a fost en passant) si i se actualizeaza tabla pt culoarea opusa
                            if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                                board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                                    board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                        board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                                    else
                                        if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                            board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                        else
                                            if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                                board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                             //en passant pt ca nu era nici una din piesele pt care se considera captura a fi legala acolo
                             //se cauta ce piesa era langa pozitia de unde pleaca pionul
                                            else
                                                if((board[1-culoare_curenta][PAWN] & ((U64) 1<<(poz-8))) != 0)    //pion
                                                        board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<(poz-8)));
                                                else
                                                    if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<(poz-8))) != 0)    //cal
                                                        board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<(poz-8)));
                                                    else
                                                        if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<(poz-8))) != 0)    //nebun
                                                            board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<(poz-8)));
                                                        else
                                                            if((board[1-culoare_curenta][ROOK] & ((U64) 1<<(poz-8))) != 0)  //tura
                                                                board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<(poz-8)));
                                                            else
                                                                if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<(poz-8))) != 0) //regina
                                                                    board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<(poz-8)));
                                           /* else //rege - nu poate fi capturat => mutare ilegala
                                                return *this;    //ERROR*/
                        }
                        else    //pionul merge in jos pe tabla
                        {
                            
                            //se actualizeaza bitboardul pt pionul jucatorului ce se afla la mutare
                            board[culoare_curenta][myPiece]=((pieceBoard & ~((U64)1<<(poz+7))) | ((U64) 1<<poz));

                            //se afla ce piesa a fost capturata si i se actualizeaza tabla pt culoarea opusa
                            if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                                board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                                    board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                        board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                                    else
                                        if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                            board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                        else
                                            if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                                board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                             //en passant pt ca nu era nici una din piesele pt care se considera captura a fi legala acolo
                             //se cauta ce piesa era langa pozitia de unde pleaca pionul
                                            else
                                                if((board[1-culoare_curenta][PAWN] & ((U64) 1<<(poz+8))) != 0)    //pion
                                                        board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<(poz+8)));
                                                else
                                                    if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<(poz+8))) != 0)    //cal
                                                        board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<(poz+8)));
                                                    else
                                                        if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<(poz+8))) != 0)    //nebun
                                                            board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<(poz+8)));
                                                        else
                                                            if((board[1-culoare_curenta][ROOK] & ((U64) 1<<(poz+8))) != 0)  //tura
                                                                board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<(poz+8)));
                                                            else
                                                                if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<(poz+8))) != 0) //regina
                                                                    board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<(poz+8)));
                                           /* else //rege - nu poate fi capturat => mutare ilegala
                                                return *this;    //ERROR*/
                        }
                    }
                    else    //(exd5) => se ia de la patratul rescpectiv cu un pion ce vine din dreapta
                    {
                        if(culoare_curenta==WHITE)  //pionul merge in sus pe tabla
                        {
                            //se actualizeaza bitboardul pt pionul jucatorului ce se afla la mutare
                            board[culoare_curenta][myPiece]=((pieceBoard & ~((U64)1<<(poz-7))) | ((U64) 1<<poz));

                            //se afla ce piesa a fost capturata si i se actualizeaza tabla pt culoarea opusa
                            if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                                board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                                    board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                        board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                                    else
                                        if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                            board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                        else
                                            if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                                board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                             //en passant pt ca nu era nici una din piesele pt care se considera captura a fi legala acolo
                             //se cauta ce piesa era langa pozitia de unde pleaca pionul
                                            else
                                                if((board[1-culoare_curenta][PAWN] & ((U64) 1<<(poz-8))) != 0)    //pion
                                                        board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<(poz-8)));
                                                else
                                                    if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<(poz-8))) != 0)    //cal
                                                        board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<(poz-8)));
                                                    else
                                                        if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<(poz-8))) != 0)    //nebun
                                                            board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<(poz-8)));
                                                        else
                                                            if((board[1-culoare_curenta][ROOK] & ((U64) 1<<(poz-8))) != 0)  //tura
                                                                board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<(poz-8)));
                                                            else
                                                                if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<(poz-8))) != 0) //regina
                                                                    board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<(poz-8)));
                                           /* else //rege - nu poate fi capturat => mutare ilegala
                                                return *this;    //ERROR*/
                        }
                        else    //pionul merge in jos pe tabla
                        {
                           
                            //se actualizeaza bitboardul pt pionul jucatorului ce se afla la mutare
                            board[culoare_curenta][myPiece]=((pieceBoard & ~((U64)1<<(poz+9))) | ((U64) 1<<poz));

                            //se afla ce piesa a fost capturata si i se actualizeaza tabla pt culoarea opusa
                            if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                                board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                                    board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                        board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                                    else
                                        if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                            board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                        else
                                            if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                                board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                             //en passant pt ca nu era nici una din piesele pt care se considera captura a fi legala acolo
                             //se cauta ce piesa era langa pozitia de unde pleaca pionul
                                            else
                                                if((board[1-culoare_curenta][PAWN] & ((U64) 1<<(poz+8))) != 0)    //pion
                                                        board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<(poz+8)));
                                                else
                                                    if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<(poz+8))) != 0)    //cal
                                                        board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<(poz+8)));
                                                    else
                                                        if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<(poz+8))) != 0)    //nebun
                                                            board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<(poz+8)));
                                                        else
                                                            if((board[1-culoare_curenta][ROOK] & ((U64) 1<<(poz+8))) != 0)  //tura
                                                                board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<(poz+8)));
                                                            else
                                                                if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<(poz+8))) != 0) //regina
                                                                    board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<(poz+8)));
                                           /* else //rege - nu poate fi capturat => mutare ilegala
                                                return *this;    //ERROR*/
                        }
                    }
                }
                else    //(e1=Q)
                {
                    poz=8*(mutare[1]-'0'-1)+mutare[0]-'a';
                    if(culoare_curenta==WHITE)  //se muta in sus pe tabla
                    {
                        board[culoare_curenta][myPiece]=(pieceBoard & ~((U64) 1<<(poz-8))); //se sterge pionul din bitboard-ul de pioni
                        //in functie e piesa in care se transforma se updateaza tabla piesei respective
                        if(mutare[3]=='Q')
                            board[culoare_curenta][QUEEN]=((board[culoare_curenta][QUEEN] | ((U64) 1<<poz)));
                        else
                            if(mutare[3]=='R')
                                board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<poz)));
                            else
                                if(mutare[3]=='B')
                                    board[culoare_curenta][BISHOP]=((board[culoare_curenta][BISHOP] | ((U64) 1<<poz)));
                                else
                                    board[culoare_curenta][KNIGHT]=((board[culoare_curenta][KNIGHT] | ((U64) 1<<poz)));
                    }
                    else    //se muta in jos pe tabla
                    {
                        board[culoare_curenta][myPiece]=(pieceBoard & ~((U64) 1<<(poz+8))); //se sterge pionul din bitboard-ul de pioni
                        //in functie e piesa in care se transforma se updateaza tabla piesei respective
                        if(mutare[3]=='Q')
                            board[culoare_curenta][QUEEN]=((board[culoare_curenta][QUEEN] | ((U64) 1<<poz)));
                        else
                            if(mutare[3]=='R')
                                board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<poz)));
                            else
                                if(mutare[3]=='B')
                                    board[culoare_curenta][BISHOP]=((board[culoare_curenta][BISHOP] | ((U64) 1<<poz)));
                                else
                                    board[culoare_curenta][KNIGHT]=((board[culoare_curenta][KNIGHT] | ((U64) 1<<poz)));
                    }
                }
            }
            else    //(dxe1=Q)
            {
                    poz=8*(mutare[3]-'0'-1)+mutare[2]-'a';
                    if(mutare[0]<mutare[2]) //se ia de la patratul respectiv cu un pion ce vine din stanga
                    {
                        if(culoare_curenta==WHITE)  //pionul merge in sus pe tabla
                        {
                            //se actualizeaza bitboardul pt pionul jucatorului ce se afla la mutare
                            board[culoare_curenta][myPiece]=(pieceBoard & ~((U64)1<<(poz-9)));

                            //se afla ce piesa a fost capturata si i se actualizeaza tabla pt culoarea opusa
                            if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                                board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                                    board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                        board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                                    else
                                        if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                            board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                        else
                                            if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                                board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                                            else    //rege - nu poate fi capturat => mutare ilegala
                                                return ;    //ERROR
                             //in functie de promovare se actualizeaza tabla piesei in care s-a promovat pionul
                            if(mutare[5]=='Q')
                                board[culoare_curenta][QUEEN]=((board[culoare_curenta][QUEEN] | ((U64) 1<<poz)));
                            else
                                if(mutare[5]=='R')
                                    board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<poz)));
                                else
                                    if(mutare[5]=='B')
                                        board[culoare_curenta][BISHOP]=((board[culoare_curenta][BISHOP] | ((U64) 1<<poz)));
                                    else
                                        board[culoare_curenta][KNIGHT]=((board[culoare_curenta][KNIGHT] | ((U64) 1<<poz)));
                        }
                        else    //pionul merge in jos pe tabla
                        {
                            //se actualizeaza bitboardul pt pionul jucatorului ce se afla la mutare adica se sterge pionul ce va fi 
                            board[culoare_curenta][myPiece]=(pieceBoard & ~((U64)1<<(poz+7)));

                            //se afla ce piesa a fost capturata si i se actualizeaza tabla pt culoarea opusa
                            if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                                board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                                    board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                        board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                                    else
                                        if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                            board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                        else
                                            if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                                board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                                            else    //rege - nu poate fi capturat => mutare ilegala
                                                return ;    //ERROR
                                //in functie de promovare se actualizeaza tabla piesei in care s-a promovat pionul
                                if(mutare[5]=='Q')
                                    board[culoare_curenta][QUEEN]=((board[culoare_curenta][QUEEN] | ((U64) 1<<poz)));
                                else
                                    if(mutare[5]=='R')
                                        board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<poz)));
                                    else
                                        if(mutare[5]=='B')
                                            board[culoare_curenta][BISHOP]=((board[culoare_curenta][BISHOP] | ((U64) 1<<poz)));
                                        else
                                            board[culoare_curenta][KNIGHT]=((board[culoare_curenta][KNIGHT] | ((U64) 1<<poz)));
                        }
                    }
                    else    //se ia de la patratul rescpectiv cu un pion ce vine din dreapta
                    {
                        if(culoare_curenta==WHITE)  //pionul merge in sus pe tabla
                        {
                            //se actualizeaza bitboardul pt pionul jucatorului ce se afla la mutare
                            board[culoare_curenta][myPiece]=(pieceBoard & ~((U64)1<<(poz-7)));

                            //se afla ce piesa a fost capturata si i se actualizeaza tabla pt culoarea opusa
                            if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                                board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                                    board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                        board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                                    else
                                        if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                            board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                        else
                                            if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                                board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                                            else    //rege - nu poate fi capturat => mutare ilegala
                                                return ;    //ERROR;
                            //in functie de promovare se actualizeaza tabla piesei in care s-a promovat pionul
                            if(mutare[5]=='Q')
                                board[culoare_curenta][QUEEN]=((board[culoare_curenta][QUEEN] | ((U64) 1<<poz)));
                            else
                                if(mutare[5]=='R')
                                    board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<poz)));
                                else
                                    if(mutare[5]=='B')
                                        board[culoare_curenta][BISHOP]=((board[culoare_curenta][BISHOP] | ((U64) 1<<poz)));
                                    else
                                        board[culoare_curenta][KNIGHT]=((board[culoare_curenta][KNIGHT] | ((U64) 1<<poz)));
                        }
                        else    //pionul merge in jos pe tabla
                        {
                            //se actualizeaza bitboardul pt pionul jucatorului ce se afla la mutare
                            board[culoare_curenta][myPiece]=(pieceBoard & ~((U64)1<<(poz+9)));

                            //se afla ce piesa a fost capturata si i se actualizeaza tabla pt culoarea opusa
                            if((board[1-culoare_curenta][PAWN] & ((U64) 1<<poz)) != 0)    //pion
                                board[1-culoare_curenta][PAWN]=(board[1-culoare_curenta][PAWN] & ~((U64) 1<<poz));
                            else
                                if((board[1-culoare_curenta][KNIGHT] & ((U64) 1<<poz)) != 0)    //cal
                                    board[1-culoare_curenta][KNIGHT]=(board[1-culoare_curenta][KNIGHT] & ~((U64) 1<<poz));
                                else
                                    if((board[1-culoare_curenta][BISHOP] & ((U64) 1<<poz)) != 0)    //nebun
                                        board[1-culoare_curenta][BISHOP]=(board[1-culoare_curenta][BISHOP] & ~((U64) 1<<poz));
                                    else
                                        if((board[1-culoare_curenta][ROOK] & ((U64) 1<<poz)) != 0)  //tura
                                            board[1-culoare_curenta][ROOK]=(board[1-culoare_curenta][ROOK] & ~((U64) 1<<poz));
                                        else
                                            if((board[1-culoare_curenta][QUEEN] & ((U64) 1<<poz)) != 0) //regina
                                                board[1-culoare_curenta][QUEEN]=(board[1-culoare_curenta][QUEEN] & ~((U64) 1<<poz));
                                            else    //rege - nu poate fi capturat => mutare ilegala
                                                return ;    //ERROR
                            //in functie de promovare se actualizeaza tabla piesei in care s-a promovat pionul
                            if(mutare[5]=='Q')
                                board[culoare_curenta][QUEEN]=((board[culoare_curenta][QUEEN] | ((U64) 1<<poz)));
                            else
                                if(mutare[5]=='R')
                                    board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<poz)));
                                else
                                    if(mutare[5]=='B')
                                        board[culoare_curenta][BISHOP]=((board[culoare_curenta][BISHOP] | ((U64) 1<<poz)));
                                    else
                                        board[culoare_curenta][KNIGHT]=((board[culoare_curenta][KNIGHT] | ((U64) 1<<poz)));
                        }
                    }
            }
    }
    }
    else    //rocada
    {
        if(strlen(mutare)==3)   //rocada mica O-O
        {
            if(culoare_curenta==WHITE)
            {
                board[culoare_curenta][KING]=((U64) 1<<6);  //muta regele pe coloana g
                board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<5)) & ~((U64) 1<<7));   //pune tura pe coloana f si o sterge de p h
            }
            else
            {
                board[culoare_curenta][KING]=((U64) 1<<62);  //muta regele pe coloana g
                board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<61)) & ~((U64) 1<<63));   //pune tura pe coloana f si o sterge de p h
            }
        }
        else    //rocada mare O-O-O
        {
            if(culoare_curenta==WHITE)
            {
                board[culoare_curenta][KING]=((U64) 1<<2);  //muta regele pe coloana c
                board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<3)) & ~((U64) 1));   //pune tura pe coloana d si o sterge de p a
            }
            else
            {
                board[culoare_curenta][KING]=((U64) 1<<58);  //muta regele pe coloana c
                board[culoare_curenta][ROOK]=((board[culoare_curenta][ROOK] | ((U64) 1<<59)) & ~((U64) 1<<56));   //pune tura pe coloana f si o sterge de p h
            }
        }
    }
}

//Functia ce returneaza codul SAN asociat unei mutari (primeste pozitia de la 0-63
//a piesei pe tabla si pozitia in care va fi mutata tot de la 0-63, tipul piesei si
//daca mutand piesa astfel va fi luata o piesa de culoare opusa.
MOVE ChessBoard::getSANofMove(COLOR culoare_curenta, int startPoz, int endPoz, PIECE myPiece)
{
    int isPieceThere= ((getAllBoardOf(1-culoare_curenta) & ((U64)1<<endPoz))!=0);
    BITBOARD bitPoz;
    BITBOARD pieceBoard;
    MOVE expr=(char*)calloc(10,sizeof(char));   //sirul cu expresia SAN
    int x,y;

	if(myPiece==KING && startPoz==ROCADA_MICA)
        {strcpy(expr,"O-O");
                return expr;
        }
	else
	if(myPiece==KING && startPoz==ROCADA_MARE)
        {strcpy(expr,"O-O-O");
        return expr;
        }
	else
    if(myPiece==KNIGHT)
        expr[0]='N';
    else
        if(myPiece==BISHOP)
            expr[0]='B';
        else
            if(myPiece==ROOK)
                expr[0]='R';
            else
                if(myPiece==QUEEN)
                    expr[0]='Q';
                else
                    if(myPiece==KING)
                        expr[0]='K';
                    //altfel e pion si deci nu are majuscula la inceput expresiei

    //pentru piesa diferita de pion
    if(myPiece!=PAWN)
    {
        bitPoz=getPseudoMoves(endPoz,myPiece);   /*returneaza bitboardul cu toate
                                                  *pozitiile in care o piesa de tip dat
                                                  *se poate muta din pozitia data. Adica care 
                                                  *pot fi pozitiile initiale*/
		
		pieceBoard=board[culoare_curenta][myPiece];     //bitboard pt piesa curenta
		
        x=Tools::bitScan((U64) (pieceBoard & bitPoz));   //cauta primul bit diferit de 1
		
        if(( (U64) (pieceBoard & (~((U64) 1<<x))) & bitPoz ) == 0)   //nu sunt 2 piese de acelasi tip care sa poata ajunge in pozitia finala
        {
            if(isPieceThere==1)     //se ia piesa de la pozitia data (Nxc3)
            {
                expr[1]='x';
                expr[2]=(char) (endPoz%8 + (int) 'a');
                expr[3]=(char) (endPoz/8 + 1 + (int) '0');

//                if(isCheckmate(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
//                    expr[4]='#';//Nxc3#
//                else
//                    if(isCheck(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
//                        expr[4]='+';//Nxc3+
            }
            else    //nu se ia piesa (Nc3)
            {
                expr[1]=(char) (endPoz%8 + (int) 'a');  //coloana
                expr[2]=(char) (endPoz/8 + 1 + (int) '0');//linie
//                if(isCheckmate(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
//                    expr[3]='#';    //Nc3#
//                else
//                    if(isCheck(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
//                        expr[3]='+';//Nc3+
            }
        }
        else    //sunt doua piese de acelasi fel care sa poata ajunge pe pozitia respectiva (Ngf3 N5f3 sau cu x)
        {
            if(x==startPoz) //primul element de 1 din bitBoard gasit e chiar cel pe care vrem sa-l mutam
            {
                //aflam pozitia celuilalt
                x=Tools::bitScan((U64)(pieceBoard & (~((U64) 1<<x))) & bitPoz );

                //se verifica daca sunt pe aceeasi linie (=>Ngf3), coloana (=>N5f3) sau nu au linia respectiv coloana comuna (=>Ngf3)
                y=x%8 + (int) 'a';
                if(y==startPoz%8 + (int) 'a')    //daca au coloana comuna
                    expr[1]=(char) (x/8 + 1 + (int) '0');   //se pune linia
                else
                    expr[1]=(char) (y); //se pune coloana


            }
            else    //primul element de 1 gasit e de fapt pozitia ceileilalte piese de acest tip
            {
                //se verifica daca sunt pe aceeasi linie (=>Ngf3), coloana (=>N5f3) sau nu au linia respectiv coloana comuna (=>Ngf3)
                y=x%8 + (int) 'a';
                if(y==startPoz%8 + (int) 'a')    //daca au coloana comuna
                    expr[1]=(char) (x/8 + 1 + (int) '0');   //se pune linia
                else
                    expr[1]=(char) (y); //se pune coloana
            }
            if(isPieceThere==1)     //se ia piesa de la pozitia data (Ngxc3 sau N5xc3)
            {
                expr[2]='x';
                expr[3]=(char) (endPoz%8 + (int) 'a');
                expr[4]=(char) (endPoz/8 + 1 + (int) '0');

            }
            else    //nu se ia piesa (Nfc3 sau N5c3)
            {
                expr[2]=(char) (endPoz%8 + (int) 'a');
                expr[3]=(char) (endPoz/8 + 1 + (int) '0');
//                if(isCheckmate(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
//                    expr[4]='#';//e mat
//                else
//                    if(isCheck(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
//                        expr[4]='+';//e sah
            }
        }
    }
    else // piesa e pion
    {

        int ls,le,cs,ce;    //linie start, linie end, coloana start, coloana end, pomov=0 => nu apare promovare
        cs=startPoz%8 + (int) 'a';
        ce=endPoz%8 + (int) 'a';
        ls=startPoz/8 + 1 + (int) '0';
        le=endPoz/8 + 1 + (int) '0';
//SAH/MAT dupa ce se implementeaza in functia de verificare
        if(culoare_curenta == WHITE)    //merge in sus pe tabla
        {
            if(endPoz-startPoz==8 || endPoz-startPoz==16)  //pionul merge in fata o patratica sau 2 => nici nu ia piesa (d5) sau (d6)
            {
                expr[0]=ce;
                expr[1]=le;
                /*if(isCheckmate(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
                    if(promov==0)
                        expr[2]='#';//e mat
                    else
                        expr[4]='#';//mat
                else
                    if(isCheck(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
                        if(promov==0)
                            expr[2]='+';//e sah
                        else
                            expr[4]='+';//e sah*/
            }
            else
                if(endPoz-startPoz==7 || endPoz-startPoz==9)  //pionul ia in stanga (dxc3) sau dreapta (dxe3) - intra si en passant aici
                {
                    expr[0]=cs;
                    expr[1]='x';
                    expr[2]=ce;
                    expr[3]=le;
                    /*if(isCheckmate(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
                        if(promov==0)
                            expr[4]='#';//e mat
                        else
                            expr[6]='#';//mat
                    else
                        if(isCheck(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
                            if(promov==0)
                                expr[4]='+';//e sah
                            else
                                expr[6]='+';//sah*/
                }
        }
        else //merge in jos pe tabla
        {
            if(startPoz-endPoz==8 || startPoz-endPoz==16)  //pionul merge in fata o patratica sau 2 => nici nu ia piesa (d5) sau (d6)
            {
                expr[0]=ce;
                expr[1]=le;
                /*if(isCheckmate(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
                    if(promov==0)
                        expr[2]='#';//e mat
                    else
                        expr[4]='#';//mat
                else
                    if(isCheck(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
                        if(promov==0)
                            expr[2]='+';//e sah
                        else
                            expr[4]='+';//sah*/
            }
            else
                if(startPoz-endPoz==7 || startPoz-endPoz==9)  //pionul ia in stanga (dxc3) sau dreapta (dxe3) - intra si en passant aici
                {
                    expr[0]=cs;
                    expr[1]='x';
                    expr[2]=ce;
                    expr[3]=le;
                    /*if(isCheckmate(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
                        if(promov==0)
                            expr[4]='#';//e mat
                        else
                            expr[6]='#';//mat
                    else
                        if(isCheck(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
                            if(promov==0)
                                expr[4]='+';//e sah
                            else
                                expr[6]='+'//sah*/
                }
        }

    }

    //Verificare promovare pion
    if(myPiece==PAWN)
    {
	//daca e pion alb si a ajuns pe ultimul rand
	if(culoare_curenta==WHITE && endPoz/8==7)
	    strcat(expr,"=Q");
	else
	    //daca e pion negru si a ajuns pe primul rand
	    if(culoare_curenta==BLACK && endPoz/8==0)
		strcat(expr,"=Q");
    }

//    if(isCheckmate(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
//	strcat(expr,"#");
//    else
          if(isCheck(1-culoare_curenta, Tools::bitScan(board[1-culoare_curenta][KING])))
	      strcat(expr,"+");

    return expr;
}

//functia realizeaza o mutare, primind pozitia initiala, finala, piesa mutata si culoarea
void ChessBoard::doMove(int pozInitiala, int pozFinala, int PIECE, COLOR culoare)
{
    int i;
    BITBOARD final=(U64)1<<pozFinala;
	//Stergem pozitia initiala
	board[culoare][PIECE]=board[culoare][PIECE] & (~((U64)1 << pozInitiala));
	//Completam pozitia finala
	board[culoare][PIECE]=board[culoare][PIECE] | final;
    //Stergem piesa luata de pe tabla inamicului
    for(i=PAWN;i<KING;i++)
        board[1-culoare][i]=board[1-culoare][i] & (~final);
	//Modificam flagurile in concordanta pentru rocada
	//Pentru tura
	if(PIECE==ROOK && pozInitiala==0 && culoare==WHITE)
		setFlags(getFlags() | F_CASTLR_L_W);
	if(PIECE==ROOK && pozInitiala==7 && culoare==WHITE)
		setFlags(getFlags() | F_CASTLR_R_W);
	if(PIECE==ROOK && pozInitiala==56 && culoare==BLACK)
		setFlags(getFlags() | F_CASTLR_L_B);
	if(PIECE==ROOK && pozInitiala==63 && culoare==BLACK)
		setFlags(getFlags() | F_CASTLR_L_W);
	//Pentru rege
	if(PIECE==KING && culoare==WHITE)
		setFlags(getFlags() & ~F_CASTLINGW);
	if(PIECE==KING && culoare==BLACK)
		setFlags(getFlags() & ~F_CASTLINGB);


}



int ChessBoard::canDoMove(int poz, int pozpr, int culoare)
{

    // poz piesa cere o muti
    // poz pr piesa care va fi atacata dupa
    

    BITBOARD tabla;   //tabla in care se retine cum arata in realitate tabla pt piesa pt care verificam ce se intampla daca se muta

    //verificam ce piesa e pe poz
    if((board[culoare][QUEEN] & ((U64) 1<<poz)) != 0)   //e regina
    {   
            tabla=board[culoare][QUEEN];

            //modificam pentru a putea apela isCheck
            board[culoare][QUEEN]=(board[culoare][QUEEN] & ~((U64) 1<<poz));

            if(isCheck(culoare,pozpr))  //daca piesa de pe pozpr e atacata
            {
                //aducem board la forma initiala pentru piesa pe care am mutat-o
                board[culoare][QUEEN]=tabla;
                return 0;//nu se poate face mutarea fara ca piesa sa fie atacata (in cazul regelui deloc)
            }
            //aducem board la forma initiala pentru piesa pe care am mutat-o
            board[culoare][QUEEN]=tabla;
            return 1;//se poate face - piesa resp nu va fi atacata
    }
    else
        if((board[culoare][ROOK] & ((U64) 1<<poz)) != 0)   //e tura
        {
            tabla=board[culoare][ROOK];

            //modificam pentru a putea apela isCheck
            board[culoare][ROOK]=(board[culoare][ROOK] & ~((U64) 1<<poz));

            if(isCheck(culoare,pozpr))  //daca piesa de pe pozpr e atacata
            {
                //aducem board la forma initiala pentru piesa pe care am mutat-o
                board[culoare][ROOK]=tabla;
                return 0;//nu se poate face mutarea fara ca piesa sa fie atacata (in cazul regelui deloc)
            }
            //aducem board la forma initiala pentru piesa pe care am mutat-o
            board[culoare][ROOK]=tabla;
            return 1;//se poate face - piesa resp nu va fi atacata
        }
        else
            if((board[culoare][KNIGHT] & ((U64) 1<<poz)) != 0)   //e cal
            {
                tabla=board[culoare][KNIGHT];

                //modificam pentru a putea apela isCheck
                board[culoare][KNIGHT]=(board[culoare][KNIGHT] & ~((U64) 1<<poz));

                if(isCheck(culoare,pozpr))  //daca piesa de pe pozpr e atacata
                {
                    //aducem board la forma initiala pentru piesa pe care am mutat-o
                    board[culoare][KNIGHT]=tabla;
                    return 0;//nu se poate face mutarea fara ca piesa sa fie atacata (in cazul regelui deloc)
                }
                //aducem board la forma initiala pentru piesa pe care am mutat-o
                board[culoare][KNIGHT]=tabla;
                return 1;//se poate face - piesa resp nu va fi atacata
            }
            else
                if((board[culoare][BISHOP] & ((U64) 1<<poz)) != 0)   //e nebun
                {
                    tabla=board[culoare][BISHOP];

                    //modificam pentru a putea apela isCheck
                    board[culoare][BISHOP]=(board[culoare][BISHOP] & ~((U64) 1<<poz));

                    if(isCheck(culoare,pozpr))  //daca piesa de pe pozpr e atacata
                    {
                        //aducem board la forma initiala pentru piesa pe care am mutat-o
                        board[culoare][BISHOP]=tabla;
                        return 0;//nu se poate face mutarea fara ca piesa sa fie atacata (in cazul regelui deloc)
                    }
                    //aducem board la forma initiala pentru piesa pe care am mutat-o
                    board[culoare][BISHOP]=tabla;
                    return 1;//se poate face - piesa resp nu va fi atacata
                }
                else
                    if((board[culoare][PAWN] & ((U64) 1<<poz)) != 0)   //e pion
                    {
                        tabla=board[culoare][PAWN];

                        //modificam pentru a putea apela isCheck
                        board[culoare][PAWN]=(board[culoare][PAWN] & ~((U64) 1<<poz));

                        if(isCheck(culoare,pozpr))  //daca piesa de pe pozpr e atacata
                        {
                            //aducem board la forma initiala pentru piesa pe care am mutat-o
                            board[culoare][PAWN]=tabla;
                            return 0;//nu se poate face mutarea fara ca piesa sa fie atacata (in cazul regelui deloc)
                        }
                        //aducem board la forma initiala pentru piesa pe care am mutat-o
                        board[culoare][PAWN]=tabla;
                        return 1;//se poate face - piesa resp nu va fi atacata
                    }
                    else
                        if((board[culoare][KING] & ((U64) 1<<poz)) != 0)   //e rege
                        {
                            tabla=board[culoare][KING];

                            //modificam pentru a putea apela isCheck
                            board[culoare][KING]=(board[culoare][KING] & ~((U64) 1<<poz));

                            if(isCheck(culoare,pozpr))  //daca piesa de pe pozpr e atacata
                            {
                                //aducem board la forma initiala pentru piesa pe care am mutat-o
                                board[culoare][KING]=tabla;
                                return 0;//nu se poate face mutarea fara ca piesa sa fie atacata (in cazul regelui deloc)
                            }
                            //aducem board la forma initiala pentru piesa pe care am mutat-o
                            board[culoare][KING]=tabla;
                            return 1;//se poate face - piesa resp nu va fi atacata
                        }
                    return 1;

}

int ChessBoard::DefByOther(int culoare, int poz)
{
    //culoare e culoarea opusa a pieselor care trebuie sa apere pozitia respectiva de pe tabla
    BITBOARD pieceBoard;
    BITBOARD bitPoz;
    
    int poz1,poz2;  //se folosesc la pioni

//verificare daca piesa e aparata de QUEEN
    pieceBoard=board[1-culoare][QUEEN]; //unde e regina pe tabla
    bitPoz=getPseudoMoves(poz,QUEEN);   //de unde se poate ajunge cu regina pe pozitia respectiva

    if((pieceBoard & bitPoz)!=0)
        return 1;

//verificare daca piesa e aparata de ROOK
    pieceBoard=board[1-culoare][ROOK]; //unde sunt turele pe tabla
    bitPoz=getPseudoMoves(poz,ROOK);   //de unde se poate ajunge cu tura pe pozitia respectiva

    if((pieceBoard & bitPoz)!=0)
        return 1;
//verificare daca piesa e aparata de BISHOP
    pieceBoard=board[1-culoare][BISHOP]; //unde sunt nebunii pe tabla
    bitPoz=getPseudoMoves(poz,BISHOP);   //de unde se poate ajunge cu nebunul pe pozitia respectiva

    if((pieceBoard & bitPoz)!=0)
        return 1;
//verificare daca piesa e aparata de KNIGHT
    pieceBoard=board[1-culoare][KNIGHT]; //unde sunt caii pe tabla
    bitPoz=getPseudoMoves(poz,KNIGHT);   //de unde se poate ajunge cu calul pe pozitia respectiva

    if((pieceBoard & bitPoz)!=0)
        return 1;
//verificare daca piesa e aparata de PAWN
    pieceBoard=board[1-culoare][PAWN]; //unde sunt pionii pe tabla
    if(culoare==WHITE)  //pionul care apara e negru deci merge in jos pe tabla => trebuie sa fie pe linia de desupra piesei
    {
        poz1=poz+7;
        poz2=poz+9;

        if(poz1<=63)
            if((pieceBoard & ((U64) 1<<poz1))!=0)   //daca e pion pe poz1 => piesa e aparata
                return 1;

        if(poz2<=63)
            if((pieceBoard & ((U64) 1<<poz2))!=0)   //daca e pion pe poz2 => piesa e aparata
                return 1;
        
        return 0;
    }
    else    //pionul care apara e alb deci merge in sus pe tabla => trebuie sa fie pe linia de sub piesei
    {
        poz1=poz-7;
        poz2=poz-9;

        if(poz1>=0)
            if((pieceBoard & ((U64) 1<<poz1))!=0)   //daca e pion pe poz1 => piesa e aparata
                return 1;

        if(poz2>=0)
            if((pieceBoard & ((U64) 1<<poz2))!=0)   //daca e pion pe poz2 => piesa e aparata
                return 1;

        return 0;
    }
 
//verificare daca piesa e aparata de KING
    pieceBoard=board[1-culoare][KING]; //unde e regele pe tabla
    bitPoz=getPseudoMoves(poz,KING);   //de unde se poate ajunge cu regele pe pozitia respectiva

    if((pieceBoard & bitPoz)!=0)
        return 1;

    return 0;   //piesa de culoare 1-culore nu e aparata
}

int ChessBoard::isCheck(int culoare, int pozKing)
{

    //nu trebuie sa trateze nici o situatie
    if(pozKing==-1)
        return 0;
    //fprintf(debug_file,"Regele de pe poz %d este ",pozKing);
  //  fprintf(debug_file,"1\n");
    BITBOARD pieceBoard;
    BITBOARD bitPoz;
    BITBOARD Verificare;    //pentru a verifica daca piesa care da sah e aparata de alta piesa
    int poz;
    /*la cal nu e nevoie sa se verifice daca e aparat de o piesa pentru ca niciodata nu va fi
     * langa rege astfel in cat sa se puna problema ca acesta sa il captureze sau nu*/
    /*    int pozKing;*/


//KNIGHT
    //se verifica daca sahul e dat de cal
    pieceBoard=board[1-culoare][KNIGHT];    //tabla cu caii de culoare opusa

    //se afla tabla cu pozitiile din care ar putea un cal sa atace regele aflat la pozKing
    bitPoz=getPseudoMoves(pozKing,KNIGHT);
   
    if((bitPoz & pieceBoard)!=0)    //daca exista cel putin o piesa opusa pe una din pozitiile respective =>sah
    {
        //fprintf(debug_file,"Atacat de Knight. ");
        return 1;
    }
 //fprintf(debug_file,"2\n");
//BISHOP
    //se verifica daca sahul e dat de nebun
    pieceBoard=board[1-culoare][BISHOP];    //tabla cu nebuni de culoare opusa

    //se afla tabla cu pozitiile din care ar putea un nebun sa atace regele aflat la pozKing
    bitPoz=getPseudoMoves(pozKing,BISHOP);

    Verificare=(bitPoz & pieceBoard);
    if(Verificare!=0)    //daca exista cel putin o piesa opusa pe una din pozitiile respective =>sah
    {
        poz=Tools::bitScan(Verificare); //poate fi atacat de o singura piesa
        if(DefByOther(culoare,poz)==1)  //e aparata
    {
        //fprintf(debug_file,"Atacat de Bishop. ");
        return 1;
    }
    {
        //fprintf(debug_file,"Atacat de Bishop. ");
        return 2;
    }   //nu e aparata si poate fi capturata de rege
    }
 //fprintf(debug_file," 3");
//ROOK
    //se verifica daca sahul e dat de tura
    pieceBoard=board[1-culoare][ROOK];    //tabla cu ture de culoare opusa

    //se afla tabla cu pozitiile din care ar putea o tura sa atace regele aflat la pozKing
    bitPoz=getPseudoMoves(pozKing,ROOK);

    Verificare=(bitPoz & pieceBoard);
    if(Verificare!=0)    //daca exista cel putin o piesa opusa pe una din pozitiile respective =>sah
    {
        poz=Tools::bitScan(Verificare); //poate fi atacat de o singura piesa
        if(DefByOther(culoare,poz)==1)  //e aparata
    {
        //fprintf(debug_file,"Atacat de Rook. ");
        return 1;
    }   //sah si regele nu poate sa ia piesa
    {
        //fprintf(debug_file,"Atacat de Rook ");
        return 2;
    }  //nu e aparata si poate fi capturata de rege
    }
// fprintf(debug_file,"4\n");
//QUEEN
    //se verifica daca sahul e dat de regina
    pieceBoard=board[1-culoare][QUEEN];    //tabla cu reine de culoare opusa

    //se afla tabla cu pozitiile din care ar putea o regina sa atace regele aflat la pozKing
    bitPoz=getPseudoMoves(pozKing,QUEEN);

    Verificare=(bitPoz & pieceBoard);
    if(Verificare!=0)    //daca exista cel putin o piesa opusa pe una din pozitiile respective =>sah
    {
        poz=Tools::bitScan(Verificare); //poate fi atacat de o singura piesa
        if(DefByOther(culoare,poz)==1)  //e aparata
    {
        //fprintf(debug_file,"Atacat de Regina. ");
        return 1;
    }   //sah si regele nu poate sa ia piesa
    {
        //fprintf(debug_file,"Atacat de Regina. ");
        return 2;
    }   //nu e aparata si poate fi capturata de rege
    }
// fprintf(debug_file,"1\n");
//PAWN
    //se verifica daca sahul e dat de pion
    pieceBoard=board[1-culoare][PAWN];
    int poz1,poz2;
    poz=pozKing;

    if(culoare==WHITE)  //pionul care ataca e negru deci merge in jos pe tabla => trebuie sa fie pe linia de desupra piesei
    {
        poz1=poz+7;
        poz2=poz+9;
        if(pozKing%8 !=0 && poz1<=63) //daca regele nu e pe coloana din stanga si pozitia e valida
            if((pieceBoard & ((U64) 1<<poz1))!=0)   //daca e pion pe poz1 => piesa e atacata
            {
                //fprintf(debug_file,"Atacat de Pawn%d. ",poz1);
                if(DefByOther(culoare,poz1)==1) //pionul e aparat
                    return 1;   //sah si regele nu poate sa ia piesa
                else
                    return 2;   //sah si regele poate sa ia pionul care nu e aparat
            }
        
        if(pozKing%8!=7 && poz2<=63)    //daca regele nu e pe coloana din dreapta si pozitia e valida
            if((pieceBoard & ((U64) 1<<poz2))!=0)   //daca e pion pe poz2 => piesa e aparata
            {
                //fprintf(debug_file,"Atacat de Pawn%d. ",poz2);
                if(DefByOther(culoare,poz2)==1) //pionul e aparat
                    return 1;   //sah si regele nu poate sa ia piesa
                else
                    return 2;   //sah si regele poate sa ia pionul care nu e aparat
            }
    }
    else    //pionul care ataca e alb deci merge in sus pe tabla => trebuie sa fie pe linia de sub piesei
    {
        poz1=poz-7;
        poz2=poz-9;
        //fprintf(debug_file,"Se verifica atacul in pozitiile %d %d \n",poz1,poz2);
        //Tools::printBitBoard(pieceBoard);
        if(pozKing%8 !=7 && poz1>=0) //daca regele nu e pe coloana din dreapta si pozitia e valida
            if((pieceBoard & ((U64) 1<<poz1))!=0)   //daca e pion pe poz1 => piesa e atacata
            {//fprintf(debug_file,"e atacat de PION\n");
                //fprintf(debug_file,"Atacat de Pawn%d. ",poz1);
                if(DefByOther(culoare,poz1)==1) //pionul e aparat
                    return 1;   //sah si regele nu poate sa ia piesa
                else
                    return 2;   //sah si regele poate sa ia pionul care nu e aparat
            }

        if(pozKing%8 !=0 && poz2>=0) //daca regele nu e pe coloana din stanga si pozitia e valida
            if((pieceBoard & ((U64) 1<<poz2))!=0)   //daca e pion pe poz2 => piesa e aparata
            {
                //fprintf(debug_file,"Atacat de Pawn%d. ",poz2);
                if(DefByOther(culoare,poz2)==1) //pionul e aparat
                    return 1;   //sah si regele nu poate sa ia piesa
                else
                    return 2;   //sah si regele poate sa ia pionul care nu e aparat
            }
    }

//KING
    //se verifica daca sahul e dat de REGE
    pieceBoard=board[1-culoare][KING];    //tabla cu regele de culoare opusa

    //se afla tabla cu pozitiile din care ar putea un rege sa atace regele aflat la pozKing
    bitPoz=getPseudoMoves(pozKing,KING);

    Verificare=(bitPoz & pieceBoard);
    if(Verificare!=0)    //daca exista cel putin o piesa opusa pe una din pozitiile respective =>sah
    {
                        //fprintf(debug_file,"Atacat de King. ");
        poz=Tools::bitScan(Verificare); //poate fi atacat de o singura piesa
        if(DefByOther(culoare,poz)==1)  //e aparata
            return 1;   //sah si regele nu poate sa ia piesa
        return 2;   //nu e aparata si poate fi capturata de rege
    }
// fprintf(d

    return 0;
}

int ChessBoard::isCheckmate(int culoare,int pozKing)
{
    BITBOARD myBoard;
    int poz,poz1=-1,poz2=-1,poz3=-1,poz4=-1,poz5=-1,poz6=-1,poz7=-1,poz8=-1;

    if(culoare==WHITE)
        myBoard=ChessBoard::getWhiteBoard();
    else
        myBoard=ChessBoard::getBlackBoard();
    //boardB=ChessBoard::getBlackBoard();

    /*int pozKing;

    //aflam pozitia regelui
    pozKing=Tools::bitScan(board[culoare][KING]);*/

    /*aflam pozitiile din jurul regelui
    7 8 9
   -1 K 1
   -9-8-7*/

    poz=pozKing+7;
    if(poz<64)
        poz1=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing+8;
    if(poz<64)
        poz2=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing+9;
    if(poz<64)
        poz3=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing-1;
    if(poz>=0)
        poz4=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing+1;
    if(poz<64)
        poz5=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing-9;
    if(poz>=0)
        poz6=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing-8;
    if(poz>=0)
        poz7=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing-7;
    if(poz>=0)
        poz8=Tools::bitScan((U64) 1<<(poz));

   // fprintf(debug_file,"%d %d %d %d %d %d %d %d %d \n",pozKing,poz1,poz2,poz3,poz4,poz5,poz6,poz7,poz8);


    if(isCheck(culoare, pozKing))
        if(isCheck(culoare, poz1) || (myBoard & ((U64) 1<<poz1)))
            if(isCheck(culoare, poz2) || (myBoard & ((U64) 1<<poz2)))
                if(isCheck(culoare, poz3) || (myBoard & ((U64) 1<<poz3)))
                    if(isCheck(culoare, poz4) || (myBoard & ((U64) 1<<poz4)))
                        if(isCheck(culoare, poz5) || (myBoard & ((U64) 1<<poz5)))
                            if(isCheck(culoare, poz6) || (myBoard& ((U64) 1<<poz6)))
                                if(isCheck(culoare, poz7) || (myBoard & ((U64) 1<<poz7)))
                                    if(isCheck(culoare, poz8) || (myBoard & ((U64) 1<<poz8)))
                                        return 1;
    return 0;
}

int ChessBoard::isDraw(int culoare,int pozKing)
{
    BITBOARD boardW, boardB, bitPoz;
    int poz,poz1=-1,poz2=-1,poz3=-1,poz4=-1,poz5=-1,poz6=-1,poz7=-1,poz8=-1;
    int p;

    boardW=ChessBoard::getWhiteBoard();
    boardB=ChessBoard::getBlackBoard();

    /*aflam pozitiile din jurul regelui
    1 2 3
    4 K 5
    6 7 8*/

    poz=pozKing+7;
    if(poz<64)
        poz1=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing+8;
    if(poz<64)
        poz2=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing+9;
    if(poz<64)
        poz3=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing-1;
    if(poz>=0)
        poz4=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing+1;
    if(poz<64)
        poz5=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing-9;
    if(poz>=0)
        poz6=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing-8;
    if(poz>=0)
        poz7=Tools::bitScan((U64) 1<<(poz));

    poz=pozKing-7;
    if(poz>=0)
        poz8=Tools::bitScan((U64) 1<<(poz));

    //verificam daca regele e singura piesa de pe tabla
    if(culoare==WHITE)
        bitPoz=getWhiteBoard();
    else
        bitPoz=getBlackBoard();

    p=Tools::bitScan(bitPoz);
    bitPoz=(bitPoz & ~((U64)1<<p));  //stergem primul bit de 1 gasit pe tabla

    p=Tools::bitScan(bitPoz);
    if(p==0)    //nu mai e niciun alt bit de 1 => singura piesa pe tabla era regele
    {
        if(isCheck(culoare, poz1)!=0)
            if(isCheck(culoare, poz2)!=0)
                if(isCheck(culoare, poz3)!=0)
                    if(isCheck(culoare, poz4)!=0)
                        if(isCheck(culoare, poz5)!=0)
                            if(isCheck(culoare, poz6)!=0)
                                if(isCheck(culoare, poz7)!=0)
                                    if(isCheck(culoare, poz8)!=0)
                                        return 1;   //toate pozitiile din jurul regelui sunt atacate
    }
    return 0;
}

/*returneaza valoarea tablei curente. Se intoarce pozitiv daca tabla e mai buna pentru 'culoare' decat pentru inamic,
 *si negativ altfel.*/
int ChessBoard::evaluateBoard(COLOR culoare)
{
    int i,nr;
    int suma=0;
    BITBOARD myPozBoard,myPieceBoard;   //tabla cu pozitiile pe care poate ajunge piesa, tabla numai cu piesele de un tip
    BITBOARD opponentBoard,otherPieceBoard,otherPozBoard;
    int myPoz,poz,ok;
    int myPiece;
    
    
    BITBOARD player,oponent; //player - tabla noastra ; oponent - tabla oponentului
  //  COLOR culoare=joc.colorEngine;
    
  

   //daca e WHITE => scoatem tabla pieselor albe pentru noi
   if(culoare==WHITE)
    {
        player=getWhiteBoard();
        oponent=getBlackBoard();
    }
    else    //scoatem tabla pieselor negre pentr noi
    {
        player=getBlackBoard();
        oponent=getWhiteBoard();
    }


    //pentru fiecare pozitie de pe tabla verificam ce piesa e acolo si modificam suma tablei in fct de asta


    if(culoare==WHITE)  //daca tabla noastra e alba e mai ok sa incepem de jos in sus (castiga timp la incep)
    {
		U64 poz;
		for(i=0;i<64;i++)
		{

			poz=(U64)1<<i;
			if((poz & player)!=0)  //daca exista o piesa de-a noastra pe poz i
			{  if((poz & board[culoare][PAWN])!=0)    //daca piesa de acolo e pion
					suma+=(PAWNVAL+WHITE_PAWN_VAL[i]);
				else
					if((poz & board[culoare][KNIGHT])!=0)    //daca piesa de acolo e cal
						suma+=(KNIGHTVAL+WHITE_KNIGHT_VAL[i]);
					else
						if((poz & board[culoare][BISHOP])!=0)    //daca piesa de acolo e nebun
							suma+=(BISHOPVAL+WHITE_BISHOP_VAL[i]);
						else
							if((poz & board[culoare][ROOK])!=0)    //daca piesa de acolo e tura
								suma+=(ROOKVAL+WHITE_ROOK_VAL[i]);
							else
								if((poz & board[culoare][QUEEN])!=0)    //daca piesa de acolo e regina
									suma+=(QUEENVAL+WHITE_QUEEN_VAL[i]);
								else
									if((poz & board[culoare][KING])!=0)    //daca piesa de acolo e rege
										suma+=(KINGVAL+WHITE_KING_VAL[i]);

                        }
                        else if((poz & oponent)!=0) //daca exista o piesa de-a adversarului pe poz i
                        {
                                if((poz & board[1-culoare][PAWN])!=0)    //daca piesa de acolo e pion
                                        suma-=(PAWNVAL+BLACK_PAWN_VAL[i]);
                                else
                                        if((poz & board[1-culoare][KNIGHT])!=0)    //daca piesa de acolo e cal
                                                suma-=(KNIGHTVAL+BLACK_KNIGHT_VAL[i]);
                                        else
                                                if((poz & board[1-culoare][BISHOP])!=0)    //daca piesa de acolo e nebun
                                                        suma-=(BISHOPVAL+BLACK_BISHOP_VAL[i]);
                                                else
                                                        if((poz & board[1-culoare][ROOK])!=0)    //daca piesa de acolo e tura
                                                                suma-=(ROOKVAL+BLACK_ROOK_VAL[i]);
                                                        else
                                                                if((poz & board[1-culoare][QUEEN])!=0)    //daca piesa de acolo e regina
                                                                        suma-=(QUEENVAL+BLACK_QUEEN_VAL[i]);
                                                                else
                                                                        if((poz & board[1-culoare][KING])!=0)    //daca piesa de acolo e rege
                                                                                suma-=(KINGVAL+BLACK_KING_VAL[i]);

                        }
                }
	}
    else    //daca tabla noastra e neagra e mai ok sa incepem de sus in jos (castiga timp la incep cand piesele sunt sus
    {
        U64 poz;
        for(i=63;i>=0;i--)
        {
            poz=(U64)1<<i;
            if((poz & player)!=0)  //daca exista o piesa de-a noastra pe poz i
            {
                if((poz & board[culoare][PAWN])!=0)    //daca piesa de acolo e pion
                    suma+=(PAWNVAL+BLACK_PAWN_VAL[i]);
                else
                    if((poz & board[culoare][KNIGHT])!=0)    //daca piesa de acolo e cal
                        suma+=(KNIGHTVAL+BLACK_KNIGHT_VAL[i]);
                    else
                        if((poz & board[culoare][BISHOP])!=0)    //daca piesa de acolo e nebun
                            suma+=(BISHOPVAL+BLACK_BISHOP_VAL[i]);
                        else
                            if((poz & board[culoare][ROOK])!=0)    //daca piesa de acolo e tura
                                suma+=(ROOKVAL+BLACK_ROOK_VAL[i]);
                            else
                                if((poz & board[culoare][QUEEN])!=0)    //daca piesa de acolo e regina
                                    suma+=(QUEENVAL+BLACK_QUEEN_VAL[i]);
                                else
                                    if((poz & board[culoare][KING])!=0)    //daca piesa de acolo e rege
                                        suma+=(KINGVAL+BLACK_KING_VAL[i]);

            }
            else if((poz & oponent)!=0) //daca exista o piesa de-a adversarului pe poz i, scadem costul acesteia
            {
                if((poz & board[1-culoare][PAWN])!=0)    //daca piesa de acolo e pion
                    suma-=(PAWNVAL+WHITE_PAWN_VAL[i]);
                else
                    if((poz & board[1-culoare][KNIGHT])!=0)    //daca piesa de acolo e cal
                        suma-=(KNIGHTVAL+WHITE_KNIGHT_VAL[i]);
                    else
                        if((poz & board[1-culoare][BISHOP])!=0)    //daca piesa de acolo e nebun
                            suma-=(BISHOPVAL+WHITE_BISHOP_VAL[i]);
                        else
                            if((poz & board[1-culoare][ROOK])!=0)    //daca piesa de acolo e tura
                                suma-=(ROOKVAL+WHITE_ROOK_VAL[i]);
                            else
                                if((poz & board[1-culoare][QUEEN])!=0)    //daca piesa de acolo e regina
                                    suma-=(QUEENVAL+WHITE_QUEEN_VAL[i]);
                                else
                                    if((poz & board[1-culoare][KING])!=0)    //daca piesa de acolo e rege
                                        suma-=(KINGVAL+WHITE_KING_VAL[i]);
            }
        }
    }

    /*************Punctaje pentru MOBILITATE (Fara pion)**********/

        //QUEEN = +2 pentru fiecare patratica pe care poate ajunge
        //ROOK  = +2 pentru fiecare patratica pe care poate ajunge
        //KNIGHT= +2 pentru fiecare patratica pe care poate ajunge
        //BISHOP= +2 pentru fiecare patratica pe care poate ajunge

        for(myPiece=KNIGHT;myPiece<KING;myPiece++)
        {
        //pentru mine
            //scoatem tabla piesei 
            myPieceBoard=board[culoare][myPiece];

            //cat timp mai sunt piese de tip myPiece
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);
                //scoatem tabla cu posibilitatile de mutare pentru piesa pentru piesa
                myPozBoard=ChessBoard::getPieceMoves(myPoz,myPiece,culoare);
                //scoatem doar pozitiile pe care poate ajunge si pe care nu sunt alte piese
                otherPieceBoard=ChessBoard::getAllBoard();
                myPozBoard=(myPozBoard & (~ otherPieceBoard));

                //cat timp mai avem pozitii pe care poate ajunge
                while(myPozBoard!=0)
                {
                    poz=Tools::bitScan(myPozBoard);
                    myPozBoard=(myPozBoard & (~ ((U64) 1<<poz)));   //scoatem una din pozitiile in care putea sa ajunga piesa
                    suma+=2;
                }

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }

        //pentru oponent
            //scoatem tabla piesei
            myPieceBoard=board[1-culoare][myPiece];

            //cat timp mai sunt piese de tip myPiece
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);
                //scoatem tabla cu posibilitatile de mutare pentru piesa pentru piesa
                myPozBoard=ChessBoard::getPieceMoves(myPoz,myPiece,1-culoare);
                //scoatem doar pozitiile pe care poate ajunge si pe care nu sunt alte piese
                otherPieceBoard=ChessBoard::getAllBoard();
                myPozBoard=(myPozBoard & (~ otherPieceBoard));

                //cat timp mai avem pozitii pe care poate ajunge
                while(myPozBoard!=0)
                {
                    poz=Tools::bitScan(myPozBoard);
                    myPozBoard=(myPozBoard & (~ ((U64) 1<<poz)));   //scoatem una din pozitiile in care putea sa ajunga piesa
                    suma-=2;
                }

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }
        }

    /*************END Punctaje pentru MOBILITATE (Fara pion)**********/


    /*************Punctaje pentru PIESE BLOCATE (Fara pion)**********/

    //Pentru: BISHOP,ROOK
        for(myPiece=BISHOP;myPiece<QUEEN;myPiece++)
        {
        //pentru mine
            //scoatem tabla piesei
            myPieceBoard=board[culoare][myPiece];

            //cat timp mai sunt piese de tip myPiece
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);
                //scoatem tabla cu posibilitatile de mutare pentru piesa pentru piesa
                myPozBoard=ChessBoard::getPieceMoves(myPoz,myPiece,culoare);
                //scoatem doar pozitiile pe care poate ajunge si pe care nu sunt alte piese
                otherPozBoard=ChessBoard::getAllBoard();
                myPozBoard=(myPozBoard & (~ otherPieceBoard));

                if(myPozBoard==0)   //nu are unde muta
                {
                    if(myPiece==ROOK)
                        suma-=75;
                    else
                        if(myPiece==BISHOP)
                            suma-=150;
                }

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }

        //pentru oponent
            //scoatem tabla piesei 
            myPieceBoard=board[1-culoare][myPiece];

            //cat timp mai sunt piese de tip myPiece
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);
                //scoatem tabla cu posibilitatile de mutare pentru piesa pentru piesa
                myPozBoard=ChessBoard::getPieceMoves(myPoz,myPiece,1-culoare);
                //scoatem doar pozitiile pe care poate ajunge si pe care nu sunt alte piese
                otherPozBoard=ChessBoard::getAllBoard();
                myPozBoard=(myPozBoard & (~ otherPieceBoard));
                
                if(myPozBoard==0)   //nu are unde muta
                {
                    if(myPiece==ROOK)
                        suma+=75;
                    else
                        if(myPiece==BISHOP)
                            suma+=150;
                }

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }
        }

    /*************END Punctaje pentru PIESE BLOCATE (Fara pion)**********/


    /*************Pairs (BISHOP & KNIGHT)**********/

    //BISHOP PAIR
    /*daca avem amandoi nebunii pe tabla*/
    //pentru mine
            nr=0;

            myPieceBoard=board[culoare][BISHOP];

            //cat timp mai sunt piese de tip myPiece
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);

                nr++;   //incrementam numarul de nebuni gasiti pe tabla

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }

            if(nr==2)
                suma+=40;
    //pentru oponent
            nr=0;

            myPieceBoard=board[1-culoare][BISHOP];

            //cat timp mai sunt piese de tip myPiece
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);

                nr++;   //incrementam numarul de nebuni gasiti pe tabla

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }

            if(nr==2)
                suma-=40;

    //KNIGHT PAIR
    /*daca avem amandoi caii pe tabla*/
    //pentru mine
            nr=0;

            myPieceBoard=board[culoare][KNIGHT];

            //cat timp mai sunt piese de tip myPiece
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);

                nr++;   //incrementam numarul de nebuni gasiti pe tabla

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }

            if(nr==2)
                suma+=5;
    //pentru oponent
            nr=0;

            myPieceBoard=board[1-culoare][KNIGHT];

            //cat timp mai sunt piese de tip myPiece
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);

                nr++;   //incrementam numarul de nebuni gasiti pe tabla

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }

            if(nr==2)
                suma-=5;

    /*************END Pairs(BISHOP & KNIGHT)**********/

    
    /*************Punctaje pentru P blocati**********/
        //pentru mine
            myPieceBoard=board[culoare][PAWN];
            //cat timp mai sunt pioni
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);
                //aflam tabla cu mutari posibile pt pion
                myPozBoard=ChessBoard::getPieceMoves(myPoz,PAWN,culoare);

                if(culoare==WHITE)  //merge in sus pe tabla
                {
                    opponentBoard=ChessBoard::getAllBoardOf(1-culoare);
                    if(myPoz+8<64 && (((U64) 1<<(myPoz+8)) & board[1-culoare][PAWN])!=0)  //e blocat sus de un pion
                        if((opponentBoard & myPozBoard) == 0) //nu poate lua nicio piesa a adversarului
                        {
                            suma-=40;
                        }
                }
                else    //merge in jos
                {
                    opponentBoard=ChessBoard::getAllBoardOf(1-culoare);
                    if(myPoz-8<64 && (((U64) 1<<(myPoz-8)) & board[1-culoare][PAWN])!=0)  //e blocat sus de un pion
                        if((opponentBoard & myPozBoard) == 0) //nu poate lua nicio piesa a adversarului
                        {
                            suma-=40;
                        }
                }

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }
        //pentru oponent
            myPieceBoard=board[1-culoare][PAWN];
            //cat timp mai sunt pioni
            while(myPieceBoard!=0)
            {
                //aflam pozitia piesei pe tabla
                myPoz=Tools::bitScan(myPieceBoard);
                //aflam tabla cu mutari posibile pt pion
                myPozBoard=ChessBoard::getPieceMoves(myPoz,PAWN,1-culoare);

                if(1-culoare==WHITE)  //merge in sus pe tabla
                {
                    opponentBoard=ChessBoard::getAllBoardOf(culoare);
                    if(myPoz+8<64 && (((U64) 1<<(myPoz+8)) & board[culoare][PAWN])!=0)  //e blocat jos de un pion
                        if((opponentBoard & myPozBoard) == 0) //nu poate lua nicio piesa a adversarului
                        {
                            suma+=40;
                        }
                }
                else    //merge in jos
                {
                    opponentBoard=ChessBoard::getAllBoardOf(culoare);
                    if(myPoz-8<64 && (((U64) 1<<(myPoz-8)) & board[culoare][PAWN])!=0)  //e blocat jos de un pion
                        if((opponentBoard & myPozBoard) == 0) //nu poate lua nicio piesa a adversarului
                        {
                            suma+=40;
                        }
                }

                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }
    /*************END Punctaje pentru P blocati**********/


    /*************Punctaje pentru P izolati**********/
        //pentru mine
            myPieceBoard=board[culoare][PAWN];
            //cat timp mai sunt pioni
            while(myPieceBoard!=0)
            {
                ok=0;
                //scoatem poz primului pion
                myPoz=Tools::bitScan(myPieceBoard);
                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
                
                for(i=KNIGHT;i<KING;i++)
                {
                    //otherPieceBoard=board[culoare][i];  //am scos tabla pentru piesele de tip i
                    otherPieceBoard=ChessBoard::getPseudoMoves(myPoz,i);    //aflam de unde poate ajunge pe myPoz o piesa de tip i
                    if((otherPieceBoard & board[culoare][i])!=0)  //exista o piesa pe tabla care sa ajunga
                    {
                        ok=1;
                        break;
                    }
                }

                //verificam daca pionul e aparat tot de un pion
                if(!ok) //daca nu e deja aparat de o alta piesa
                {
                    if(culoare==WHITE)  //de jos in sus
                    {
                        if(myPoz%8==0)  //e pe prima coloana poate fi aparat doar din dreapta
                        {
                            if(myPoz-7>=0 && (board[culoare][PAWN] & ((U64) 1<<(myPoz-7)))!=0)  //e aparat
                                ok=1;
                        }
                        else
                            if(myPoz%8==7)  //e pe ultima coloana => poate fi aparat doar din stanga
                            {
                                if(myPoz-9>=0 && (board[culoare][PAWN] & ((U64) 1<<(myPoz-9)))!=0)  //e aparat
                                    ok=1;
                            }
                            else    //poate fi aparat din stanga si dreapta
                            {
                                if(myPoz-9>=0 && (board[culoare][PAWN] & ((U64) 1<<(myPoz-9)))!=0)  //e aparat
                                    ok=1;
                                else
                                    if(myPoz-7>=0 && (board[culoare][PAWN] & ((U64) 1<<(myPoz-7)))!=0)  //e aparat
                                        ok=1;
                            }
                    }
                    else    //de sus in jos
                    {
                        if(myPoz%8==0)  //e pe prima coloana poate fi aparat doar din dreapta
                        {
                            if(myPoz+9<64 && (board[culoare][PAWN] & ((U64) 1<<(myPoz+9)))!=0)  //e aparat
                                ok=1;
                        }
                        else
                            if(myPoz%8==7)  //e pe ultima coloana => poate fi aparat doar din stanga
                            {
                                if(myPoz+7<64 && (board[culoare][PAWN] & ((U64) 1<<(myPoz+7)))!=0)  //e aparat
                                    ok=1;
                            }
                            else    //poate fi aparat din stanga si dreapta
                            {
                                if(myPoz+7<64 && (board[culoare][PAWN] & ((U64) 1<<(myPoz+7)))!=0)  //e aparat
                                    ok=1;
                                else
                                    if(myPoz+9<64 && (board[culoare][PAWN] & ((U64) 1<<(myPoz+9)))!=0)  //e aparat
                                        ok=1;
                            }
                    }
                }

                if(!ok)
                    suma-=15;
            }


        //pentru oponent
            myPieceBoard=board[1-culoare][PAWN];
            //cat timp mai sunt pioni
            while(myPieceBoard!=0)
            {
                ok=0;
                //scoatem poz primului pion
                myPoz=Tools::bitScan(myPieceBoard);
                //scotem piesa de pe tabla
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));

                for(i=KNIGHT;i<KING;i++)
                {
                    //otherPieceBoard=board[culoare][i];  //am scos tabla pentru piesele de tip i
                    otherPieceBoard=ChessBoard::getPseudoMoves(myPoz,i);    //aflam de unde poate ajunge pe myPoz o piesa de tip i
                    if((otherPieceBoard & board[1-culoare][i])!=0)  //exista o piesa pe tabla care sa ajunga
                    {
                        ok=1;
                        break;
                    }
                }

                //verificam daca pionul e aparat tot de un pion
                if(!ok) //daca nu e deja aparat de o alta piesa
                {
                    if(1-culoare==WHITE)  //de jos in sus
                    {
                        if(myPoz%8==0)  //e pe prima coloana poate fi aparat doar din dreapta
                        {
                            if(myPoz-7>=0 && (board[1-culoare][PAWN] & ((U64) 1<<(myPoz-7)))!=0)  //e aparat
                                ok=1;
                        }
                        else
                            if(myPoz%8==7)  //e pe ultima coloana => poate fi aparat doar din stanga
                            {
                                if(myPoz-9>=0 && (board[1-culoare][PAWN] & ((U64) 1<<(myPoz-9)))!=0)  //e aparat
                                    ok=1;
                            }
                            else    //poate fi aparat din stanga si dreapta
                            {
                                if(myPoz-9>=0 && (board[1-culoare][PAWN] & ((U64) 1<<(myPoz-9)))!=0)  //e aparat
                                    ok=1;
                                else
                                    if(myPoz-7>=0 && (board[1-culoare][PAWN] & ((U64) 1<<(myPoz-7)))!=0)  //e aparat
                                        ok=1;
                            }
                    }
                    else    //de sus in jos
                    {
                        if(myPoz%8==0)  //e pe prima coloana poate fi aparat doar din dreapta
                        {
                            if(myPoz+9<64 && (board[1-culoare][PAWN] & ((U64) 1<<(myPoz+9)))!=0)  //e aparat
                                ok=1;
                        }
                        else
                            if(myPoz%8==7)  //e pe ultima coloana => poate fi aparat doar din stanga
                            {
                                if(myPoz+7<64 && (board[1-culoare][PAWN] & ((U64) 1<<(myPoz+7)))!=0)  //e aparat
                                    ok=1;
                            }
                            else    //poate fi aparat din stanga si dreapta
                            {
                                if(myPoz+7<64 && (board[1-culoare][PAWN] & ((U64) 1<<(myPoz+7)))!=0)  //e aparat
                                    ok=1;
                                else
                                    if(myPoz+9<64 && (board[1-culoare][PAWN] & ((U64) 1<<(myPoz+9)))!=0)  //e aparat
                                        ok=1;
                            }
                    }
                }

                if(!ok)
                    suma+=15;
            }
    /*************END Punctaje pentru P izolati**********/


    /*************Punctaje pentru P dublati**********/
        //pentru mine
            /*pentru primul pion gasit mergem in sus pe coloana si vedem cati pioni mai sunt
             * pentru toti pe aceeasi coloana se scade din suma 10*nr_pioni_pe_col si apoi se sterg
             * din myPieceBoard*/
            
            myPieceBoard=board[culoare][PAWN];  //am scos tabla de pioni

            while(myPieceBoard!=0)
            {
                myPoz=Tools::bitScan(myPieceBoard); //scoatem pozitia primului pion
                nr=1;   //consideram ca pe coloana curenta nu am gasit decat pionul crt

                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz))); //scoatem pionul
                
                //mergem in sus pe coloana
                while(myPoz+8<64)
                {
                    myPoz+=8;
                    if((((U64) 1<<myPoz) & myPieceBoard)!=0)    //=> e un pion acolo
                    {
                        nr++;
                        myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz))); //scoatem pionul
                    }
                }

                if(nr!=1)   //a fost mai mult de un pion pe linie
                {
                    suma=suma-nr*10;
                }
            }
        //pentru oponent
            myPieceBoard=board[1-culoare][PAWN];  //am scos tabla de pioni

            while(myPieceBoard!=0)
            {
                myPoz=Tools::bitScan(myPieceBoard); //scoatem pozitia primului pion
                nr=1;   //consideram ca pe coloana curenta nu am gasit decat pionul crt

                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz))); //scoatem pionul

                //mergem in sus pe coloana
                while(myPoz+8<64)
                {
                    myPoz+=8;
                    if((((U64) 1<<myPoz) & myPieceBoard)!=0)    //=> e un pion acolo
                    {
                        nr++;
                        myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz))); //scoatem pionul
                    }
                }

                if(nr!=1)   //a fost mai mult de un pion pe linie
                {
                    suma=suma+nr*10;
                }
            }
    /*************END Punctaje pentru P dublati**********/


    /*************Punctaje pentru KingSide PAWNS**********/
        /* daca unul din pioni e departe => -10
         * daca 2 sunt departe           => -20
         * daca 3 sunt departe           => -30
         * daca au fost capturati        => -458
         */
            
         //pentru mine
            ok=0;   //ok va retine cati pioni de tip KingSide lipsesc de pe tabla
            if(culoare==WHITE)
            {
                for(poz=13;poz<=15;poz++)   //trecem prin pozitiile initiale ale fiecarui client
                {
                    myPoz=poz;

                    nr=0;   //consideram ca e exact pe pozitia initiala pionul

                    while(myPoz<64)
                    {
                        if((((U64) 1<<myPoz) & board[culoare][PAWN])!=0)    //exista pion pe pozitia resp
                            break;
                        else
                        {
                            myPoz+=8;
                            nr++;
                        }
                    }

                    if(nr==7)   //nu e pion pe coloana respectiva
                        ok++;
                    else
                        if(nr>2)
                            suma-=10;
                }
                if(ok==3)   //toti pionii lipsesc
                    suma-=45;
            }
            else
            {
                for(poz=61;poz<=63;poz++)   //trecem prin pozitiile initiale ale fiecarui client
                {
                    myPoz=poz;

                    nr=0;   //consideram ca e exact pe pozitia initiala pionul

                    while(myPoz>=0)
                    {
                        if((((U64) 1<<myPoz) & board[culoare][PAWN])!=0)    //exista pion pe pozitia resp
                            break;
                        else
                        {
                            myPoz-=8;
                            nr++;
                        }
                    }

                    if(nr==7)   //nu e pion pe coloana respectiva
                        ok++;
                    else
                        if(nr>2)
                            suma-=10;
                }
                if(ok==3)   //toti pionii lipsesc
                    suma-=45;
            }
         //pentru oponent
            ok=0;   //ok va retine cati pioni de tip KingSide lipsesc de pe tabla
            if(culoare==WHITE)
            {
                for(poz=13;poz<=15;poz++)   //trecem prin pozitiile initiale ale fiecarui client
                {
                    myPoz=poz;

                    nr=0;   //consideram ca e exact pe pozitia initiala pionul

                    while(myPoz<64)
                    {
                        if((((U64) 1<<myPoz) & board[1-culoare][PAWN])!=0)    //exista pion pe pozitia resp
                            break;
                        else
                        {
                            myPoz+=8;
                            nr++;
                        }
                    }

                    if(nr==7)   //nu e pion pe coloana respectiva
                        ok++;
                    else
                        if(nr>2)
                            suma+=10;
                }
                if(ok==3)   //toti pionii lipsesc
                    suma+=45;
            }
            else
            {
                for(poz=61;poz<=63;poz++)   //trecem prin pozitiile initiale ale fiecarui client
                {
                    myPoz=poz;

                    nr=0;   //consideram ca e exact pe pozitia initiala pionul

                    while(myPoz>=0)
                    {
                        if((((U64) 1<<myPoz) & board[1-culoare][PAWN])!=0)    //exista pion pe pozitia resp
                            break;
                        else
                        {
                            myPoz-=8;
                            nr++;
                        }
                    }

                    if(nr==7)   //nu e pion pe coloana respectiva
                        ok++;
                    else
                        if(nr>2)
                            suma+=10;
                }
                if(ok==3)   //toti pionii lipsesc
                    suma+=45;
            }
    /*************END Punctaje pentru KingSide PAWNS**********/

            
    /*************Punctaje pentru TURE pe aceeasi lin/col**********/
        //pentru mine
            myPieceBoard=board[culoare][ROOK];  //scotem tabla cu turele

            poz=Tools::bitScan(myPieceBoard);
            myPieceBoard=(myPieceBoard & (~ ((U64) 1<<poz))); //scoatem tura

            if(myPieceBoard!=0) //daca exista a doua tura pe tabla
            {
                myPoz=Tools::bitScan(myPieceBoard);

                if(myPoz%8==poz%8 || myPoz/8==poz/8)    //sunt pe aceeasi coloana sau aceeasi linie
                    suma+=5;
            }
        //pentru oponent
            myPieceBoard=board[1-culoare][ROOK];  //scotem tabla cu turele

            poz=Tools::bitScan(myPieceBoard);
            myPieceBoard=(myPieceBoard & (~ ((U64) 1<<poz))); //scoatem tura

            if(myPieceBoard!=0) //daca exista a doua tura pe tabla
            {
                myPoz=Tools::bitScan(myPieceBoard);

                if(myPoz%8==poz%8 || myPoz/8==poz/8)    //sunt pe aceeasi coloana sau aceeasi linie
                    suma-=5;
            }
    /*************END Punctaje pentru TURE pe aceeasi lin/col**********/


    /*************Punctaje pentru REGE DESCOPERIT**********/
        //pentru mine
            nr=0;   //presupunem ca sunt 0 patratele in juru regelui neacoperite
            myPoz=Tools::bitScan(board[culoare][KING]); //aflam pozitia regelui pe tabla
            otherPieceBoard=ChessBoard::getAllBoardOf(culoare); //aflam toata tabla noastra

            if(myPoz-8>=0)  //nu e pe prima linie
            {
                if(( ((U64) 1<<(myPoz-8)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                    nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
            }

            if(myPoz+8<64)  //nu e pe ultima linie
            {
                if(( ((U64) 1<<(myPoz+8)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                    nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
            }

            if(myPoz%8!=7)  //nu e pe ultima coloana
            {
                if(myPoz+1<64)  
                {
                    if(( ((U64) 1<<(myPoz+1)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
                if(myPoz+9<64)
                {
                    if(( ((U64) 1<<(myPoz+9)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
                if(myPoz-7>=0)
                {
                    if(( ((U64) 1<<(myPoz-7)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
            }

            if(myPoz%8!=0)  //nu e pe prima coloana
            {
                if(myPoz-1>=0)
                {
                    if(( ((U64) 1<<(myPoz-1)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
                if(myPoz-9>=0)
                {
                    if(( ((U64) 1<<(myPoz-9)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
                if(myPoz+7<64)
                {
                    if(( ((U64) 1<<(myPoz+7)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
            }

            if(nr>=4)   //rege descoperit din 4 directii
                suma-=30;
        //pentru oponent
            nr=0;   //presupunem ca sunt 0 patratele in juru regelui neacoperite
            myPoz=Tools::bitScan(board[1-culoare][KING]); //aflam pozitia regelui pe tabla
            otherPieceBoard=ChessBoard::getAllBoardOf(1-culoare); //aflam toata tabla noastra

            if(myPoz-8>=0)  //nu e pe prima linie
            {
                if(( ((U64) 1<<(myPoz-8)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                    nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
            }

            if(myPoz+8<64)  //nu e pe ultima linie
            {
                if(( ((U64) 1<<(myPoz+8)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                    nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
            }

            if(myPoz%8!=7)  //nu e pe ultima coloana
            {
                if(myPoz+1<64) //nu are sens pt ca stim deja ca nu e pe ult coloana
                {
                    if(( ((U64) 1<<(myPoz+1)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
                if(myPoz+9<64)
                {
                    if(( ((U64) 1<<(myPoz+9)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
                if(myPoz-7>=0)
                {
                    if(( ((U64) 1<<(myPoz-7)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
            }

            if(myPoz%8!=0)  //nu e pe prima coloana
            {
                if(myPoz-1>=0) //nu are sens pt ca stim dj ca nu e pe prima coloana
                {
                    if(( ((U64) 1<<(myPoz-1)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
                if(myPoz-9>=0)
                {
                    if(( ((U64) 1<<(myPoz-9)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
                if(myPoz+7<64)
                {
                    if(( ((U64) 1<<(myPoz+7)) &  otherPieceBoard)==0)   //nu exista pe poz resp nicio piesa a noastra
                        nr++;   //se incrementeaza nr spatiilor libere din jurul regelui
                }
            }

            if(nr>=4)   //rege descoperit din 4 directii
                suma+=30;
    /*************END Punctaje pentru REGE DESCOPERIT**********/


    /*************Punctaje pentru REGINA atacata**********/
        //pentru mine
            myPieceBoard=board[culoare][QUEEN]; //scoatem tabla reginelor
            while(myPieceBoard!=0)
            {
                ok=0;   //pp ca nu e atacata
                //scoatem pozitia reginei
                myPoz=Tools::bitScan(myPieceBoard);
                //verificam daca exista la inamic o piesa care poate ajunge unde avem regina
                for(i=KNIGHT;i<=QUEEN;i++)
                {
                    myPozBoard=ChessBoard::getPseudoMoves(myPoz,i);
                    if((myPozBoard & board[1-culoare][i])!=0)   //exista o piesa de tip i la inamica care poate sa ia regina
                    {
                        ok=1;
                        suma-=40;
                        break;
                    }
                }
                if(!ok) //daca nu e atacata de piese de tip i verificam daca nu e atacata nici de pion
                {
                    myPozBoard=ChessBoard::getPieceMoves(myPoz,QUEEN,culoare);  //aflam unde poate ajunge regina
                    opponentBoard=board[1-culoare][PAWN];   //aflam unde are pionii adversarul

                    otherPieceBoard=(opponentBoard & myPozBoard); //aflam tabla pionilor care se afla in raza reginei
                    while(otherPieceBoard!=0)
                    {
                        poz=Tools::bitScan(otherPieceBoard);    //aflam poz primului pion
                        if(culoare==WHITE)  //=>oponent BLACK => pion de sus in jos
                        {
                            if(myPoz+7==poz || myPoz+9==poz)    //regina e atacata
                            {
                                suma-=40;
                            }
                        }
                        else
                        {
                            if(myPoz-7==poz || myPoz-9==poz)    //regina e atacata
                            {
                                suma-=40;
                            }
                        }
                        //scoatem pionul
                        otherPieceBoard=(otherPieceBoard & (~ ((U64) 1<<poz)));
                    }
                }
                //scoatem regina
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }
        //pentru oponent
            myPieceBoard=board[1-culoare][QUEEN]; //scoatem tabla reginelor
            while(myPieceBoard!=0)
            {
                ok=0;   //pp ca nu e atacata
                //scoatem pozitia reginei
                myPoz=Tools::bitScan(myPieceBoard);
                //verificam daca exista la inamic o piesa care poate ajunge unde avem regina
                for(i=KNIGHT;i<=QUEEN;i++)
                {
                    myPozBoard=ChessBoard::getPseudoMoves(myPoz,i);
                    if((myPozBoard & board[culoare][i])!=0)   //exista o piesa de tip i la inamica care poate sa ia regina
                    {
                        ok=1;
                        suma+=40;
                        break;
                    }
                }
                if(!ok) //daca nu e atacata de piese de tip i verificam daca nu e atacata nici de pion
                {
                    myPozBoard=ChessBoard::getPieceMoves(myPoz,QUEEN,1-culoare);  //aflam unde poate ajunge regina
                    opponentBoard=board[culoare][PAWN];   //aflam unde are pionii adversarul

                    otherPieceBoard=(opponentBoard & myPozBoard); //aflam tabla pionilor care se afla in raza reginei
                    while(otherPieceBoard!=0)
                    {
                        poz=Tools::bitScan(otherPieceBoard);    //aflam poz primului pion
                        if(1-culoare==WHITE)  //=>oponent BLACK => pion de sus in jos
                        {
                            if(myPoz+7==poz || myPoz+9==poz)    //regina e atacata
                            {
                                suma+=40;
                            }
                        }
                        else
                        {
                            if(myPoz-7==poz || myPoz-9==poz)    //regina e atacata
                            {
                                suma+=40;
                            }
                        }
                        //scoatem pionul
                        otherPieceBoard=(otherPieceBoard & (~ ((U64) 1<<poz)));
                    }
                }
                //scoatem regina
                myPieceBoard=(myPieceBoard & (~ ((U64) 1<<myPoz)));
            }
    /*************END Punctaje pentru REGINA atacata**********/


    /*************Punctaje pentru ROCADA**********/
        if(culoare==WHITE)
        {
                if((getFlags() & F_CATLDONEW)!=0)	//Daca a fost facuta rocada alba
                        suma+=EVAL_ROCADA;
                else if( (getFlags()& F_CATLDONEB)!=0)
                        suma-=EVAL_ROCADA;
        }
        else
        {
                if((getFlags() & F_CATLDONEB)!=0)	//Daca a fost facuta rocada alba
                        suma+=EVAL_ROCADA;
                else if((getFlags()& F_CATLDONEW)!=0)
                        suma-=EVAL_ROCADA;
        }
    /*********END Punctaj ROCADA******************/

    
//	fprintf(debug_file,"Evaluarea %d pentru \n",suma);
//	printBoard();

    //Adaugam un mic factor de "noroc", pe evitarea repetitiilor
   // suma+=rand()%3;

    return suma;
}

//functie de evaluare rapida folosita pentru sortarea mutarilor
int ChessBoard::evaluateQuick(COLOR culoare)
{

    int i;
    int suma=0;


    BITBOARD player,oponent; //player - tabla noastra ; oponent - tabla oponentului
  //  COLOR culoare=joc.colorEngine;



   //daca e WHITE => scoatem tabla pieselor albe pentru noi
   if(culoare==WHITE)
    {
        player=getWhiteBoard();
        oponent=getBlackBoard();
    }
    else    //scoatem tabla pieselor negre pentr noi
    {
        player=getBlackBoard();
        oponent=getWhiteBoard();
    }


    //pentru fiecare pozitie de pe tabla verificam ce piesa e acolo si modificam suma tablei in fct de asta


    if(culoare==WHITE)  //daca tabla noastra e alba e mai ok sa incepem de jos in sus (castiga timp la incep)
    {
		U64 poz;
		for(i=0;i<64;i++)
		{

			poz=(U64)1<<i;
			if((poz & player)!=0)  //daca exista o piesa de-a noastra pe poz i
			{  if((poz & board[culoare][PAWN])!=0)    //daca piesa de acolo e pion
					suma+=(PAWNVAL+WHITE_PAWN_VAL[i]);
				else
					if((poz & board[culoare][KNIGHT])!=0)    //daca piesa de acolo e cal
						suma+=(KNIGHTVAL+WHITE_KNIGHT_VAL[i]);
					else
						if((poz & board[culoare][BISHOP])!=0)    //daca piesa de acolo e nebun
							suma+=(BISHOPVAL+WHITE_BISHOP_VAL[i]);
						else
							if((poz & board[culoare][ROOK])!=0)    //daca piesa de acolo e tura
								suma+=(ROOKVAL+WHITE_ROOK_VAL[i]);
							else
								if((poz & board[culoare][QUEEN])!=0)    //daca piesa de acolo e regina
									suma+=(QUEENVAL+WHITE_QUEEN_VAL[i]);
								else
									if((poz & board[culoare][KING])!=0)    //daca piesa de acolo e rege
										suma+=(KINGVAL+WHITE_KING_VAL[i]);

				}
				else if((poz & oponent)!=0) //daca exista o piesa de-a adversarului pe poz i
				{
					if((poz & board[1-culoare][PAWN])!=0)    //daca piesa de acolo e pion
						suma-=(PAWNVAL+BLACK_PAWN_VAL[i]);
					else
						if((poz & board[1-culoare][KNIGHT])!=0)    //daca piesa de acolo e cal
							suma-=(KNIGHTVAL+BLACK_KNIGHT_VAL[i]);
						else
							if((poz & board[1-culoare][BISHOP])!=0)    //daca piesa de acolo e nebun
								suma-=(BISHOPVAL+BLACK_BISHOP_VAL[i]);
							else
								if((poz & board[1-culoare][ROOK])!=0)    //daca piesa de acolo e tura
									suma-=(ROOKVAL+BLACK_ROOK_VAL[i]);
								else
									if((poz & board[1-culoare][QUEEN])!=0)    //daca piesa de acolo e regina
										suma-=(QUEENVAL+BLACK_QUEEN_VAL[i]);
									else
										if((poz & board[1-culoare][KING])!=0)    //daca piesa de acolo e rege
											suma-=(KINGVAL+BLACK_KING_VAL[i]);

				}

			}
	}
    else    //daca tabla noastra e neagra e mai ok sa incepem de sus in jos (castiga timp la incep cand piesele sunt sus
    {
		U64 poz;
        for(i=63;i>=0;i--)
        {
			poz=(U64)1<<i;
            if((poz & player)!=0)  //daca exista o piesa de-a noastra pe poz i
            {
                if((poz & board[culoare][PAWN])!=0)    //daca piesa de acolo e pion
                    suma+=(PAWNVAL+BLACK_PAWN_VAL[i]);
                else
                    if((poz & board[culoare][KNIGHT])!=0)    //daca piesa de acolo e cal
                        suma+=(KNIGHTVAL+BLACK_KNIGHT_VAL[i]);
                    else
                        if((poz & board[culoare][BISHOP])!=0)    //daca piesa de acolo e nebun
                            suma+=(BISHOPVAL+BLACK_BISHOP_VAL[i]);
                        else
                            if((poz & board[culoare][ROOK])!=0)    //daca piesa de acolo e tura
                                suma+=(ROOKVAL+BLACK_ROOK_VAL[i]);
                            else
                                if((poz & board[culoare][QUEEN])!=0)    //daca piesa de acolo e regina
                                    suma+=(QUEENVAL+BLACK_QUEEN_VAL[i]);
                                else
                                    if((poz & board[culoare][KING])!=0)    //daca piesa de acolo e rege
                                        suma+=(KINGVAL+BLACK_KING_VAL[i]);

            }
			else if((poz & oponent)!=0) //daca exista o piesa de-a adversarului pe poz i
			{
				if((poz & board[1-culoare][PAWN])!=0)    //daca piesa de acolo e pion
                    suma-=(PAWNVAL+WHITE_PAWN_VAL[i]);
                else
                    if((poz & board[1-culoare][KNIGHT])!=0)    //daca piesa de acolo e cal
                        suma-=(KNIGHTVAL+WHITE_KNIGHT_VAL[i]);
                    else
                        if((poz & board[1-culoare][BISHOP])!=0)    //daca piesa de acolo e nebun
                            suma-=(BISHOPVAL+WHITE_BISHOP_VAL[i]);
                        else
                            if((poz & board[1-culoare][ROOK])!=0)    //daca piesa de acolo e tura
                                suma-=(ROOKVAL+WHITE_ROOK_VAL[i]);
                            else
                                if((poz & board[1-culoare][QUEEN])!=0)    //daca piesa de acolo e regina
                                    suma-=(QUEENVAL+WHITE_QUEEN_VAL[i]);
                                else
                                    if((poz & board[1-culoare][KING])!=0)    //daca piesa de acolo e rege
                                        suma-=(KINGVAL+WHITE_KING_VAL[i]);
			}
        }
    }

//	fprintf(debug_file,"Evaluarea %d pentru \n",suma);
//	printBoard();

    //Adaugam un mic factor de "noroc", pe evitarea repetitiilor
    suma+=rand()%3;

    return suma;
}


//Seteaza piesele pe pozitia initiala;
void ChessBoard::InitializePieces(){

    board[WHITE][PAWN]=255<<8;
    board[BLACK][PAWN]=(U64)255<<48;

    board[WHITE][ROOK]=129;
    board[BLACK][ROOK]=(U64)129<<56;

    board[WHITE][KNIGHT]=66;
    board[BLACK][KNIGHT]=(U64)66<<56;

    board[WHITE][KING]=16;
    board[BLACK][KING]=(U64)16<<56;

    board[WHITE][QUEEN]=8;
    board[BLACK][QUEEN]=(U64)8<<56;

    board[WHITE][BISHOP]=36;
    board[BLACK][BISHOP]=(U64)36<<56;
 
    board_black=board[BLACK][PAWN] | board[BLACK][ROOK] | board[BLACK][KNIGHT] | board[BLACK][BISHOP] | board[BLACK][KING] | board[BLACK][QUEEN];
    board_white=board[WHITE][PAWN] | board[WHITE][ROOK] | board[WHITE][KNIGHT] | board[WHITE][BISHOP] | board[WHITE][KING] | board[WHITE][QUEEN];

    flags=F_INITIAL;
}


//DEBUG: Folosita pentru afisarea la stderr a tablei curente; litere mari piese albe, litere mici piese negre
void ChessBoard::printBoard(void){


    unsigned char tabla[8][8]={{0}};
    int i,j,poz;

    BITBOARD temp_board[2][6];
    for(i=0;i<=1;i++)
        for(j=0;j<6;j++)
            temp_board[i][j]=board[i][j];

    //punem piesele;
    for(i=0;i<=1;i++)
        for(j=0;j<6;j++)
            for(poz=0;poz<64;poz++)
            {
                if(temp_board[i][j]%2==1){    //daca e impar => bitul e 1;
                    if(i==WHITE)    //piese albe
                        switch(j){
                            case PAWN:    tabla[7-poz/8][poz%8]='P'; break;
                            case KNIGHT:  tabla[7-poz/8][poz%8]='N'; break;
                            case KING:    tabla[7-poz/8][poz%8]='K'; break;
                            case BISHOP:  tabla[7-poz/8][poz%8]='B'; break;
                            case ROOK:    tabla[7-poz/8][poz%8]='R'; break;
                            case QUEEN:   tabla[7-poz/8][poz%8]='Q'; break;
                            
                        }
                    else
                         switch(j){
                            case PAWN:    tabla[7-poz/8][poz%8]='p'; break;
                            case KNIGHT:  tabla[7-poz/8][poz%8]='n'; break;
                            case KING:    tabla[7-poz/8][poz%8]='k'; break;
                            case BISHOP:  tabla[7-poz/8][poz%8]='b'; break;
                            case ROOK:    tabla[7-poz/8][poz%8]='r'; break;
                            case QUEEN:   tabla[7-poz/8][poz%8]='q'; break;
                        }
                    }
                    temp_board[i][j]>>=1;
                  
                }
        //afisare matrice
        for(i=0;i<8;i++)
        {

          for(j=0;j<8;j++)
              if(tabla[i][j]!=0)
                  fprintf(debug_file," %c ",tabla[i][j]);
              else
                  fprintf(debug_file,"   ");
          fprintf(debug_file,"\n");
        }
        //Afisare flags
        fprintf(debug_file,"\nFLAGS: RRW RLW RRB RLB CAW CAB CADOW CADOB\n");
        fprintf(debug_file,"  %3d   ",flags);
        for(i=0;i<5;i++)
            if(1<<i & flags)
                fprintf(debug_file,"1  ");
            else
                fprintf(debug_file,"0  ");
         fprintf(debug_file,"\n\n");

         
  


}

/* Verifica daca este sah si seteaza Flags in concordanta
 * Returneaza 1 daca este sah si 0 in caz contrar;
 */
int ChessBoard::isCheck(){
    //TODO
    return 0;
}

//Destructor
ChessBoard::~ChessBoard() {

    
}

/*Obtine o copie a acestui ChessBoard;*/
ChessBoard ChessBoard::clone(){
    ChessBoard new_board;

    //copiem vectorul de BITBOARD pentru piese
    new_board.setBoards((BITBOARD*)board);


    //copiem steagurile
    new_board.setFlags(flags);

    return new_board;
}

//obtine vectorul de BITBOARD al tablei curente
BITBOARD* ChessBoard::getBoards(){
    return (BITBOARD*)board;
}

//seteaza vectorul de BITBOARD al tablei curente
void ChessBoard::setBoards(BITBOARD *new_boards){
    //copiez elementele
    memcpy(new_boards,board,2*6*sizeof(BITBOARD));
}


/*Obtine un bitboard care contine 1 pe pozitiile in care poate muta
 * VALID piesa 'piesa', de culoarea "culoare", pornind din 'pozitie'
 * Nu se ia in considerarea verificarea de sah;
 */
BITBOARD ChessBoard::getPieceMoves(int pozitie, PIECE piesa, COLOR culoare){
    //Daca piesa este una care are mutari generate de getPseudoMoves
    if(piesa==KNIGHT || piesa==ROOK || piesa==QUEEN || piesa==BISHOP || piesa==KING)
	return getPseudoMoves(pozitie,piesa) & ~(getAllBoardOf(culoare));

    //Daca piesa este pion
    BITBOARD mutare=0;
    int linie, coloana;
    linie=pozitie/8;
    coloana=pozitie%8;

    if(piesa==PAWN){
        if(culoare==WHITE)  //pionul se misca in sus
        {
            if(linie<7 && (getAllBoard() & ((U64)1<<(pozitie+8)))==0) //poate avansa drept in fata
                mutare|=((U64)1<<(pozitie+8));
            if(coloana<7) //poata ataca in dreapta
                if((getBlackBoard() & ((U64)1<<(pozitie+9)))!=0) //si exista ceva ce poate fi atacat
                    mutare|=((U64)1<<(pozitie+9));
            if(coloana>0) //poata ataca in stanga
                if((getBlackBoard() & ((U64)1<<(pozitie+7)))!=0) //si exista ceva ce poate fi atacat
                    mutare|=((U64)1<<(pozitie+7));
	    if(linie==1 && (getAllBoard() & ((U64)1<<(pozitie+8)))==0 && (getAllBoard() & ((U64)1<<(pozitie+16)))==0) //daca e pe linia initiala si nu e nimic in fata
		    mutare|=((U64)1<<(pozitie+16));
        }
        else                //pionul se misca in jos
        {
            if(linie>0 && (getAllBoard() & ((U64)1<<(pozitie-8)))==0  ) //poate avansa drept in fata
                mutare|=((U64)1<<(pozitie-8));
            if(coloana<7) //poata ataca in dreapta (stanga lui)
                if((getWhiteBoard() & ((U64)1<<(pozitie-7)))!=0) //si exista ceva ce poate fi atacat
                    mutare|=((U64)1<<(pozitie-7));
            if(coloana>0) //poata ataca in stanga
                if((getWhiteBoard() & ((U64)1<<(pozitie-9)))!=0) //si exista ceva ce poate fi atacat
                    mutare|=((U64)1<<(pozitie-9));
	    if(linie==6 && (getAllBoard() & ((U64)1<<(pozitie-8)))==0 && (getAllBoard() & ((U64)1<<(pozitie-16)))==0) //daca e pe linia initiala si nu e nimic in fata
		    mutare|=((U64)1<<(pozitie-16));
        }
        return (mutare & ~(getAllBoardOf(culoare)));
    }

    return 0;
}


/* Functii pentru generare mutari posibile pentru tipuri de piese: Knight, King, Queen, Bishop, Rook
 * In cazul pieselor glisante, sunt incluse inclusiv piesele de la limita capete, atat ale
 * engine-ului cat si ale inamicului.*/
BITBOARD ChessBoard::getPseudoMoves(int pozitie,PIECE piesa){


    if(piesa==ROOK){
         BITBOARD northBitboard;
         BITBOARD southBitboard;
         BITBOARD eastBitboard;
         BITBOARD vestBitboard;

         vestBitboard = (getAllBoard() & ROOK_VEST[pozitie]);
         vestBitboard = (vestBitboard>>1)|(vestBitboard>>2)|(vestBitboard>>3)|(vestBitboard>>4)|(vestBitboard>>5)|(vestBitboard>>6);
         vestBitboard= (vestBitboard & ROOK_VEST[pozitie])^ROOK_VEST[pozitie];

         eastBitboard = (getAllBoard() & ROOK_EAST[pozitie]);
         eastBitboard = (eastBitboard<<1)|(eastBitboard<<2)|(eastBitboard<<3)|(eastBitboard<<4)|(eastBitboard<<5)|(eastBitboard<<6);
         eastBitboard = (eastBitboard & ROOK_EAST[pozitie]) ^ ROOK_EAST[pozitie];


         northBitboard = (getAllBoard() & ROOK_NORTH[pozitie]);
         northBitboard = (northBitboard<<8)|(northBitboard<<16)|(northBitboard<<24)|(northBitboard<<32)|(northBitboard<<40)|(northBitboard<<48);
         northBitboard = (northBitboard & ROOK_NORTH[pozitie]) ^ ROOK_NORTH[pozitie];

         southBitboard = (getAllBoard() & ROOK_SOUTH[pozitie]);
         southBitboard = (southBitboard>>8)|(southBitboard>>16)|(southBitboard>>24)|(southBitboard>>32)|(southBitboard>>40)|(southBitboard>>48);
         southBitboard = (southBitboard & ROOK_SOUTH[pozitie]) ^ ROOK_SOUTH[pozitie];

         return (vestBitboard | eastBitboard | northBitboard | southBitboard);
    }

    if(piesa==BISHOP){
        BITBOARD board45;
        BITBOARD board135;
        BITBOARD board225;
        BITBOARD board315;

         board45 = (getAllBoard() & BISHOP_NE[pozitie]);
         board45 = (board45<<9)|(board45<<18)|(board45<<27)|(board45<<36)|(board45<<45)|(board45<<54);
         board45 = (board45 & BISHOP_NE[pozitie]) ^ BISHOP_NE[pozitie];

         board135 = (getAllBoard() & BISHOP_NV[pozitie]);
         board135 = (board135<<7)|(board135<<14)|(board135<<21)|(board135<<28)|(board135<<35)|(board135<<42);
         board135 = (board135 & BISHOP_NV[pozitie]) ^ BISHOP_NV[pozitie];

         board225 = (getAllBoard() & BISHOP_SV[pozitie]);
         board225 = (board225>>9)|(board225>>18)|(board225>>27)|(board225>>36)|(board225>>45)|(board225>>54);
         board225 = (board225 & BISHOP_SV[pozitie]) ^ BISHOP_SV[pozitie];

         board315 = (getAllBoard() & BISHOP_SE[pozitie]);
         board315 = (board315>>7)|(board315>>14)|(board315>>21)|(board315>>28)|(board315>>35)|(board315>>42);
         board315 = (board315 & BISHOP_SE[pozitie]) ^ BISHOP_SE[pozitie];

         return (board45 | board135 | board225 | board315);

    }
    if(piesa==QUEEN){
          BITBOARD northBitboard;
          BITBOARD southBitboard;
          BITBOARD eastBitboard;
          BITBOARD vestBitboard;
          BITBOARD board45;
          BITBOARD board135;
          BITBOARD board225;
          BITBOARD board315;

         vestBitboard = (getAllBoard() & ROOK_VEST[pozitie]);
         vestBitboard = (vestBitboard>>1)|(vestBitboard>>2)|(vestBitboard>>3)|(vestBitboard>>4)|(vestBitboard>>5)|(vestBitboard>>6);
         vestBitboard= (vestBitboard & ROOK_VEST[pozitie])^ROOK_VEST[pozitie];

         eastBitboard = (getAllBoard() & ROOK_EAST[pozitie]);
         eastBitboard = (eastBitboard<<1)|(eastBitboard<<2)|(eastBitboard<<3)|(eastBitboard<<4)|(eastBitboard<<5)|(eastBitboard<<6);
         eastBitboard = (eastBitboard & ROOK_EAST[pozitie]) ^ ROOK_EAST[pozitie];


         northBitboard = (getAllBoard() & ROOK_NORTH[pozitie]);
         northBitboard = (northBitboard<<8)|(northBitboard<<16)|(northBitboard<<24)|(northBitboard<<32)|(northBitboard<<40)|(northBitboard<<48);
         northBitboard = (northBitboard & ROOK_NORTH[pozitie]) ^ ROOK_NORTH[pozitie];

         southBitboard = (getAllBoard() & ROOK_SOUTH[pozitie]);
         southBitboard = (southBitboard>>8)|(southBitboard>>16)|(southBitboard>>24)|(southBitboard>>32)|(southBitboard>>40)|(southBitboard>>48);
         southBitboard = (southBitboard & ROOK_SOUTH[pozitie]) ^ ROOK_SOUTH[pozitie];


         board45 = (getAllBoard() & BISHOP_NE[pozitie]);
         board45 = (board45<<9)|(board45<<18)|(board45<<27)|(board45<<36)|(board45<<45)|(board45<<54);
         board45 = (board45 & BISHOP_NE[pozitie]) ^ BISHOP_NE[pozitie];

         board135 = (getAllBoard() & BISHOP_NV[pozitie]);
         board135 = (board135<<7)|(board135<<14)|(board135<<21)|(board135<<28)|(board135<<35)|(board135<<42);
         board135 = (board135 & BISHOP_NV[pozitie]) ^ BISHOP_NV[pozitie];

         board225 = (getAllBoard() & BISHOP_SV[pozitie]);
         board225 = (board225>>9)|(board225>>18)|(board225>>27)|(board225>>36)|(board225>>45)|(board225>>54);
         board225 = (board225 & BISHOP_SV[pozitie]) ^ BISHOP_SV[pozitie];

         board315 = (getAllBoard() & BISHOP_SE[pozitie]);
         board315 = (board315>>7)|(board315>>14)|(board315>>21)|(board315>>28)|(board315>>35)|(board315>>42);
         board315 = (board315 & BISHOP_SE[pozitie]) ^ BISHOP_SE[pozitie];

    return (vestBitboard | eastBitboard | northBitboard | southBitboard | board45 | board135 | board225 | board315);
    }
    
    if(piesa==KNIGHT)
	return KNIGHT_MOVES[pozitie];
    
    if(piesa==KING)
        return KING_MOVES[pozitie];


// daca nu returneaza toata tabla de sah

    // daca nu returneaza 0, adica nici o mutare posibila

    return 0;
    
}

