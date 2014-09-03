/*

Author: Ben Eggers <ben.eggers36@gmail.com>
License: MIT

Implementation of the board specification found in "Board.h".

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Board.h"
#include "Board_priv.h"
#include "_Zob.h"

#include "BitBoard.h"
#include "InitBoard.h"
#include "MoveStack.h"

#include "Pieces.h"
#include "PieceTransform.h"


Board AllocateBoard() {
    Piece ps[64];

    ReadPieces(&INIT_BOARD_CHARS[0], &ps[0]);

    Board b = (Board) malloc(sizeof(BoardRec));
    if (b == NULL) {
        // shit
        return (Board) NULL;
    }

    b->moveStack = AllocateStack();
    if (b->moveStack == NULL) {
        free(b);
        return NULL;
    }

    b->whiteToMove = true;
    b->gameOver = false;
    b->enPassant = -1;
    b->promo = WhiteQueen;
    b->quietMoves = 0;
    b->numMoves = 0;

    ZobristInit(b);
    b->pastHashes[0] = ZobristEntireBoard(b);

    memcpy(&b->bbs, &INIT_BOARD_BBREP, sizeof(b->bbs));
    memcpy(&b->pieces, &ps, sizeof(b->pieces));

    return b;
}


void FreeBoard(Board board) {
    FreeStack(board->moveStack);
    free(board);
}


void SetPromochar(Board board, Piece piece) {
    board->promo = piece;
}


bool WhiteToMove(Board b) {
    return b->whiteToMove;
}


int NumMoves(Board b) {
    return b->numMoves;
}


int NumQuietMoves(Board b) {
    return b->quietMoves;
}


// TODO: Make sure they're not being attacked
bool WhiteCastleKingsSide(Board b) {
    //         king           h-rook      virgin bits
    return (b->pieces[4] & b->pieces[7] & VIRGIN_MASK);
}


bool WhiteCastleQueensSide(Board b) {
    return (b->pieces[4] & b->pieces[0] & VIRGIN_MASK);
}


bool BlackCastleKingsSide(Board b) {
    return (b->pieces[60] & b->pieces[63] & VIRGIN_MASK);
}


bool BlackCastleQueensSide(Board b) {
    return (b->pieces[60] & b->pieces[54] & VIRGIN_MASK);
}


int EnPassantPawn(Board b) {
    return b->enPassant;
}


bool IsMoveLegal(Board board, int from, int to) {
    return 0 <= from && from <= 63 && 0 <= to && to <= 63;
}


uint64_t ZobristHash(Board b) {
    return b->pastHashes[b->numMoves];
}

const uint64_t *HistoricalHashes(Board b) {
    return b->pastHashes;
}


bool IsPlayerToMoveInCheck(Board b) {
    // TODO
    return false;
}


bool IsGameOver(Board b) {
    return b->gameOver;
}


int WhoWon(Board b) {
    // TODO
    return 1;
}


bool MakeMove(Board b, int from, int to) {
    MoveStackNode sn;   

    if (!IsMoveLegal(b, from, to)) {
        return false;
    }

    sn.fromSquare = from;
    sn.toSquare = to;
    sn.takenPiece = b->pieces[to];
    sn.prevQuietCounter = b->quietMoves;
    sn.prevEnPassant = b->enPassant;
    PushStack(b->moveStack, sn);

    b->pieces[to] = b->pieces[from] & ~VIRGIN_MASK;
    b->pieces[from] = EmptySquare;
    // TODO: BB changes and stuff
    // TODO: EP changes
    // TODO: Promotion
    // TODO: RoP hash
    // TODO: quiet move?
    b->whiteToMove = !(b->whiteToMove);

    return true;
}


void UnmakeMove(Board b) {
    MoveStackNode *sn;

    sn = PopStack(b->moveStack);

    b->enPassant = sn->prevEnPassant;
    b->quietMoves = sn->prevQuietCounter;
    b->pieces[sn->fromSquare] = b->pieces[sn->toSquare];
    b->pieces[sn->toSquare] = sn->takenPiece;

    b->whiteToMove = !(b->whiteToMove);
}


const Piece *GetArrayRep(Board board) {
    return board->pieces;
}


const bb_t *GetBBRep(Board board) {
    return board->bbs;
}


Board BoardCopy(Board b) {
    Board nb;

    nb = (Board) malloc(sizeof(BoardRec));
    if (nb == NULL) {
        return nb;
    }

    nb->whiteToMove = b->whiteToMove;
    nb->gameOver = b->gameOver;
    nb->enPassant = b->enPassant;
    nb->quietMoves = b->quietMoves;
    nb->promo = b->promo;

    nb->moveStack = StackCopy(b->moveStack);

    memcpy(&nb->bbs, &b->bbs, sizeof(nb->bbs));
    memcpy(&nb->pieces, &b->pieces, sizeof(nb->pieces));

    if (nb->moveStack == NULL) {
        free(nb);
    }

    return nb;
}


bool BoardEQ(Board b1, Board b2) {
    for (int i = 0; i < 12; i++) {
        if (b1->bbs[i] != b2->bbs[i]) {
            return false;
        }
    }

    return b1->whiteToMove == b2->whiteToMove &&
           b1->gameOver == b2->gameOver &&
           b1->enPassant == b2->enPassant &&
           b1->quietMoves == b2->quietMoves;
}


void PrintBoard(Board board) {
    int i, j;
    char c[64];

    MakeBoardPrintable(GetArrayRep(board), &c[0]);

    for (i = 7; i >= 0; i--) {  // rows (backwards so black is printed above)
        for (j = 0; j < 8; j++) {  // cols
            printf("%c ", c[8*i + j]);
        }
        printf("\n");
    }
    printf("\n");
}
