#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "../include/hashtable.h"
#include "../include/functions.h"
#include "../include/lru.h"
#include "../include/second_chance.h"


int main(int argc, char *argv[]){

    char *arg;
    const char lru[4]="lru";
    const char sec_chance[14]="second_chance";
    const int pid_gcc=0;
    const int pid_bzip=1;
    char algo_replacement[8];
    char read_or_write;
    char *trace=calloc(1, 6);
    int max_frames=0;
    int q=0;
    int max_traces_each_file=1000;
    int cnt_traces=0;
    double current_time=0.0;
    int current_number_of_frames=0;
    FILE *fp_gcc, *fp_bzip;


    if(argc<7 || argc>9 || argc==8){

		printf("Please enter all the required arguments. Try again. Exit...\n");
		exit(0);
	}
	
	//Check the parameters given at command line
	while(--argc){

		arg = *++argv;
		
		if(!strcmp(arg, "-alg_replacement")){	
			if(argc > 1 && --argc){

                strcpy(algo_replacement, *++argv);

                if(!strcmp(lru, algo_replacement) && !strcmp(sec_chance, algo_replacement)){
                    printf("Invalid replacement algorithm. Try again. Exit...\n");
                    exit(1);
                }
            }
		}
		else if(!strcmp(arg, "-frames")){	
			if(argc > 1 && --argc) 
				max_frames = atoi(*++argv);
		}
		else if(!strcmp(arg, "-q")){
			if(argc > 1 && --argc) 
				q = atoi(*++argv);
		}
		else if(!strcmp(arg, "-trace_each_file")){
			if(argc > 1 && --argc) 
				max_traces_each_file = atoi(*++argv);
        }
		else{
			printf("FALSE arguments. Try again. Exit...\n");
			exit(1);
		}
	}

    //limit 1 million traces for each file
    if(max_traces_each_file>1000000){

        printf("Error. Max number of traces is 1 million for these files. Try again. Exit...\n");
    }


    //Open the two files
    if((fp_gcc=fopen("gcc.trace", "r"))==NULL){

        //if cannot open the file exit
        printf("Could not open file 'gcc.trace'. Exit...\n");
        exit(1);
    }

    if((fp_bzip=fopen("bzip.trace", "r"))==NULL){

        //if cannot open the file exit
        printf("Could not open file 'bzip.trace'. Exit...\n");
        exit(1);
    }


    //Initialize hashtables and arrays
    page_table_node **page_table_gcc=NULL;
    page_table_node **page_table_bzip=NULL; 
    statistics *stats=NULL;
    LRU_info *LRU_array=NULL;
    Second_Chance_info *Second_Chance_array=NULL;

    page_table_gcc = calloc(NUMBER_OF_INDEXES_OF_HASHTABLE, sizeof(page_table_node *));
    if(page_table_gcc==NULL){

		printf("Sorry, cannot allocate memory\n");
		return -1;
	}

    page_table_bzip = calloc(NUMBER_OF_INDEXES_OF_HASHTABLE, sizeof(page_table_node *));
    if(page_table_bzip==NULL){

		printf("Sorry, cannot allocate memory\n");
		return -1;
	}	

    stats = calloc(1, sizeof(statistics));
    if(stats==NULL){

		printf("Sorry, cannot allocate memory\n");
		return -1;
	}	

    stats->cnt_read_from_disk=0;
    stats->cnt_write_to_disk=0;
    stats->cnt_page_fault=0;
    stats->cnt_entries_from_each_trace_file=max_traces_each_file;
    stats->cnt_entries_from_each_trace_file=max_traces_each_file;
    if(max_frames>max_traces_each_file*2)
        stats->cnt_frames=max_traces_each_file*2;
    else
         stats->cnt_frames=max_frames;
    strcpy(stats->algo_replacement, algo_replacement);
    stats->q=q;
    
    //Select and initialize appropriate data structure based on the replacement algorithm
    if(!strcmp(lru, algo_replacement)){

        LRU_array = calloc(max_frames, sizeof(LRU_info));
        if(LRU_array==NULL){

            printf("Sorry, cannot allocate memory\n");
            return -1;
        }	

        for(int i=0;i<max_frames;i++){
            LRU_array[i].time=-1;
            LRU_array[i].page_number=calloc(1, 6);
        }
    }
    else{

        Second_Chance_array = calloc(max_frames, sizeof(Second_Chance_info));
        if(Second_Chance_array==NULL){

            printf("Sorry, cannot allocate memory\n");
            return -1;
        }	

        for(int i=0;i<max_frames;i++){

            Second_Chance_array[i].time=-1;
            Second_Chance_array[i].page_number=calloc(1, 6);
        }
    }


    for(int i=0;i<NUMBER_OF_INDEXES_OF_HASHTABLE;i++){

        page_table_gcc[i]=create_page_table(page_table_gcc[i]);
        page_table_bzip[i]=create_page_table(page_table_bzip[i]);
    }


    while(cnt_traces<max_traces_each_file){

        //read and apply 'q' traces from gcc.trace file        
        for(int i=0;i<q;i++){
            
            //read exactly the required number of traces
            if(cnt_traces+i>=max_traces_each_file){

                cnt_traces = max_traces_each_file;
                break;
            }

            //read one trace from file (removing offset)
            assert(!read_trace(fp_gcc, trace, &read_or_write));

            if(!strcmp(lru, algo_replacement)){

                LRU(page_table_gcc, page_table_bzip, trace, read_or_write, &current_number_of_frames, max_frames, current_time, pid_gcc, LRU_array, stats);
            }
            else{

                second_chance(page_table_gcc, page_table_bzip, trace, read_or_write, &current_number_of_frames, max_frames, current_time, pid_gcc, Second_Chance_array, stats);
            }

            current_time++;
        }


        //read and apply 'q' traces from bzip.trace file
        for(int j=0;j<q;j++){
            
            //read exactly the required number of traces
            if(cnt_traces+j>=max_traces_each_file){

                cnt_traces = max_traces_each_file;
                break;
            }
            
            //read one trace from file (removing offset)
            assert(!read_trace(fp_bzip, trace, &read_or_write));

            if(!strcmp(lru, algo_replacement)){

                LRU(page_table_gcc, page_table_bzip, trace, read_or_write, &current_number_of_frames, max_frames, current_time, pid_bzip, LRU_array, stats);
            }
            else{

                second_chance(page_table_gcc, page_table_bzip, trace, read_or_write, &current_number_of_frames, max_frames, current_time, pid_bzip, Second_Chance_array, stats);
            }

            current_time++;
        }

    
        cnt_traces = cnt_traces + q;
    }

    
    //deleted all structures used
    assert(!delete_page_table(page_table_gcc, stats));
    assert(!delete_page_table(page_table_bzip, stats));

    print_statistics(stats);

    free(page_table_gcc);
    free(page_table_bzip);
    free(stats);
    free(trace);

    if(!strcmp(lru, algo_replacement)){

        for(int i=0;i<max_frames;i++)
            free(LRU_array[i].page_number);
        free(LRU_array);
    }
    else{

        for(int i=0;i<max_frames;i++)
            free(Second_Chance_array[i].page_number);
        free(Second_Chance_array);
    }

    assert(!fclose(fp_gcc));
    assert(!fclose(fp_bzip));

    return 0;
}