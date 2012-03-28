/* 
 * File:   Game.cpp
 * Author: Flip4it chess
 * 
 * Created on March 11, 2010, 7:34 PM
 */

#include "Game.h"
#include <stdio.h>
#include "stuff.h"
#include<stdlib.h>
#include "Xboard.h"
#include <string.h>
#include "Tools.h"
#include "Mind.h"
#include "Book.h"


extern Xboard xboard;
extern Mind mind;

//DEBUG
FILE* debug_file;

Game::Game() {
    currentBoard.InitializePieces();
	movesDone=0;
	lateGame=false;
}


Game::~Game() {
}

void Game::initialize(){

}
//obtine tabla ChessBoard curenta;
ChessBoard Game::getCurrentBoard(){
    return currentBoard;
}

//seteaza tabla ChessBoard curenta;
void Game::setCurrentBoard(ChessBoard newCurrentBoard){
    currentBoard=newCurrentBoard;
}

//seteaza culoarea curenta a engine-ului
void Game::setGameColor(COLOR culoare){
    colorEngine=culoare;
}

//seteaza timerii curenti ai jocului
void Game::setGameTimers(int new_time,int new_otime){
    time=new_time;
    otime=new_otime;
}

//Functia de resetare a jocului. Se reinitializeaza tabla;
void Game::gameReset(){
    currentBoard.InitializePieces();
}

void Game::play(){

    char *buf=0;
    Book newBook;
    
    
    //Obtinem prima mutare, daca exista. Daca e randul meu la mutare, buf=null
    buf=xboard.getFirstMove();
  
    //daca a mutat oponentul, actualizez tabla
    if(buf!=NULL)
    {
	currentBoard.updateBoard(buf,1-colorEngine);
    }

    //DEBUG: Deschidem fisierele de debug, cate unul pentru fiecare player
    if(colorEngine==BLACK)
		debug_file=fopen("FlipDebugFile_BLACK.txt","w");
    else
		debug_file=fopen("FlipDebugFile_WHITE.txt","w");

    setbuf(debug_file,NULL);


    if(colorEngine==BLACK)
        fprintf(debug_file,"Flip4itEngine is playing BLACK\n");
    else
        fprintf(debug_file,"Flip4itEngine is playing WHITE\n");

    //DESCHIDERI
    while(1)
    {
	//Obtinem mutarea urmatoare
	buf=newBook.getNextMove(currentBoard,colorEngine);
	//Daca buf=0, inseamna ca nu mai avem deschideri si iesim;
	if(buf==0)
	    break;

	movesDone++;
	//actualizez tabla interna
	currentBoard.updateBoard(buf,colorEngine);
	
	//trimit mutarea la XBoard
	xboard.sendMoveToXboard(buf);
	free(buf);

	fprintf(debug_file,"Obtinem mutarea adversarului...\n");
	//Obtin mutarea adversarului
	buf=xboard.getMoveFromXboard();

	//Actualizez tabla curenta, dupa mutarea adversarului
	currentBoard.updateBoard(buf,1-colorEngine);
	fprintf(debug_file,"Mutare adversar:%s\n",buf);
	currentBoard.printBoard();

	free(buf);
    }

    fprintf(debug_file,"Exiting OPENINGS Area!\n\n");
    
    while(1)
    {
		if(colorEngine==WHITE)
			   fprintf(debug_file,"Engine WHITE:\n");
		else
			   fprintf(debug_file,"Engine BLACK:\n");

//                if(currentBoard.isCheckmate(colorEngine,Tools::bitScan(currentBoard.board[colorEngine][KING])))
//                {
//                        fprintf(debug_file,"Am primit sah mat!\n");
//                        xboard.resignEngineToXboard();
//                        xboard.getMoveFromXboard();
//                }
                //Obtine piesa ce trebuie mutata si pozitiile din/in care se face mutarea

//        // doar daca e sah la rege se  apeleaza mutari de evitare, daca nu rezultat NegaMax
//        if(currentBoard.isCheck(colorEngine,Tools::bitScan(currentBoard.board[colorEngine][KING]))>0)
//         {
//			fprintf(debug_file,"Este sah la rege\n");
//			piesa_de_mutat=mind.getKingsEscape(move_poz);
//
//			if(piesa_de_mutat==NO_PIECE)
//				xboard.resignEngineToXboard();
//			buf=currentBoard.getSANofMove(colorEngine,move_poz[0],move_poz[1],piesa_de_mutat);
//        }
//        else

        //Obtinem urmatoarea mutare din functia mind.
        buf=mind.getSanNegaMax();

        //actualizez tabla interna
		currentBoard.updateBoard(buf,colorEngine);
		fprintf(debug_file,"Mutarea este %s \n",buf);

		//Numar de mutari
		movesDone++;
		if(movesDone==40)
		{
			lateGame=true;
			movesDone=0;
		}

		xboard.sendMoveToXboard(buf);
		free(buf);

		fprintf(debug_file,"Obtinem mutarea adversarului...\n");
		//Obtin mutarea adversarului
		buf=xboard.getMoveFromXboard();

		//Actualizez tabla curenta, dupa mutarea adversarului
		currentBoard.updateBoard(buf,1-colorEngine);
		fprintf(debug_file,"Tabla curenta dupa mutarea adversarului:%s\n",buf);
		currentBoard.printBoard();

		free(buf);
    }

}