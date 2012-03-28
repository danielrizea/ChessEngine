/* 
 * File:   MoveTree.h
 * Author: Flip4it chess
 *
 * Created on April 14, 2010, 4:04 AM
 */

#ifndef _MOVETREE_H
#define	_MOVETREE_H

#include "ChessBoard.h"
#include "list"

#include <time.h>

#define MAX_COPII 1     //pentru ca pe ultimul nivel nu e nevoie sa aloc 15 copii;
#define NR_REALLOC 15


using namespace std;
struct mnod;
typedef struct mnod MNode;

struct mnod{
   //PUBLIC
   ChessBoard board; //tabla din nodul respectiv
   int movePositions[2];    //pozitiile de pe care a mutat piesa din parinte in fiu
                            /*Daca movedPiece==KING si movePositions[1]=-1 => s-a facut rocada mica.
                             *Daca movedPiece==KING si movePositions[1]=-2 => s-a facut rocada mica.*/
   PIECE movedPiece;    //piesa mutata fata de parinte

   //PRIVATE

   //referinta catre nodul parinte
   MNode *parinte;

   int boardScore;
   U16 quickScore;
   //vector referinte catre copii
   MNode** copii;
     // int nivelN;
   // numarul efectiv de copii
   U16 nrCopii;
   U16 niv;
   // numarul maxim pentru care este alocat spatiul de memorie
   U16 nrMaxAlocat;
 
   
};


class MoveTree {
public:
    /*Constructor pentru pozitia initiala a tablei*/
    MoveTree();
    int nrPrunninguri;
    bool levelIncomplete;
    time_t timeLimit;
    void MakeTree(ChessBoard startBoard,int level);

    /*Actualizeaza arborele in functie de tabla curenta 'currentBoard'
     *Se sterg ramurile inutile si se adauga noi nivele in arbore, pana se completeaza 'level' nivele;*/
    void updateNewTree(ChessBoard currentBoard, int level);

    /*Obtine radacina arborelui MoveTree, de tip MNode.*/
    MNode* getTree();

    //elibereaza memoria elimina arborele pana la nivelul level
    void Dispose(MNode*,int level);
    int NegaScout(MNode* node,int alfa,int beta,COLOR color,int depthLimit, time_t maxDuration);
    // returneaza mutarea
    char* NegaMaxIterativeDeepening();

    char* IterativeDeepening(time_t maxDuration);
    char* getTreeBestMove(MNode *nodRad);

    // o parcurgere de test
    void parcurge(MNode* rad,int level);

    //aloca memorie pentru un nod si face setarile de rigoare
    MNode* creareNod();

            int AlfaBeta(MNode* node,int alfa,int beta,COLOR color,int depth);
               int onTime;
     MNode *root;     //radacina
private:

    int cLevel;     //nivel curent
    int fLevel;     //nivel final
    int nrNoduri;
    list<MNode*> Noduri;
    MNode *lastNode;
    int maxScore;
    time_t startTime;
    time_t currentTime;
    int copilToMove;

   int nrCopii;


   
   //time_t startSeconds;
   time_t curentSeconds;




    // adauga/creeaza la arborele caracterizat de root un nou nivel, returneaza radacina;
    MNode* creareNivelTree(MNode *nodRoot, COLOR culoare);

    int NegaMax(MNode* node,int alfa,int beta,COLOR color,int depth,int depthLimit);

     /* adauga/creeaza la arborele caracterizat de root un nou nivel, returneaza radacina;
     * nivelul contine doar mutarile de atac si va fi folosit in Quiesence Search.*/
    MNode* creareNivelAtac(MNode* nodRoot,COLOR culoare);

    int negaMax2( int depth, MNode* copil,COLOR culoare );

    // Queseance search pentru a duce evalaorea la un nivel in care se gaseste un echilibru (nu se mai pot lua piese)
    int quesenceSearch(MNode* node,int alfa,int beta,COLOR color,int depth);

    //adauga copilul copil la parintele parinte
    MNode* adaugaCopil(MNode *parinte, MNode *copil, COLOR culoare);

    // eliberare memorie nod (+ distruge obiect Chessboard)
    void eliberareNod(MNode *node);
};

#endif	/* _MOVETREE_H */

