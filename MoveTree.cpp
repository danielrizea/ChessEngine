/*
 * File:   MoveTree.cpp
 * Author: Flip4it chess
 *
 * Created on April 14, 2010, 4:04 AM
 */

#include "MoveTree.h"
#include <stdio.h>
#include "stuff.h"
#include "Tools.h"
#include <stdlib.h>
#include <list>
#include "time.h"
#include "Xboard.h"
extern FILE* debug_file;
extern Game joc;
extern Xboard xboard;
using namespace std;







MoveTree::MoveTree(){
    nrNoduri=0;

}

//aloca memorie pentru un nod si face setarile de rigoare
MNode* MoveTree::creareNod(){

    MNode *node;
	node=(MNode*)malloc(sizeof(MNode));

	node->parinte=NULL;
	node->copii=(MNode**)malloc(MAX_COPII*sizeof(MNode*));
	node->nrCopii=0;
	node->nrMaxAlocat=MAX_COPII;
	node->niv=0;

	node->boardScore=UNDEFINED;  // scorul intotdeauna pozitiv pe tabla

    return node;
}

//adauga copilul copil la parintele parinte
MNode* MoveTree::adaugaCopil(MNode* tata, MNode* copil,COLOR culoare)
{
    /*if(tata==NULL)
        fprintf(debug_file,"Tata NULLLLL\n");
    else
        fprintf(debug_file,"Se adauga la parintele cu %d nr max_copii %d copii fiul ce are %d copii \n",tata->nrCopii,tata->nrMaxAlocat,copil->nrCopii);
	*/

	//trebuie realocat spatiu suplimentar pentru referinte catre copii
    if(tata->nrCopii==tata->nrMaxAlocat)
    {
		//   fprintf(debug_file, "se realoca spatiu\n");
		//   fprintf(debug_file, "nivel tata %d \n",tata->niv);
        tata->copii=(MNode**)realloc(tata->copii,(tata->nrMaxAlocat+NR_REALLOC)*sizeof(MNode*));

        tata->nrMaxAlocat=tata->nrMaxAlocat+NR_REALLOC;
    }

	//Evaluam copilul
	copil->quickScore=copil->board.evaluateQuick(culoare);

	//Cautam si introducem respectivul copil in vectorul de copii, pe pozitia buna;
	short i;
	for(i=0;i<tata->nrCopii;i++)
		if(tata->copii[i]->quickScore > copil->quickScore)
			break;

	//Mutam elementele la dreapta si introducem copilul
	short poz=i;
	for(i=tata->nrCopii-1;i>=poz;i--)
		tata->copii[i+1]=tata->copii[i];

	//Punem elementul in pozitia finala
	tata->copii[poz]=copil;
//	tata->copii[tata->nrCopii]=copil;

	//Actualizam restul datelor
    tata->nrCopii=tata->nrCopii+1;

	copil->niv=tata->niv+1;
    copil->parinte=tata;

	//returneaza copilul
    return copil;
}

/* adauga/creeaza la arborele caracterizat de root un nou nivel, returneaza radacina;
 * nivelul contine doar mutarile de atac si va fi folosit in Quiesence Search.*/
MNode* MoveTree::creareNivelAtac(MNode* nodRoot,COLOR culoare)
{
    int i;
    int pozPiesa,finalPozPiesa;
    int movePositions[2];
    PIECE piesa;
    BITBOARD tablaPiese,mutari;
    ChessBoard backup;

    MNode *fiu_curent;

    int pozKing=Tools::bitScan(nodRoot->board.board[culoare][KING]);


    for(i=PAWN;i<=KING;i++)
    {
		// se iau toate piesele
        piesa=i;

        //fprintf(debug_file,"Pentru piesa:%d %d\n",piesa,culoare);

        tablaPiese=nodRoot->board.board[culoare][piesa];
        //fprintf(debug_file,"tabla piese\n");
		//nodRoot->board.printBoard();

		//Cat timp tabla mai are piese, pentru fiecare piese de un anumit tip se iau toate mutarile
		while(tablaPiese)
        {
			//We find the position of the piece
            pozPiesa=Tools::bitScan(tablaPiese);
			//Stergem 1-ul de pe pozitia gasita
			tablaPiese=tablaPiese & (~((U64)1<<pozPiesa));

			//fprintf(debug_file,"piesa:%d %d de culoare %d\n",i,pozPiesa,culoare);

			//if(nodRoot->board.canDoMove(pozPiesa,pozKing,culoare)==1)
			{
				if(culoare==WHITE)
					mutari=nodRoot->board.getPieceMoves(pozPiesa,piesa,culoare) & nodRoot->board.getBlackBoard();
				else
					mutari=nodRoot->board.getPieceMoves(pozPiesa,piesa,culoare) & nodRoot->board.getWhiteBoard();

				// se ia fiecare mutare
				while(mutari)
				{
					//We find the final position of the piece
					finalPozPiesa=Tools::bitScan(mutari);
					//Stergem 1-ul de pe pozitia gasita
					mutari=mutari & (~((U64)1<<finalPozPiesa));

					movePositions[0]=pozPiesa;
					movePositions[1]=finalPozPiesa;

                    //salvam tabla curenta
                    backup=nodRoot->board;

                    //efectuam "virtual" mutarea pe tabla backup pentru a verifica sahul
                    backup.doMove(movePositions[0],movePositions[1],piesa,culoare);
                    //fprintf(debug_file,"Incercam piesa %d: %d -> %d\n",piesa,movePositions[0],movePositions[1]);
                    //nodRoot->board.printBoard();

                    if(piesa==KING)
                        pozKing=finalPozPiesa;


                    //Daca e sah, anulam mutarea si trecem mai departe
                    if(backup.isCheck(culoare,pozKing))
                    {
                            //fprintf(debug_file,"INCORECTA mutarea %d : %d -> %d!\n\n",piesa,movePositions[0],movePositions[1]);
                            continue;
                    }

                    //salvam fiul

                    //Cream un nou nod;
					nrNoduri++;

					fiu_curent=creareNod();
					fiu_curent->board=nodRoot->board;
					fiu_curent->niv=nodRoot->niv+1;
					adaugaCopil(nodRoot,fiu_curent,culoare);

					fiu_curent->movePositions[0]=movePositions[0];
					fiu_curent->movePositions[1]=movePositions[1];
					fiu_curent->movedPiece=i;

					fiu_curent->board.doMove(movePositions[0],movePositions[1],i,culoare);
				}
            }
		}
	}
	return  nodRoot;
}

// adauga/creeaza la arborele caracterizat de root un nou nivel, returneaza radacina;
MNode* MoveTree::creareNivelTree(MNode* nodRoot,COLOR culoare)
{
	//fprintf(debug_file,"New level\n");
    int i;
    int pozPiesa,finalPozPiesa;
    int movePositions[2];
    PIECE piesa;
    BITBOARD tablaPiese,mutari;
    ChessBoard backup;

    MNode *fiu_curent;

    int pozKing=Tools::bitScan(nodRoot->board.board[culoare][KING]);


    for(i=PAWN;i<=KING;i++)
    {
		// se iau toate piesele
        piesa=i;

        //fprintf(debug_file,"Pentru piesa:%d %d\n",piesa,culoare);

        tablaPiese=nodRoot->board.board[culoare][piesa];
        //fprintf(debug_file,"tabla piese\n");
		//nodRoot->board.printBoard();

		//Cat timp tabla mai are piese, pentru fiecare piese de un anumit tip se iau toate mutarile
		while(tablaPiese)
        {
			//We find the position of the piece
            pozPiesa=Tools::bitScan(tablaPiese);
			//Stergem 1-ul de pe pozitia gasita
			tablaPiese=tablaPiese & (~((U64)1<<pozPiesa));

			//fprintf(debug_file,"piesa:%d %d de culoare %d\n",i,pozPiesa,culoare);

			//if(nodRoot->board.canDoMove(pozPiesa,pozKing,culoare)==1)
			
				mutari=nodRoot->board.getPieceMoves(pozPiesa,piesa,culoare);

				// se ia fiecare mutare
				while(mutari)
				{
					//We find the final position of the piece
					finalPozPiesa=Tools::bitScan(mutari);
					//Stergem 1-ul de pe pozitia gasita
					mutari=mutari & (~((U64)1<<finalPozPiesa));

					movePositions[0]=pozPiesa;
					movePositions[1]=finalPozPiesa;

                    //salvam tabla curenta
                    backup=nodRoot->board;

                    //efectuam "virtual" mutarea pe tabla backup pentru a verifica sahul
                    backup.doMove(movePositions[0],movePositions[1],piesa,culoare);
                    //fprintf(debug_file,"Incercam piesa %d: %d -> %d\n",piesa,movePositions[0],movePositions[1]);
                    //nodRoot->board.printBoard();

                    if(piesa==KING)
                        pozKing=finalPozPiesa;


                    //Daca e sah, anulam mutarea si trecem mai departe
                    if(backup.isCheck(culoare,pozKing))
                    {
                            //fprintf(debug_file,"INCORECTA mutarea %d : %d -> %d!\n\n",piesa,movePositions[0],movePositions[1]);
                            continue;
                    }

                    //salvam fiul

                    //Cream un nou nod;
					nrNoduri++;

					fiu_curent=creareNod();
					fiu_curent->board=nodRoot->board;
					fiu_curent->niv=nodRoot->niv+1;
					adaugaCopil(nodRoot,fiu_curent,culoare);

					fiu_curent->movePositions[0]=movePositions[0];
					fiu_curent->movePositions[1]=movePositions[1];
					fiu_curent->movedPiece=piesa;

					fiu_curent->board.doMove(movePositions[0],movePositions[1],piesa,culoare);
				}
            
		}
	}

	//Adaugam alte tipuri de mutari:
	/*ROCADA*/
	/*Daca movedPiece==KING si movePositions[1]=-1 => s-a facut rocada mica.
	*Daca movedPiece==KING si movePositions[1]=-2 => s-a facut rocada mare.*/
	ChessBoard nouaTabla=nodRoot->board;
	if(culoare==WHITE)
	{
		if((nodRoot->board.getFlags() & F_CASTLINGW)!=0 && (nodRoot->board.getFlags() & F_CATLDONEW)==0)	//daca nu a fost facuta si mai poate fi facuta
		{
			nouaTabla=nodRoot->board;
			//si daca nu sunt piese acolo si nu s-a mutat tura stanga
			if((nodRoot->board.getFlags() & F_CASTLR_L_W)==0 && (nodRoot->board.board[WHITE][ROOK] & ((U64)1)) && (nodRoot->board.getAllBoard() & ((U64)1<<1))==0 && (nodRoot->board.getAllBoard() & ((U64)1<<2))==0 && (nodRoot->board.getAllBoard() & ((U64)1<<3))==0)
				//Si nu e sah pe nici una dintre pozitii
				if(nodRoot->board.isCheck(WHITE,2)==0 && nodRoot->board.isCheck(WHITE,3)==0 && nodRoot->board.isCheck(WHITE,4)==0)
				{
					nouaTabla.board[WHITE][KING] =((U64)4);		//setam regele pe poz finala
					nouaTabla.board[WHITE][ROOK] |=((U64)8);	//setam tura pe pozitia finala
					nouaTabla.board[WHITE][ROOK] &=~((U64)1);	//stergem tura de pe pozitia initiala
					nouaTabla.setFlags(((nouaTabla.getFlags() | F_CATLDONEW) & ~F_CASTLINGW) | F_CASTLR_R_W);

//					fprintf(debug_file,"Posibilitate ROCADA1:\n");
//					nouaTabla.printBoard();
//					nodRoot->board.printBoard();

					//Adaugare nod
					fiu_curent=creareNod();
					fiu_curent->board=nouaTabla;
					fiu_curent->niv=nodRoot->niv+1;
					adaugaCopil(nodRoot,fiu_curent,culoare);

					fiu_curent->movePositions[0]=ROCADA_MARE;
					fiu_curent->movedPiece=KING;
				}
			
			nouaTabla=nodRoot->board;
			//si daca nu sunt piese acolo si nu s-a mutat tura dreapta
			if((nodRoot->board.getFlags() & F_CASTLR_R_W)==0 && (nodRoot->board.board[WHITE][ROOK] & ((U64)1<<7)) && (nodRoot->board.getAllBoard() & ((U64)1<<5))==0 && (nodRoot->board.getAllBoard() & ((U64)1<<6))==0)
				//Si nu e sah pe nici una dintre pozitii
				if(nodRoot->board.isCheck(WHITE,4)==0 && nodRoot->board.isCheck(WHITE,5)==0 && nodRoot->board.isCheck(WHITE,6)==0)
				{
					nouaTabla.board[WHITE][KING] =((U64)1<<6);
					nouaTabla.board[WHITE][ROOK] |=((U64)1<<5);
					nouaTabla.board[WHITE][ROOK] &=~((U64)1<<7);
					nouaTabla.setFlags(((nouaTabla.getFlags() | F_CATLDONEW) & ~F_CASTLINGW) | F_CASTLR_R_W);

//					fprintf(debug_file,"Posibilitate ROCADA2:\n");
//					nouaTabla.printBoard();
//					nodRoot->board.printBoard();
					

					//Adaugare nod
					fiu_curent=creareNod();
					fiu_curent->board=nouaTabla;
					fiu_curent->niv=nodRoot->niv+1;
					adaugaCopil(nodRoot,fiu_curent,culoare);

					fiu_curent->movePositions[0]=ROCADA_MICA;
					fiu_curent->movedPiece=KING;
				}
		}
	}
	else	//Culoare neagra
	{

		if((nodRoot->board.getFlags() & F_CASTLINGB)!=0 && (nodRoot->board.getFlags() & F_CATLDONEB)==0)	//daca nu a fost facuta si mai poate fi facuta
		{
			nouaTabla=nodRoot->board;
			//si daca nu sunt piese acolo si nu s-a mutat tura stanga
			if((nodRoot->board.getFlags() & F_CASTLR_L_B)==0 && (nodRoot->board.board[BLACK][ROOK] & ((U64)1<<56)) && (nodRoot->board.getAllBoard() & ((U64)1<<57))==0 && (nodRoot->board.getAllBoard() & ((U64)1<<58))==0 && (nodRoot->board.getAllBoard() & ((U64)1<<59))==0)
				//Si nu e sah pe nici una dintre pozitii
				if(nodRoot->board.isCheck(BLACK,58)==0 && nodRoot->board.isCheck(BLACK,59)==0 && nodRoot->board.isCheck(BLACK,60)==0)
				{
					nouaTabla.board[BLACK][KING] =((U64)1<<58);
					nouaTabla.board[BLACK][ROOK] |=((U64)1<<59);
					nouaTabla.board[BLACK][ROOK] &=~((U64)1<<56);
					nouaTabla.setFlags(((nouaTabla.getFlags() | F_CATLDONEB) & ~F_CASTLINGB) | F_CASTLR_L_B);

//					fprintf(debug_file,"Posibilitate ROCADA3:\n");
//					nouaTabla.printBoard();
//					nodRoot->board.printBoard();


					//Adaugare nod
					fiu_curent=creareNod();
					fiu_curent->board=nouaTabla;
					fiu_curent->niv=nodRoot->niv+1;
					adaugaCopil(nodRoot,fiu_curent,culoare);

					fiu_curent->movePositions[0]=ROCADA_MARE;
					fiu_curent->movedPiece=KING;
				}
			nouaTabla=nodRoot->board;
			//si daca nu sunt piese acolo si nu s-a mutat tura dreapta
			if((nodRoot->board.getFlags() & F_CASTLR_R_B)==0 && (nodRoot->board.board[BLACK][ROOK] & ((U64)1<<63)) && (nodRoot->board.getAllBoard() & ((U64)1<<61))==0 && (nodRoot->board.getAllBoard() & ((U64)1<<62))==0)
				//Si nu e sah pe nici una dintre pozitii
				if(nodRoot->board.isCheck(BLACK,60)==0 && nodRoot->board.isCheck(BLACK,61)==0 && nodRoot->board.isCheck(BLACK,62)==0)
				{
					nouaTabla.board[BLACK][KING] =((U64)1<<62);
					nouaTabla.board[BLACK][ROOK] |=((U64)1<<61);
					nouaTabla.board[BLACK][ROOK] &=~((U64)1<<63);
					nouaTabla.setFlags(((nouaTabla.getFlags() | F_CATLDONEB) & ~F_CASTLINGB) | F_CASTLR_R_B);

					//Adaugare nod
					fiu_curent=creareNod();
					fiu_curent->board=nouaTabla;
					fiu_curent->niv=nodRoot->niv+1;
					adaugaCopil(nodRoot,fiu_curent,culoare);

//					fprintf(debug_file,"Posibilitate ROCADA din in4:\n");
//					nouaTabla.printBoard();
//					nodRoot->board.printBoard();


					fiu_curent->movePositions[0]=ROCADA_MICA;
					fiu_curent->movedPiece=KING;
				}
		}
	}
	return  nodRoot;
}



// trebuie apelata cu rootul arborelui

/*
char* MoveTree::getTreeBestMove(MNode *nodRoot){


    int i;
    int poz=-1;
    int aux;
    int max=MIN;

    fprintf(debug_file,"Evalaore tabla curenta %d\n",nodRoot->board.evaluateBoard(joc.colorEngine));
    for(i=0;i<nodRoot->nrCopii;i++){


        aux=NegaMax(nodRoot->copii[i],MIN,MAX,joc.colorEngine);


        MNode *auxNode=nodRoot->copii[i];



        fprintf(debug_file,"Evaluare tabla %d\n",auxNode->board.evaluateBoard(joc.colorEngine));



	fprintf(debug_file," varianta %d cu valoarea %d  %d(%d -> %d) adica %s:\n",poz,aux,auxNode->parinte->movedPiece,auxNode->movePositions[0],auxNode->movePositions[1],auxNode->board.getSANofMove(joc.colorEngine,auxNode->movePositions[0],auxNode->movePositions[1],auxNode->movedPiece));
        if(aux>max)
        {
         max=aux;
         poz=i;

        }
    }

	MNode *chosenNode=nodRoot->copii[poz];
	fprintf(debug_file,"S-a ales nodul %d varianta %d(%d -> %d) adica %s:\n",i,chosenNode->parinte->movedPiece,chosenNode->movePositions[0],chosenNode->movePositions[1],chosenNode->board.getSANofMove(joc.colorEngine,chosenNode->movePositions[0],chosenNode->movePositions[1],chosenNode->movedPiece));
	chosenNode->board.printBoard();

      return chosenNode->parinte->board.getSANofMove(joc.colorEngine,chosenNode->movePositions[0],chosenNode->movePositions[1],chosenNode->movedPiece);


}



*/
 // Constructor pentru pozitia initiala a tablei

/*
void MoveTree::MakeTree(ChessBoard startBoard,int level)
{


    MNode *parinte;
	COLOR culoare=joc.colorEngine;	//culoarea inversa pentru ca sa se inceapa cu generarea mutarilor noastre

    int nivel_tree=0;

    int onTime=1;
    int i;
    int j;
    int k;
    int l;

    time_t startSeconds;
    time_t curentSeconds;

    root=creareNod();
    root->board=startBoard;
    root->parinte=NULL;

	//fprintf(debug_file,"Placa de inceput:\n");
	root->board.printBoard();

    // se creaza primul nivel;
    creareNivelTree(root,joc.colorEngine);
    nivel_tree++;

    startSeconds = time (NULL);


    while(onTime==1 && nivel_tree<level)
    {

        parinte=root;

        for(i=0;i<parinte->nrCopii;i++)
        {
            creareNivelTree(parinte->copii[i],1-culoare);
			culoare=1-culoare;
        }

        //arborele acum are doua nivele (parinte=root);
        nivel_tree++;

        if(nivel_tree==level)
            break;

        // arbore 3 nivele
        for(i=0;i<parinte->nrCopii;i++)
            for(j=0;j<parinte->copii[i]->nrCopii;j++)
                creareNivelTree(parinte->copii[i]->copii[j],culoare);


		nivel_tree++;
		if(nivel_tree==level)
			break;

		// nivelul 4
		 for(i=0;i<parinte->nrCopii;i++)
			for(j=0;j<parinte->copii[i]->nrCopii;j++)
				for(k=0;k<parinte->copii[i]->copii[j]->nrCopii;k++)
				{
					curentSeconds=time(NULL);

					//daca generarea ia mai mult de MAX_CALC_TIME secunde
					if(curentSeconds-startSeconds>MAX_CALC_TIME)
						return;
					else
						creareNivelTree(parinte->copii[i]->copii[j]->copii[k],1-culoare);
				}

		   nivel_tree++;
		   if(nivel_tree==level)
			   break;
                   //nivel 5
                    for(i=0;i<parinte->nrCopii;i++)
			for(j=0;j<parinte->copii[i]->nrCopii;j++)
				for(k=0;k<parinte->copii[i]->copii[j]->nrCopii;k++)
                                    for(l=0;l<parinte->copii[i]->copii[j]->copii[k]->nrCopii;l++)
				{
					curentSeconds=time(NULL);

					//daca generarea ia mai mult de MAX_CALC_TIME secunde
					if(curentSeconds-startSeconds>MAX_CALC_TIME)
						return;
					else
						creareNivelTree(parinte->copii[i]->copii[j]->copii[k]->copii[l],culoare);
				}


		   nivel_tree++;
		   if(nivel_tree==level)
			   break;

    }

}


*/




/*Actualizeaza arborele in functie de tabla curenta 'currentBoard'
 *Se sterg ramurile inutile si se adauga noi nivele in arbore, pana se completeaza 'level' nivele;*/
void MoveTree::updateNewTree(ChessBoard currentBoard, int level)
{
    //TODO
}

/*Obtine radacina arborelui MoveTree, de tip MNode.*/
MNode* MoveTree::getTree()
{
    return root;
}

void MoveTree::eliberareNod(MNode* node)
{
	// se elibereaza nodul doar daca este frunza (nu cumva sa se distruga arborele)
    if(node->nrCopii==0)
    {
        node->board.~ChessBoard();
        free(node->copii);
        free(node);
    }
}

void MoveTree::parcurge(MNode* rad,int level){


    int i;
  //  fprintf(debug_file,"nodul se afla pe nivelul %d si are %d cu %d copii\n",level,rad->nrCopii,rad->nrMaxAlocat);

    nrCopii++;
    for(i=0;i<rad->nrCopii;i++)
    {


   // fprintf(debug_file,"\t Fiul are la randul sau %d %d \n ",rad->copii[i]->nrCopii,rad->copii[i]->nrMaxAlocat);
    parcurge(rad->copii[i],level+1);

    }




}


void MoveTree::Dispose(MNode *node,int level){



    int i=0;

// node trebuie sa fie parinte
    nrCopii++;

    //fprintf(debug_file,"Numar stergeri :%d\n",nrCopii);

  //    fprintf(debug_file,"eliberare memorie nod nivel :%d %d nr copii  %d\n",level,node->niv,node->nrCopii);

      if(node->nrCopii==0)
      {
          //fprintf(debug_file,"eliberare nod frunza %d\n",node->nrCopii);

			eliberareNod(node);
            return;
      }

      else
      {



//fprintf(debug_file,"eliberare nod intermediar %d\n",node->nrCopii);

      for(i=0;i<node->nrCopii;i++){


       //  fprintf(debug_file,"apel pentru  eliberare nod urmator %d\n",node->copii[i]->niv);

           Dispose(node->copii[i],level+1);

       }


      node->nrCopii=0;
        eliberareNod(node);


      }

}

/* cod wiki
function negamax(node, depth, α, β, color)
    if node is a terminal node or depth = 0
        return color * the heuristic value of node
    else
        foreach child of node
            α := max(α, -negamax(child, depth-1, -β, -α, -color))
            {the following if statement constitutes alpha-beta pruning}
            if α≥β
                return α
        return α
*/


int MoveTree::AlfaBeta(MNode* node, int alfa, int beta, COLOR color, int depth){
    int i;
    int score;
	int poz;

    int bestScore=MIN;


    if(depth==0)
        return node->board.evaluateBoard(color);

    if(node->nrCopii==0)
		creareNivelTree(node,1-color);


    for(i=0;i<node->nrCopii;i++)
    {
        score=-AlfaBeta(node->copii[i],-beta,-alfa,1-color,depth-1);


        if(score>=beta && node->parinte!=NULL )
            return score; // fail soft

        if(score>bestScore)
        {
            bestScore=score;
            if(score>alfa)
			{
				alfa=score;

			}
							poz=i;

        }
    }

	if(node->parinte==NULL)
		return poz;
    return bestScore;


}


int MoveTree::quesenceSearch(MNode* node,int alfa,int beta,COLOR color, int depth){

 int i;
    int score;

    int bestScore=MIN;

  //  fprintf(debug_file,"S-a apelat quesenceSearch este nivelul %d in quesencesearch \n",depth);


    // daca a ajuns la nivelul setat
    if(depth==0)
        return node->board.evaluateBoard(color);



    if(node->nrCopii==0)
        creareNivelAtac(node,color);



        //daca nu mai are copii
    if(node->nrCopii==0)
        return node->board.evaluateBoard(color);




    for(i=0;i<node->nrCopii;i++)
    {
        score=-quesenceSearch(node->copii[i],-beta,-alfa,1-color,depth-1);

        if(score>=beta)
            return score; // fail soft

        if(score>bestScore)
        {
            bestScore=score;
            if(score>alfa)
                alfa=score;

        }
    }
    return bestScore;





}


int MoveTree::NegaScout(MNode* node,int alfa,int beta,COLOR color,int depth, time_t maxDuration){
    int i;
    int poz=0;
    int cur;

	//facem verificarea de timp la nivelul final
    if(depth==0)
    {
		currentTime=clock();

		if(maxDuration-(clock()-startTime)/(CLOCKS_PER_SEC/1000)<=0)
		{
		//    fprintf(debug_file,"break de pe nivelul %d %d\n",depthLimit,curentSeconds-startSeconds);

			this->levelIncomplete=true;
			onTime=0;


            return node->board.evaluateBoard(color);
       	}

    }

    if(depth==0)
    {
       
            return quesenceSearch(node,-beta,-alfa,color,QUESENCE_LEVEL);

        // daca nu este quit node

        //return node->board.evaluateBoard(color);


    }
	// se creeaza copii pentru nodul curent, copii culoare opusa, alt nivel, celalat oponent
	// daca nodul nu are copii deja creati, nod terminal, atunci se creeaza;

    if(node->nrCopii==0)
		creareNivelTree(node,color);

    // daca suntem pe nivelul 0, punctul de plecare (tabla curenta) si nu se poate face nici o mutare
    if(node->parinte==NULL)
        if(node->nrCopii==0){

        int pozKing;
        // se obtine pozitia regelui a enginu-lui

        pozKing=Tools::bitScan(node->board.board[joc.colorEngine][KING]);



        if(node->board.isCheck(joc.colorEngine,pozKing)==1)
            return CHECKMATE;


            return DRAW;
       }

//daca nu se trece mai departe, ei sunt deja creati

    int b=beta;
//fprintf(debug_file,"La nivelul %d avem variantele :\n",ARB_DEPTH-depth);
    for(i=0;i<node->nrCopii;i++)
        {

	    //NegaScout


        cur=-NegaScout(node->copii[i],-b,-alfa,1-color,depth-1,maxDuration);


       if(cur>alfa)
       {


           alfa=cur;
           maxScore=cur;

           poz=i;

       }
 // fprintf(debug_file,"%d ",-alfa);

		if(alfa>=beta && node->parinte!=NULL)
		{

		nrPrunninguri++;
                return alfa;             // beta cut-off
		}
                if(onTime==0)
                    return i;
    }
            if(node->parinte==NULL)
                    return poz;

	levelIncomplete=false;
    return alfa;
}

int MoveTree::negaMax2( int depth, MNode* rad,COLOR culoare )
{
        if ( depth == 0 )
			return rad->board.evaluateBoard(culoare);
        int max = 1000000;
 
		creareNivelTree(rad,1-culoare);
	//	fprintf(debug_file,"Copii lui -  in numar de : %d\n",rad->nrCopii);
		rad->board.printBoard();
		int i=0,poz;
        for (i=0;i<rad->nrCopii;i++)
        {
                int score = -negaMax2( depth - 1, rad->copii[i],1-culoare );
                if( score < max )
				{
					max = score;
					poz=i;
//					fprintf(debug_file,"Score: %d\n",max);
//					rad->copii[i]->board.printBoard();
				}
        }

		if(depth==3)
			copilToMove=poz;
//		fprintf(debug_file,"\n\n\n\n\n\n\n\n\n\n_________________\nPe level %d S-a ajuns la maximul: %d\n",depth, max);
//		rad->copii[poz]->board.printBoard();
        return max;
}



char* MoveTree::IterativeDeepening(time_t maxDuration)
{


    MNode* nodeRoot;
	COLOR culoare=joc.colorEngine;	//culoarea inversa pentru ca sa se inceapa cu generarea mutarilor noastre


// presepunem ca nivelele sunt generate complet


	//fprintf(debug_file,"Starting Iterative deepening with duration: %d\n",(int)(maxDuration-(clock()-startTime)/(CLOCKS_PER_SEC/1000)));
    int depthLimit;
    int max;
    int poz;



    // cand incepe procesul
    //startSeconds=time(NULL);
    // cand incepe procesul
    startTime=clock();

    // se creeaza nodul radacina
    nodeRoot=creareNod();
    nodeRoot->board=joc.currentBoard;
    nodeRoot->parinte=NULL;

    root=nodeRoot;

    // depthLimitul initial
    depthLimit=ARB_DEPTH;
	


    levelIncomplete=0;
    onTime=1;


    int pozFinal=0;
  //depthLimit=2;
    // iterative deepening

    while(1)
	{
		fprintf(debug_file,"Starting Iterative deepening with duration %d for level %d\n",(int)(maxDuration-(clock()-startTime)/(CLOCKS_PER_SEC/1000)),depthLimit);

		poz=0;

		nrPrunninguri=0;


        poz=NegaScout(nodeRoot,MIN,MAX,culoare,depthLimit-1,maxDuration);

		fprintf(debug_file,"S-a ales pozitia %d \n",poz);

		//Am primit sah mat
		if(poz==CHECKMATE)
		{
			xboard.resignEngineToXboard();
			fprintf(debug_file,"Am primit sah mat.\n");
			xboard.getMoveFromXboard();
			return NULL;
		}
		//Este pat (nu putem muta)
		if(poz==DRAW)
		{
			xboard.resignEngineToXboard();
			fprintf(debug_file,"Am primit draw. Nu putem muta.\n");
			xboard.getMoveFromXboard();
			return NULL;
		}

        fprintf(debug_file,"S-a ales pozitia %d \n",poz);

        if(levelIncomplete==0)
        {
            pozFinal=poz;

        }

		 if(levelIncomplete==false)
		 {

			 fprintf(debug_file,"-->S-a generat complet nivelul %d si mai sunt %d milisecunde.\n",depthLimit,(int)(maxDuration-(clock()-startTime)/(CLOCKS_PER_SEC/1000)));

		 }
		else
			 fprintf(debug_file,"Nu s-a generat complet nivelul %d si timpul \"ramas\" este %d milisecunde.\n",depthLimit,(int)(maxDuration-(clock()-startTime)/(CLOCKS_PER_SEC/1000)));



		//daca nu mai este timp, iesim;
		if(maxDuration-(clock()-startTime)/(CLOCKS_PER_SEC/1000)<=0)
		{
			fprintf(debug_file,"S-a depasit timpul alocat. Time left: %d\n",(int)(maxDuration-(clock()-startTime)/(CLOCKS_PER_SEC/1000)));
			break;
		}

		depthLimit++;

		if(depthLimit==MAX_DEPTH)
			break;

    }





    poz=pozFinal;
    max=maxScore;

MNode *chosenNode=nodeRoot->copii[poz];
fprintf(debug_file,"S-a ales poz %d cu aux %d din nr total copii root %d si valoarea %d\n",poz,max,nodeRoot->nrCopii,max);
	fprintf(debug_file,"S-a ales nodul %d varianta %d(%d -> %d) adica %s:\n",poz,chosenNode->parinte->movedPiece,chosenNode->movePositions[0],chosenNode->movePositions[1],chosenNode->board.getSANofMove(joc.colorEngine,chosenNode->movePositions[0],chosenNode->movePositions[1],chosenNode->movedPiece));
	chosenNode->board.printBoard();

      return chosenNode->parinte->board.getSANofMove(joc.colorEngine,chosenNode->movePositions[0],chosenNode->movePositions[1],chosenNode->movedPiece);



}

