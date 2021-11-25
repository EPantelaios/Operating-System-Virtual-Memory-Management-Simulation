#ifndef LRU_H
#define LRU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "hashtable.h"
#include "functions.h"


typedef struct LRU_info{

    char *page_number;
    double time;
    int pid;
    char read_or_write;

}LRU_info;



LRU_info *find_victim_and_replace_lru(LRU_info *LRU_array, LRU_info *memory_info, char *page_number, 
                                      double current_time, int pid, char read_or_write, int max_frames);


int LRU(page_table_node **page_table_gcc, page_table_node **page_table_bzip, char *page_number, char read_or_write,
        int *current_number_of_frames, int max_frames, double current_time, int pid, LRU_info *LRU_array, statistics *stats);


#endif