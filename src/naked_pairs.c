#include "naked_pairs.h"
#include <stdlib.h>
#include <stdio.h>

// Global variable to mark the processed cells
bool processed[BOARD_SIZE][BOARD_SIZE];

// Helper function to check if two cells have naked pairs
bool are_naked_pairs(Cell *cell1, Cell *cell2)
{
    if (cell1->num_candidates != 2 || cell2->num_candidates != 2)
        return false;

    int *candidates1 = get_candidates(cell1);
    int *candidates2 = get_candidates(cell2);

    // Check if both cells have the same candidates
    bool result = (candidates1[0] == candidates2[0] && candidates1[1] == candidates2[1]) ||
                  (candidates1[0] == candidates2[1] && candidates1[1] == candidates2[0]);

    free(candidates1);
    free(candidates2);

    return result;
}

// Helper function to remove candidates except the naked pairs
void remove_candidates_except_pair(SudokuBoard *p_board, Cell *currentCell, Cell *pairCell, int *candidates1, int *candidates2)
{
    int unit_index;

    // Determine the unit index based on the cell's position
    if (currentCell->row_index == pairCell->row_index) {
        unit_index = currentCell->row_index; // Row
    } else if (currentCell->col_index == pairCell->col_index) {
        unit_index = currentCell->col_index; // Column
    } else {
        unit_index = currentCell->box_index; // Box
    }

    Cell **unit_array; // Adjust this based on your structure

    // Determine the array for the unit
    if (currentCell->row_index == pairCell->row_index) {
        unit_array = p_board->p_rows[unit_index];
    } else if (currentCell->col_index == pairCell->col_index) {
        unit_array = p_board->p_cols[unit_index];
    } else {
        unit_array = p_board->p_boxes[unit_index];
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Check if the cell is different from the pair cells
        if (unit_array[i] != currentCell && 
            (unit_array[i]->row_index != pairCell->row_index ||
             unit_array[i]->col_index != pairCell->col_index ||
             unit_array[i]->box_index != pairCell->box_index)) {
            
            // Loop through the candidates of the cell
            for (int j = 0; j < BOARD_SIZE; j++) {
                // Check if the candidate matches any of the candidates of the naked pair
                if (unit_array[i]->candidates[j] && (j + 1 == candidates1[0] || j + 1 == candidates1[1])) {
                    // Remove the candidate from the cell
                    unset_candidate(unit_array[i], j + 1);
                    // Mark the cell as having candidates removed
                    unit_array[i]->candidates_removed = true;
                }
            }
        }
    }
}

// Helper function to apply naked pairs strategy
void naked_pairs_apply_strategy(SudokuBoard *p_board, Cell *cell1, Cell *cell2)
{
    int *candidates1 = get_candidates(cell1);
    int *candidates2 = get_candidates(cell2);

    // Remove candidates that are naked pairs from other cells in the same row, column, and box
    remove_candidates_except_pair(p_board, cell1, cell2, candidates1, candidates2);
    remove_candidates_except_pair(p_board, cell2, cell1, candidates1, candidates2);

    // Mark the cells as processed
    processed[cell1->row_index][cell1->col_index] = true;
    processed[cell2->row_index][cell2->col_index] = true;

    free(candidates1);
    free(candidates2);
}

// Main function to find and apply naked pairs strategy
int naked_pairs(SudokuBoard *p_board)
{
    int nakedPairsCount = 0;

    // Initialize the processed array to false
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            processed[i][j] = false;
        }
    }

    // Check naked pairs in rows
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 1; j++)
        {
            for (int k = j + 1; k < BOARD_SIZE; k++)
            {
                if (are_naked_pairs(p_board->p_rows[i][j], p_board->p_rows[i][k]) && !processed[i][j] && !processed[i][k])
                {
                    naked_pairs_apply_strategy(p_board, p_board->p_rows[i][j], p_board->p_rows[i][k]);
                    nakedPairsCount++;
                }
            }
        }
    }

    // Check naked pairs in columns
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 1; j++)
        {
            for (int k = j + 1; k < BOARD_SIZE; k++)
            {
                if (are_naked_pairs(p_board->p_cols[i][j], p_board->p_cols[i][k]) && !processed[j][i] && !processed[k][i])
                {
                    naked_pairs_apply_strategy(p_board, p_board->p_cols[i][j], p_board->p_cols[i][k]);
                    nakedPairsCount++;
                }
            }
        }
    }

    // Check naked pairs in boxes
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 1; j++)
        {
            for (int k = j + 1; k < BOARD_SIZE; k++)
            {
                if (are_naked_pairs(p_board->p_boxes[i][j], p_board->p_boxes[i][k]) && !processed[p_board->p_boxes[i][j]->row_index][p_board->p_boxes[i][j]->col_index] && !processed[p_board->p_boxes[i][k]->row_index][p_board->p_boxes[i][k]->col_index] && !p_board->p_boxes[i][k]->candidates_removed)
                {
                    nakedPairsCount++;  
                    naked_pairs_apply_strategy(p_board, p_board->p_boxes[i][j], p_board->p_boxes[i][k]);
                }
            }
        }
    }

    return nakedPairsCount;
}
