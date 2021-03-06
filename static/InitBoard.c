/*

Author: Ben Eggers <ben.eggers36@gmail.com>
License: MIT

Definition for the inital board configuration.

*/

#include "InitBoard.h"
#include "BitBoard.h"

const char INIT_BOARD_CHARS[64] = {
                               'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
                               'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
                               '.', '.', '.', '.', '.', '.', '.', '.',
                               '.', '.', '.', '.', '.', '.', '.', '.',
                               '.', '.', '.', '.', '.', '.', '.', '.',
                               '.', '.', '.', '.', '.', '.', '.', '.',
                               'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
                               'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'
                              };

const bb_t INIT_BOARD_BBREP[12] = {
                                0x000000000000ff00,
                                0x00ff000000000000,
                                0x0000000000000042,
                                0x4200000000000000,
                                0x0000000000000024,
                                0x2400000000000000,
                                0x0000000000000081,
                                0x8100000000000000,
                                0x0000000000000008,
                                0x0800000000000000,
                                0x0000000000000010,
                                0x1000000000000000
                              };
