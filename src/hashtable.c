#include "../include/hashtable.h"


int hash_function(char *str){

    int i=0;
    int hash = 7;

    for (i=0;i<strlen(str);i++) {
        hash = hash*31 + str[i];
    }
        
    return hash % NUMBER_OF_INDEXES_OF_HASHTABLE;
}



page_table_node *create_page_table(page_table_node *p){

    p=NULL;
    return p;
}



page_table_node *insert_page_table(page_table_node *p, char *page_number, int frame_number, char read_or_write, statistics *stats){

    page_table_node *aux=NULL;

	if(p==NULL){

		p = (page_table_node *)calloc(1, sizeof(page_table_node));
        if(p==NULL){

            printf("Sorry, cannot allocate memory\n");
            exit(1);
        }

        //Initialize node
        p->page_number = calloc(1, 6);
		strcpy(p->page_number, page_number);
		p->frame_number=frame_number;
        p->next_page_table_node = NULL;

        if(read_or_write == 'R'){

            p->modified_page=0;
        }
        else if(read_or_write == 'W'){

            p->modified_page=1;
        }
    }
    else{

        aux=p;
        while(aux->next_page_table_node!=NULL){

            aux=aux->next_page_table_node;
        }

        //Add new node
        page_table_node *new_page=calloc(1, sizeof(page_table_node));
        if(new_page==NULL){

            printf("Sorry, cannot allocate memory\n");
            exit(1);
        }

        //Initialize node
        new_page->page_number = calloc(1, 6);
		strcpy(new_page->page_number, page_number);
		new_page->frame_number=frame_number;
        new_page->next_page_table_node = NULL;
        //connect previous last node to last
        aux->next_page_table_node=new_page;

        if(read_or_write == 'R'){

            new_page->modified_page=0;
        }
        else if(read_or_write == 'W'){

            new_page->modified_page=1;
        }
	}

	return p;
}




page_table_node *remove_node_page_table(page_table_node *p, char *page_number, char read_or_write, statistics *stats){

    page_table_node *aux=NULL;
    page_table_node *previous=NULL, *throwaway=NULL;

    previous=NULL;
    throwaway=NULL;
    aux=p;
	
	while(aux!=NULL){
        
        previous = aux;

        if(!strcmp(aux->page_number, page_number) && aux->frame_number!=-10){

            previous=throwaway;
            throwaway = NULL;
            
            aux->frame_number=-10;
            
            if(aux->modified_page==1){

                stats->cnt_write_to_disk++;
            }
            throwaway=previous;

            return p;
        }

		aux=aux->next_page_table_node;
	}

    return p;
}




int find_page_number(page_table_node *p, char *page_number){

    page_table_node *aux=NULL;

    aux=p;

    while(aux!=NULL){

		if(!strcmp(aux->page_number, page_number) && aux->frame_number!=-10)
			return true;

		aux=aux->next_page_table_node;
	}

    //if not found return 0 (false).
	return false;
}




int print_page_table(page_table_node *p){

    page_table_node *aux=NULL;
    if(p == NULL){

		printf("Current index is empty.\n");
		return -1;
	}
	aux = p;
	while(aux!= NULL){

        if(aux->frame_number!=-10){

            printf("Page number =  -%s-\n", aux->page_number);
            printf("Frame number =  %d\n", aux->frame_number);
            printf("Modified page =  %d\n\n\n", aux->modified_page);
        }
		aux=aux->next_page_table_node;
	}

    return 0;
}



int delete_page_table(page_table_node **page_table, statistics *stats){
    
    page_table_node *aux=NULL;
    page_table_node *temp=NULL;

    for(int i=0;i<NUMBER_OF_INDEXES_OF_HASHTABLE;i++){

        if(page_table[i]!=NULL){

            aux=page_table[i];

            while(aux!=NULL){

                if(aux->modified_page==1 && aux->frame_number!=-10){

                    stats->cnt_write_to_disk++;
                }

                temp=aux;
                aux=aux->next_page_table_node;
                free(temp->page_number);
                free(temp);
                temp=NULL;
            }
            
            page_table[i]=NULL;
        }

    }

    return 0;
}




int modify_page(page_table_node *p, char *page_number, statistics *stats){

    page_table_node *aux=NULL;

    aux=p;

    while(aux!=NULL){

		if(!strcmp(aux->page_number, page_number) && aux->frame_number!=-10 && aux->modified_page==0){

            aux->modified_page=1;
        }

		aux=aux->next_page_table_node;
	}

	return 0;
}