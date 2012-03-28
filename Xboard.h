/*
 * File:   xboard.h
 * Author: Flip4it chess
 *
 * Created on March 12, 2010, 9:31 PM
 */

#ifndef _XBOARD_H
#define	_XBOARD_H

#include "Game.h"
#include "stuff.h"
class Xboard {
public:
    Xboard();
    //toate comenzile citite trec prin aceata comanda


    // de fiecare data cand se executa un strind comanda ex: "move f3"
    // se va apela giveCommandToXboard("move f3");
    void   sendMoveToXboard(char*);
    char*  getMoveFromXboard();
    char*  getFirstMove();
    void resignEngineToXboard();
    int   initialize();


    // de dat pt joc
    int time;
    int otime;
    COLOR colorEngine;
    bool forceMode;
    COLOR onTheMove;

    //terminat variabile



private:


        int giveCommandToXboard(char *command);
    // toate functiile de mai jos sunt apelate din functia
    // giveCommandToXoard(command) in functie de stringul command
    // sunt private, nu ar trebui apelate din exterior
    void initializeFeatures();
    void illegalMoveEngineToXboard(char *s);
  
    void moveCommandXboardToEngine(char *command);
    void newCommandXboardToEngine(char *command);
    void forceCommandXboardToEngine(char *command);
    void goCommandXboardToEngine(char *command);
    void whiteCommandXBoardToEngine(char *command);
    void blackCommandXboardToEngine(char *command);
    void quitCommandXboardToEngine(char *command);


};

#endif	/* _XBOARD_H */
