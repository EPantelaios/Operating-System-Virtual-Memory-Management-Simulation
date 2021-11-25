#include "../include/lru.h"


LRU_info *find_victim_and_replace_lru(LRU_info *LRU_array, LRU_info *memory_info, char *page_number, double current_time, int pid, char read_or_write, int max_frames){

    int index_of_array=0;
    
    int min=INT_MAX;
    int i=0;
    
    for(i=0; i<max_frames; i++){

        if(LRU_array[i].time!=-1 && LRU_array[i].time<min){
            index_of_array=i;
            min=LRU_array[index_of_array].time;
        }
    }    
    
    //store the information to return page number and process ID.
    strcpy(memory_info->page_number, LRU_array[index_of_array].page_number);
    memory_info->pid = LRU_array[index_of_array].pid;
    memory_info->read_or_write = LRU_array[index_of_array].read_or_write;



    //Replace new values at array
    strcpy(LRU_array[index_of_array].page_number, page_number);
    LRU_array[index_of_array].time = current_time;
    LRU_array[index_of_array].pid = pid;
    LRU_array[index_of_array].read_or_write = read_or_write;
    
    return memory_info;
}




int LRU(page_table_node **page_table_gcc, page_table_node **page_table_bzip, char *page_number, 
         char read_or_write, int *current_number_of_frames, int max_frames, double current_time, int pid, LRU_info *LRU_array, statistics *stats){


    int find_page=false;

    LRU_info *memory_info=calloc(1, sizeof(LRU_info));
    memory_info->page_number=calloc(1, 6);


    int hash_index=hash_function(page_number);
  
    find_page = find_page_number(page_table_gcc[hash_index], page_number);

    if(find_page==false){

        find_page = find_page_number(page_table_bzip[hash_index], page_number);

        //Page not found to either of the two hashtables.
        //Insert new page.
        if(find_page==false){
            
            stats->cnt_page_fault++;
            
            if(*current_number_of_frames < max_frames){

                stats->cnt_read_from_disk++;
                
                if(pid==0)
                    page_table_gcc[hash_index] = insert_page_table(page_table_gcc[hash_index], page_number, *current_number_of_frames, read_or_write, stats);
                else if(pid==1)
                    page_table_bzip[hash_index] = insert_page_table(page_table_bzip[hash_index], page_number, *current_number_of_frames, read_or_write, stats);
                    
                for(int i=0;i<max_frames;i++){
                    
                    if(LRU_array[i].time==-1){

                        strcpy(LRU_array[i].page_number, page_number);
                        LRU_array[i].time=current_time;
                        if(pid==0)
                            LRU_array[i].pid=0;
                        else if(pid==1)
                            LRU_array[i].pid=1;
                        LRU_array[i].read_or_write=read_or_write;

                        *current_number_of_frames = *current_number_of_frames + 1;

                        free(memory_info->page_number);
                        free(memory_info);
                        
                        return 0;
                    }
                }
            }
            else{   //Page not found and memory is full.

                stats->cnt_read_from_disk++;
               
                
                if(pid==0)
                    memory_info = find_victim_and_replace_lru(LRU_array, memory_info, page_number, current_time, 0, read_or_write, max_frames);
                else if(pid==1)
                    memory_info = find_victim_and_replace_lru(LRU_array, memory_info, page_number, current_time, 1, read_or_write, max_frames);


                //Calculate the new hash for the deleted page (page replacement)
                hash_index=hash_function(memory_info->page_number);

                //For gcc process (pid=0).
                if(memory_info->pid==0){

                    page_table_gcc[hash_index]=remove_node_page_table(page_table_gcc[hash_index], memory_info->page_number, memory_info->read_or_write, stats);

                    if(page_table_gcc[hash_index]==NULL){

                        printf("1111111\n");
                        exit(1);
                    }

                }
                else if(memory_info->pid==1){  //For bzip process (pid=1).

                   page_table_bzip[hash_index]=remove_node_page_table(page_table_bzip[hash_index], memory_info->page_number, memory_info->read_or_write, stats);

                    if(page_table_bzip[hash_index]==NULL){
                        
                        printf("222222\n");
                        exit(1);
                    }
                }

                hash_index=hash_function(page_number);
                
                if(pid==0)
                    page_table_gcc[hash_index] = insert_page_table(page_table_gcc[hash_index], page_number, *current_number_of_frames, read_or_write, stats);
                else if(pid==1)
                    page_table_bzip[hash_index] = insert_page_table(page_table_bzip[hash_index], page_number, *current_number_of_frames, read_or_write, stats);


                free(memory_info->page_number);
                free(memory_info);
                
                return 0;
            }   
        }
        
    }

    if(read_or_write=='W'){

        if(pid==0)
            modify_page(page_table_gcc[hash_index], page_number, stats);
        else if(pid==1)
            modify_page(page_table_bzip[hash_index], page_number, stats);
    }
    

    for(int i=0;i<max_frames;i++){

        if((!strcmp(LRU_array[i].page_number, page_number)) && LRU_array[i].time!=-1){

            LRU_array[i].time=current_time;
            LRU_array[i].read_or_write=read_or_write;
        }
    }


    free(memory_info->page_number);
    free(memory_info);

    return 0;
}