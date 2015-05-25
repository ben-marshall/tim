/*!
@ingroup sw-asm
@{
@file asm_hash_table.c
@brief Contains all functions that operate on the asm_hash_table datastructure.
*/

#include "asm.h"

/*!
@brief Creates and returns a new pointer to a hash table.
@param initial_size - The initial size of the hash table's internal data structure.
*/
void asm_hash_table_new(int initial_size, asm_hash_table * tr)
{
    tr -> element_count = 0;
    tr -> current_size  = initial_size;

    tr -> buckets = calloc(initial_size, sizeof(asm_hash_table_bin));
}


/*
@brief This function expands the internal datastructure of a hash table.
*/
int asm_hash_table_expand(asm_hash_table * table, int new_size)
{
    printf("%d %s NOT IMPLEMENTED\n", __LINE__, __FILE__);
}

/*!
@brief Returns a hash key of a string.
*/
asm_hash_key asm_hash_key_string(char * string, int table_size)
{
    int keylen = 0;
    char start = string[keylen];
    while(start != ';' && start != '\0')
    {
        keylen++;
        start = string[keylen];
    }

    int i = 0;
    asm_hash_key tr = 0;
    while(i < keylen)
    {
        tr = (tr << 3) ^ string[i];
        tr = tr % table_size;
        i ++;
    }

    return tr;
}

/*!
@brief Inserts an element into the hash table associated with the provided key.
@param table - Pointer to the hash table to insert into.
@param key - The key to the data.
@param data - Pointer to the data the table will contain.
@returns a status code. Zero if it worked, otherwise some integer.
@todo Check that the key will fit!!
*/
int asm_hash_table_insert(asm_hash_table * table, char * key, void * data)
{

    asm_hash_key binkey = asm_hash_key_string(key,table -> current_size);

    if(table -> buckets [binkey].data == NULL)
    {
        table -> buckets[binkey].data = data;
        table -> buckets[binkey].key = key;
        table -> buckets[binkey].used = 1;
    }
    else
    {
        asm_hash_table_bin * walker = &table -> buckets[binkey];
        while(walker -> next != NULL)
            walker = walker -> next;

        walker -> next = calloc(1, sizeof(asm_hash_table_bin));
        walker -> next -> key = key;
        walker -> next -> data = data;
        walker -> next -> used = 1;
    }

    table -> element_count ++;
}

/*!
@brief Returns a pointer to the data stored in a hash table with the given key or NULL if no such
       element exists.
@param table - The table to fetch the data from.
@param key - The key to the data to fetch.
@todo Make sure the key fits into the table.
*/
void * asm_hash_table_get(asm_hash_table * table, char * strkey)
{
    void * tr = NULL;
    asm_hash_key binkey = asm_hash_key_string(strkey, table->current_size);
    
    if(table -> buckets[binkey].used != 0)
    {
        if(strcmp(table -> buckets [binkey].key, strkey)==0)
        {
            tr = table -> buckets[binkey].data;
        }
        else
        {
            asm_hash_table_bin * walker = &table -> buckets[binkey];
            while(walker -> next != NULL && *walker -> key != binkey)
                walker = walker -> next;

            if(*walker -> key == binkey)
                tr = walker -> data;
        }
    }

    return tr;
}

//! }@
