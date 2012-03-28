/* 
 * File:   Game.h
 * Author: Flip4it chess
 *
 * Created on March 11, 2010, 7:34 PM
 */

#include "stuff.h"
#include "ChessBoard.h"

#ifndef _GAME_H 
#define	_GAME_H

class Game {
public:
    //constructor
    Game();
    //destructor
    virtual ~Game();

    //initializarea jocului
    void initialize();

    //obtine tabla ChessBoard curenta;
    ChessBoard getCurrentBoard();

    //seteaza tabla ChessBoard curenta;
    void setCurrentBoard(ChessBoard);

    //seteaza culoarea curenta a engine-ului
    void setGameColor(COLOR);

    //seteaza timerii curenti ai jocului
    void setGameTimers(int time,int otime);

    //Functia de joc efectiv
    void play();

    //Functia de resetare a jocului. Se reinitializeaza tabla;
    void gameReset();

    
    ChessBoard currentBoard;
    COLOR colorEngine;
    COLOR onTheMove;
    int time, otime;
    bool forceMode;
    int movesDone;
    bool lateGame;

};

#endif	/* _GAME_H */

