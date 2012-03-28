/* 
 * File:   ChessBoard.h
 * Author: Flip4it chess
 *
 * Created on March 10, 2010, 9:11 PM
 */

#ifndef _CHESSBOARD_H
#define	_CHESSBOARD_H

#include "stuff.h"
#include "Evaluation.h"


class ChessBoard {
public:
    
    //constructor null
    ChessBoard();
    //destructor;
    virtual ~ChessBoard();

    //DEBUG - folosita pentru afisarea tablei
    void printBoard(void );

    //obtine bitboard-ul pieselor albe
    inline BITBOARD getWhiteBoard() {   return board[WHITE][PAWN] | board[WHITE][KNIGHT] | board[WHITE][BISHOP] | board[WHITE][ROOK] |
                                        board[WHITE][QUEEN] | board[WHITE][KING]; };
    //obtine bitboard-ul pieselor negre
    inline BITBOARD getBlackBoard() {   return board[BLACK][PAWN] | board[BLACK][KNIGHT] | board[BLACK][BISHOP] | board[BLACK][ROOK] |
                                        board[BLACK][QUEEN] | board[BLACK][KING]; };
    //obtine bitboard-ul tuturor pieselor;
    inline BITBOARD getAllBoard()   {   return getWhiteBoard() | getBlackBoard(); };
    //obtine bitboard-ul pieselor de culoarea ceruta
    inline BITBOARD getAllBoardOf(COLOR culoare) { if(culoare==WHITE) return getWhiteBoard();
                                                else return getBlackBoard();}


    /*Functia ce returneaza codul SAN asociat unei mutari (primeste pozitia de la 0-63
    *a piesei pe tabla si pozitia in care va fi mutata tot de la 0-63, tipul piesei si
    *daca mutand piesa astfel va fi luata o piesa de culoare opusa.    */
    MOVE getSANofMove(COLOR culoare_curenta, int startPoz, int endPoz, PIECE myPiece);

    //functia intoarce 1 daca s-a dat sah la regele de culoare specificata, respectiv 0 daca nu
    int isCheck(int culoare, int pozKing);

    //functia intoarce 1 daca s-a dat sahmat la regele de culoare specificata, respectiv 0 daca nu
    int isCheckmate(int culoare, int pozKing);

    //functia verifica daca piesa de pe pozitia poz e aparata de o alta piesa
    int DefByOther(int culoare, int poz);

    //functia verifica daca regele e in imposibilitate de a se muta (dar nu e atacat unde este) si e singura piesa de pe tabla =>remiza
    int isDraw(int culoare,int pozKing);

    //functia realizeaza o mutare, primind pozitia initiala, finala, piesa mutata si culoarea
    void doMove(int pozInitiala, int pozFinala, int PIECE, COLOR culoare);

    //functia returneaza 1 daca o piesa poate fi mutata de pe poz fara ca piesa de pe pozpr sa fie atacata
    int canDoMove(int poz, int pozpr, int culoare);

    //functia returneaza valoarea tablei curente
    int evaluateBoard(COLOR);

    //functie de evaluare rapida folosita pentru sortarea mutarilor
    int evaluateQuick(COLOR);

    /*Actualizeaza ChessBoard-ul curent in conformitate cu MOVE;
     */
    void updateBoard(MOVE,COLOR);

    /*Obtine o copie a acestui ChessBoard;*/
    ChessBoard clone();


    //obtine vectorul de BITBOARD al tablei curente
    BITBOARD* getBoards();
    //seteaza vectorul de BITBOARD al tablei curente
    void setBoards(BITBOARD*);

    //obtine Flag-urile pentru tabla
    FLAGS inline getFlags(){return flags;};
    //seteaza flagurile pentru tabla
    void inline setFlags(FLAGS new_flags){flags=new_flags;};

    /* Verifica daca este sah si seteaza Flags in concordanta
     * Returneaza 1 daca este sah si 0 in caz contrar;
     */
    int isCheck();

    /*Obtine un bitboard care contine 1 pe pozitiile in care poate muta
     * VALID piesa 'piesa', de culoarea "culoare", pornind din 'pozitie'.
     * Nu se ia in considerarea verificarea de sah;
     */
    BITBOARD getPieceMoves(int pozitie, PIECE piesa, COLOR culoare);

    BITBOARD board[2][6];

    //Seteaza tabla in stare initiala, cu piesele pe pozitia initiala!
    void InitializePieces();
    
    /* Functii pentru generare mutari posibile pentru tipuri de piese: Knight, King, Queen, Bishop, Rook
     * In cazul pieselor glisante, sunt incluse inclusiv piesele de la limita capete, atat ale
     * engine-ului cat si ale inamicului.*/
    BITBOARD getPseudoMoves(int pozitie,PIECE piesa);

    static void printBitboard(BITBOARD b);


private:
    /*Tabla de sah este un U64; Avem o matrice de table, astfel:
     *      -prima linie pentru albe, a doua linie pentru negre
     *      -fiecare coloana pentru una dintre piese: PAWN, KNIGHT, BISHOP, QUEEN, ROOK, KING constante definite in stuff.h
     */


    BITBOARD board_white;
    BITBOARD board_black;
    ChessBoard* parent;

    FLAGS flags;




};

#endif	/* _BITBOARD_H */

