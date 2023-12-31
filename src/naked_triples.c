#include "naked_triples.h"
#include <stdlib.h>

int Find_naked_triple_values(Cell **p_cells, int *naked_triple_values)
{
    int naked_triple_count = 0;
    int naked_triple_array[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++){
        naked_triple_array[i] = 0;
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        if (p_cells[i]->num_candidates == 2 || p_cells[i]->num_candidates == 3){
            int* candidate_array = get_candidates(p_cells[i]); 
            for (int j = 0; j < p_cells[i]->num_candidates; j++){
                naked_triple_array[candidate_array[j]-1] += 1; 
            }
            free(candidate_array);
        }
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        if (naked_triple_array[i] >= 2){
            naked_triple_values[naked_triple_count++] = i+1;                  
        }
    }
    return naked_triple_count;
}


int Is_naked_triple_cell(Cell *p_cell, int value1, int value2, int value3) {
    if ((p_cell->num_candidates == 2 || p_cell->num_candidates == 3)) {
        if (is_candidate(p_cell, value1) || is_candidate(p_cell, value2) || is_candidate(p_cell, value3)) {
            for (int i = 0; i <= 9; i++) {
                if ((i != value1) && (i != value2) && (i != value3)) {
                    if (is_candidate(p_cell, i)) {
                        return false;
                    }
                }
            }
            return true;
        }
    }
    return false;
}


void Find_naked_triples(Cell **p_cells, NakedTriple *p_naked_triple,
                        int *p_counter)
{       
    int naked_triple_value_array[BOARD_SIZE];
    int num_value = Find_naked_triple_values(p_cells, naked_triple_value_array);
    for (int i = 0; i < num_value-2; i++){
        for (int j = i+1; j < num_value-1; j++){
            for (int k = j+1; k < num_value; k++){
                for (int l = 0; l < BOARD_SIZE-2; l++){
                    for (int m = l+1; m < BOARD_SIZE-1; m++){
                        for (int n = m+1; n < BOARD_SIZE; n++){
                            if (((Is_naked_triple_cell(p_cells[l], naked_triple_value_array[i], naked_triple_value_array[j], naked_triple_value_array[k])) && 
                                (Is_naked_triple_cell(p_cells[m], naked_triple_value_array[i], naked_triple_value_array[j], naked_triple_value_array[k])) && 
                                (Is_naked_triple_cell(p_cells[n], naked_triple_value_array[i], naked_triple_value_array[j], naked_triple_value_array[k])))) {
                                NakedTriple naked_triple_object;
                                naked_triple_object.p_cell1 = p_cells[l];
                                naked_triple_object.p_cell2 = p_cells[m];
                                naked_triple_object.p_cell3 = p_cells[n];
                                naked_triple_object.p_cells = p_cells;
                                naked_triple_object.value1 = naked_triple_value_array[i];
                                naked_triple_object.value2 = naked_triple_value_array[j];
                                naked_triple_object.value3 = naked_triple_value_array[k];
                                int duplicate = 0;
                                for (int o = 0; o < *p_counter; ++o) {
                                    if (naked_triple_object.value1 == p_naked_triple[o].value1 &&
                                        naked_triple_object.value2 == p_naked_triple[o].value2 &&
                                        naked_triple_object.value3 == p_naked_triple[o].value3 &&        
                                        naked_triple_object.p_cell1 == p_naked_triple[o].p_cell1 &&
                                        naked_triple_object.p_cell2 == p_naked_triple[o].p_cell2 &&
                                        naked_triple_object.p_cell3 == p_naked_triple[o].p_cell3) {
                                        duplicate = 1;
                                        break;
                                    }
                                }

                                if (!duplicate) {
                                    p_naked_triple[(*p_counter)++] = naked_triple_object;
                                }   
                            }
                        }
                    }
                }                               
            }
        }
    }
}

int naked_triples(SudokuBoard *p_board)
{
    NakedTriple p_naked_triple[BOARD_SIZE*BOARD_SIZE];
    int nt_counter = 0;
    for (int i = 0; i< BOARD_SIZE; i++){
        Find_naked_triples(p_board->p_cols[i], p_naked_triple, &nt_counter);
        Find_naked_triples(p_board->p_rows[i], p_naked_triple, &nt_counter);
        Find_naked_triples(p_board->p_boxes[i], p_naked_triple, &nt_counter);
    }
    
    NakedTriple solved_cell[BOARD_SIZE*BOARD_SIZE];
    int solved_cell_count = 0;
    for (int i = 0; i < nt_counter; i++){
        Cell **p_cells = p_naked_triple[i].p_cells;
        solved_cell[solved_cell_count++] = p_naked_triple[i];
        for (int k = 0; k < BOARD_SIZE; k++)
        {
            if (p_cells[k] != p_naked_triple[i].p_cell1 && p_cells[k] != p_naked_triple[i].p_cell2 && p_cells[k] != p_naked_triple[i].p_cell3) {
            int* candidate_array = get_candidates(p_cells[k]);
            int num_candidates = p_cells[k]->num_candidates;
            for (int j = 0; j < num_candidates; j++){
                if ((candidate_array[j]== p_naked_triple[i].value1)){
                    unset_candidate(p_cells[k], candidate_array[j]);
                }
                if ((candidate_array[j]== p_naked_triple[i].value2)){
                    unset_candidate(p_cells[k], candidate_array[j]);
                }
                if ((candidate_array[j]== p_naked_triple[i].value3)){
                    unset_candidate(p_cells[k], candidate_array[j]);
                }
            }
             
            free(candidate_array);
            }
        }                 
    }
    return nt_counter;
}