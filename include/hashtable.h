#ifndef HASHTABLE_H
#define HASHTABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "functions.h"


#define NUMBER_OF_INDEXES_OF_HASHTABLE 17
#define true 1
#define false 0


typedef struct page_table_node{

    char *page_number;
    int frame_number;
    int modified_page; //1 if page is modified, 0 if its not modified
    struct page_table_node *next_page_table_node;

}page_table_node;



int hash_function(char *str);

page_table_node *create_page_table(page_table_node *p);

page_table_node *insert_page_table(page_table_node *p, char *page_number, int frame_number, char read_or_write, statistics *stats);

page_table_node *remove_node_page_table(page_table_node *p, char *page_number, char read_or_write, statistics *stats);

int modify_page(page_table_node *p, char *page_number, statistics *stats);

int find_page_number(page_table_node *p, char *page_number);

int print_page_table(page_table_node *p);

int delete_page_table(page_table_node **page_table, statistics *stats);

#endif