#pragma once

#include "sudoku.h"

typedef struct HiddenPair_impl {
    Cell *p_cells[2];
    int value1;
    int value2;
} HiddenPair;

int hidden_pairs(SudokuBoard *p_board);
bool Is_hidden_pairs(Cell **p_cells, int value1, int value2);
bool Is_in_cell(Cell *p_cell, int value1, int value2);
int Find_hidden_pair_values(Cell **p_cells, int *hidden_pair_values);
void Find_hidden_pairs(Cell **p_cells, HiddenPair *p_hidden_pair, int *p_counter);