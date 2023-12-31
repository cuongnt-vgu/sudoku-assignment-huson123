#include "naked_triples.h"
#include <stdlib.h>
#include <stdio.h>

// Global variable to mark the processed cells
bool processed[BOARD_SIZE][BOARD_SIZE];

// Helper function to check if three cells have naked triples
bool are_naked_triples(Cell *cell1, Cell *cell2, Cell *cell3)
{
    if (cell1->num_candidates != 3 || cell2->num_candidates != 3 || cell3->num_candidates != 3)
        return false;

    int *candidates1 = get_candidates(cell1);
    int *candidates2 = get_candidates(cell2);
    int *candidates3 = get_candidates(cell3);

    // Sort candidates to simplify comparison
    for (int i = 0; i < 2; ++i) {
        for (int j = i + 1; j < 3; ++j) {
            if (candidates1[i] > candidates1[j]) {
                int temp = candidates1[i];
                candidates1[i] = candidates1[j];
                candidates1[j] = temp;
            }

            if (candidates2[i] > candidates2[j]) {
                int temp = candidates2[i];
                candidates2[i] = candidates2[j];
                candidates2[j] = temp;
            }

            if (candidates3[i] > candidates3[j]) {
                int temp = candidates3[i];
                candidates3[i] = candidates3[j];
                candidates3[j] = temp;
            }
        }
    }

    // Check for valid combinations of candidates
    bool result = (candidates1[0] == candidates2[0] && candidates1[0] == candidates3[0] &&
                   candidates1[1] == candidates2[1] && candidates1[1] == candidates3[1] &&
                   (candidates1[2] == candidates2[2] || candidates1[2] == candidates3[2] || candidates2[2] == candidates3[2]));

    free(candidates1);
    free(candidates2);
    free(candidates3);

    return result;
}

// Helper function to remove candidates except the naked triples
void remove_candidates_except_triples(SudokuBoard *p_board, Cell *cell1, Cell *cell2, Cell *cell3)
{
    int unit_index;

    // Determine the unit index based on the cell's position
    if (cell1->row_index == cell2->row_index && cell1->row_index == cell3->row_index) {
        unit_index = cell1->row_index; // Row
    } else if (cell1->col_index == cell2->col_index && cell1->col_index == cell3->col_index) {
        unit_index = cell1->col_index; // Column
    } else {
        unit_index = cell1->box_index; // Box
    }

    Cell **unit_array; 

    // Determine the array for the unit
    if (cell1->row_index == cell2->row_index && cell1->row_index == cell3->row_index) {
        unit_array = p_board->p_rows[unit_index];
    } else if (cell1->col_index == cell2->col_index && cell1->col_index == cell3->col_index) {
        unit_array = p_board->p_cols[unit_index];
    } else {
        unit_array = p_board->p_boxes[unit_index];
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Check if the cell is different from the triple cells
        if (unit_array[i] != cell1 && unit_array[i] != cell2 && unit_array[i] != cell3) {
            
            // Loop through the candidates of the cell
            for (int j = 0; j < BOARD_SIZE; j++) {
                // Check if the candidate matches any of the candidates of the naked triple
                if (unit_array[i]->candidates[j] && 
                    (j + 1 == cell1->candidates[0] || j + 1 == cell1->candidates[1] || j + 1 == cell1->candidates[2] ||
                     j + 1 == cell2->candidates[0] || j + 1 == cell2->candidates[1] || j + 1 == cell2->candidates[2] ||
                     j + 1 == cell3->candidates[0] || j + 1 == cell3->candidates[1] || j + 1 == cell3->candidates[2])) {
                    // Remove the candidate from the cell
                    unset_candidate(unit_array[i], j + 1);
                }
            }
        }
    }
}

// Helper function to apply naked triples strategy
void naked_triples_apply_strategy(SudokuBoard *p_board, Cell *cell1, Cell *cell2, Cell *cell3)
{
    // Remove candidates that are naked triples from other cells in the same row, column, and box
    remove_candidates_except_triples(p_board, cell1, cell2, cell3);

    // Mark the cells as processed
    processed[cell1->row_index][cell1->col_index] = true;
    processed[cell2->row_index][cell2->col_index] = true;
    processed[cell3->row_index][cell3->col_index] = true;
}

// Main function to find and apply naked triples strategy
int naked_triples(SudokuBoard *p_board)
{
    int nakedTriplesCount = 0;

    // Initialize the processed array to false
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            processed[i][j] = false;
        }
    }

    // Check naked triples in rows
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 2; j++)
        {
            for (int k = j + 1; k < BOARD_SIZE - 1; k++)
            {
                for (int m = k + 1; m < BOARD_SIZE; m++)
                {
                    if (are_naked_triples(p_board->p_rows[i][j], p_board->p_rows[i][k], p_board->p_rows[i][m]) &&
                        !processed[i][j] && !processed[i][k] && !processed[i][m])
                    {
                        naked_triples_apply_strategy(p_board, p_board->p_rows[i][j], p_board->p_rows[i][k], p_board->p_rows[i][m]);
                        nakedTriplesCount++;
                        printf("%d %d ", i,j);
                        printf("%d %d ", i,k);
                        printf("%d %d ", i,m);
                    }
                }
            }
        }
    }

    // Check naked triples in columns
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 2; j++)
        {
            for (int k = j + 1; k < BOARD_SIZE - 1; k++)
            {
                for (int m = k + 1; m < BOARD_SIZE; m++)
                {
                    if (are_naked_triples(p_board->p_cols[i][j], p_board->p_cols[i][k], p_board->p_cols[i][m]) &&
                        !processed[j][i] && !processed[k][i] && !processed[m][i])
                    {
                        naked_triples_apply_strategy(p_board, p_board->p_cols[i][j], p_board->p_cols[i][k], p_board->p_cols[i][m]);
                        nakedTriplesCount++;
                        printf("%d %d ", i,j);
                        printf("%d %d ", i,k);
                        printf("%d %d ", i,m);
                    }
                }
            }
        }
    }

    // Check naked triples in boxes
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 2; j++)
        {
            for (int k = j + 1; k < BOARD_SIZE - 1; k++)
            {
                for (int m = k + 1; m < BOARD_SIZE; m++)
                {
                    if (are_naked_triples(p_board->p_boxes[i][j], p_board->p_boxes[i][k], p_board->p_boxes[i][m]) &&
                        !processed[p_board->p_boxes[i][j]->row_index][p_board->p_boxes[i][j]->col_index] &&
                        !processed[p_board->p_boxes[i][k]->row_index][p_board->p_boxes[i][k]->col_index] &&
                        !processed[p_board->p_boxes[i][m]->row_index][p_board->p_boxes[i][m]->col_index])
                    {
                        printf("%d %d ", i,j);
                        printf("%d %d ", i,k);
                        printf("%d %d ", i,m);
                        naked_triples_apply_strategy(p_board, p_board->p_boxes[i][j], p_board->p_boxes[i][k], p_board->p_boxes[i][m]);
                        nakedTriplesCount++;
                    }
                }
            }
        }
    }
    return nakedTriplesCount;
}
