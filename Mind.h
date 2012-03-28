/* 
 * File:   Mind.h
 * Author: Flip4it chess
 *
 * Created on March 18, 2010, 12:04 AM
 */

#ifndef _MIND_H
#define	_MIND_H

#include "time.h"
#include "stuff.h"
#include "Game.h"
#include "ChessBoard.h"
#include "MoveTree.h"



class Mind {
public:
    Mind();
    virtual ~Mind();

    /*Obtine mutarea optima ce trabuie executata. Intoarce pozitia initiala a piesei (intre 0 si 63) si pozitia finala a piesei
     *in movePositions[0], respectiv movePositions[1]. Returneaza tipul piesei mutate; */
    PIECE getMove(int *movePositions);
    PIECE getKingsEscape(int *movePositions);
    char* getSanNegaMax();
private:
    PIECE getPiece();
    PIECE getPieceMove(PIECE piese,int *movePositions);
    PIECE whatPieceAttacksKing(int culoare,int pozKing, BITBOARD *pieceBoard);
    PIECE getDesperateMove(int *mobePositions);
    MNode *IterativeDeepening(MoveTree* tree,time_t maxDuration);

};

#endif	/* _MIND_H */

