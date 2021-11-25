#ifndef SECOND_CHANCE_H
#define SECOND_CHANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "hashtable.h"
#include "functions.h"


typedef struct Second_Chance_info{

    char *page_number;
    double time;
    int pid;
    char read_or_write;
    int reference_bit;

}Second_Chance_info;


void swap(Second_Chance_info *x, Second_Chance_info *y);

void bubbleSort(Second_Chance_info *sc_array, int n);

Second_Chance_info *find_victim_and_replace_second_chance(Second_Chance_info *Second_Chance_array, Second_Chance_info *memory_info, 
                                                          char *page_number, double current_time, int pid, char read_or_write, int max_frames);

int second_chance(page_table_node **page_table_gcc, page_table_node **page_table_bzip, char *page_number, char read_or_write,
        int *current_number_of_frames, int max_frames, double current_time, int pid, Second_Chance_info *Second_Chance_array, statistics *stats);


#endif