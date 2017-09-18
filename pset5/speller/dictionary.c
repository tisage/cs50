/**
 * Implements a dictionary's functionality.
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Prepare advanced data structure

// linked list node
typedef struct node{
    char word[LENGTH+1];
    struct node *next;
}node;

// hash table
#define HASHTABLE_SIZE 100000
node *hashtable[HASHTABLE_SIZE] = {NULL};

// words count
int word_count = 0;

// tracking load/unload dictionary
bool loaded = false;

/**
 * Hash function
 *      Convert words to int using the ASCII values of every letter in the word
 * cited from https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/
 */
int hash_it(char* needs_hashing){
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    return hash % HASHTABLE_SIZE;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word){
    // create char array to store word
    // because word is a const char* so that we cannot modify it
    // it's better to create a copy of word
    
    int len = strlen(word);
    char word_copy[len + 1];
    
    // copy the value of word to word_copy
    for (int i = 0; i < len; i++){
        word_copy[i] = tolower(word[i]); // lower-case and upper-case letter have differnt ASCII number
    }
    
    // add NULL at the end of word array to indicate the end of char array
    word_copy[len] = '\0';
    
    // get hash value
    int h = hash_it(word_copy);
    
    // assign cursor node to the first node of the bucket
    node* cursor = hashtable[h];
    
    // check until the end of the linked list
    while (cursor != NULL){
        if (strcasecmp(cursor->word, word_copy) == 0) // word is in our dictionary
            return true; 
        else
            // move cursor to the next node
            cursor = cursor->next;
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary){
    // open dictionary using File
    FILE* file = fopen(dictionary, "r");
    if (file == NULL){
        printf("Could not open dictionary.\n");
        return false;
    }

    while (true){
        // malloc a node for each new word
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL){
            printf("Could not malloc a new node.\n");
            unload();
            return false;
        }
        
        // read a word from the dictionary and store it in new_node->word
        fscanf(file, "%s", new_node->word);
        new_node->next = NULL;
        
        if (feof(file)){
            // hit end of file
            free(new_node);
            break;
        }

        word_count++; // found word in dictionary, word's counter increase by 1
        
        // use hash function to get index
        int h = hash_it(new_node->word);
        node* head = hashtable[h];
        
        // if bucket is empty, insert the first node
        if (head == NULL){
            hashtable[h] = new_node;
        }
        // if bucket is not empty, attach node to front of list
        // do not sort linked list
        else{
            new_node->next = hashtable[h];
            hashtable[h] = new_node;
        }
    }
    // close dictionary
    fclose(file);
    loaded = true;
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void){
    if (loaded)
        return word_count;
    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void){
    for (int i = 0; i < HASHTABLE_SIZE; i++){
        // traversing linked lists
        node *cursor = hashtable[i];
        while (cursor != NULL){
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    loaded = false;
    return true;
}
