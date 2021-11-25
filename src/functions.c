#include "../include/functions.h"


int read_trace(FILE *fp, char *str_token, char *read_or_write){

    char current_line[20];

    fgets(current_line, 20, fp);

    for(int i=0;i<5;i++)
        str_token[i]=current_line[i];
    
    read_or_write[0]=current_line[9];

    return 0;
}


void print_statistics(statistics *stats){

    printf("\n----> For Algorithm '%s' with q = %d <----\n\n", stats->algo_replacement, stats->q); 
    printf("Number of traces read from 'gcc.trace' process: %d\n", stats->cnt_entries_from_each_trace_file);
    printf("Number of traces read from 'bzip.trace' process: %d\n", stats->cnt_entries_from_each_trace_file);
    printf("Number of frames: %d\n", stats->cnt_frames);
    printf("Number of page faults: %d\n", stats->cnt_page_fault);
    printf("Number of reads from disk: %d\n", stats->cnt_read_from_disk);
    printf("Number of writes to disk: %d\n", stats->cnt_write_to_disk);
}