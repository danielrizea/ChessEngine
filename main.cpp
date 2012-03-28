/* *****
 * File:   main.cpp
 * Author: Cosmin
 *
 * Created on March 10, 2010, 8:46 PM
 */

#include <stdio.h>
#include "ChessBoard.h"
#include "stuff.h"
#include "Tools.h"
#include "Xboard.h"
#include "Game.h"
#include "Mind.h"
#include "MoveTree.h"

#include <signal.h>
#include <ctime>
#include <cstdlib>  //pentru rand()


Xboard xboard;
Game joc;
Mind mind;

extern FILE* debug_file;




int main(int argc, char** argv)
{
     //signal handling  SIGINT
     //daca nu e analizat crashuieste programul
     //ignora SIGINT altfel crasuieste dupa prima mutare a engine-ului
    signal(SIGINT,SIG_IGN);
    srand((unsigned)time(0));

//	debug_file=fopen("test.txt","w");
//	ChessBoard testBoard;
//	testBoard.InitializePieces();
//
//	MoveTree tree;
//	tree.MakeTree(testBoard,3);
//
//	return 0;


	//Initializam clasa de comunicare cu XBoard
    xboard.initialize();



	//Incepe jocul
    joc.play();


}
