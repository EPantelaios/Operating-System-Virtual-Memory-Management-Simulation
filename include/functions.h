#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct statistics{

    int cnt_page_fault;
    int cnt_read_from_disk;
    int cnt_write_to_disk;
    int cnt_entries_from_each_trace_file;
    int cnt_frames;
    char algo_replacement[20];
    int q;

}statistics;


int read_trace(FILE *fp, char *str_token, char *read_or_write);

void print_statistics(statistics *stats);


#endif