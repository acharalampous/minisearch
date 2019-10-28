/*******************************/
/* PostingList.h */

/* Name:    Andreas Charalampous
 * A.M :    1115201500195
 * e-mail:  sdi1500195@di.uoa.gr
 */
/********************************/
#pragma once

#include "DocMap.h"
#include "aclib.h"

/*  Struct that implements the posting list of a word. Specifically
 *  it holds info about all documents that the word is found in, as
 *  well as how many times in each document
 */

/* Standard model of list-nodes
 * Specs:   * Single-Linked List
 *          * Pointer at start
 *          * LIFO Insertion at start
 */


/* Implementation of the Posting List node */
typedef struct PLNode{
    int id; // id of text where word is found
    int times; // times that the word is repeated in text
    struct PLNode* next; // Next node of posting list
}PLNode;

/* Implementation of Posting List */
typedef struct PostingList{
    PLNode* start; // points at the first node of list
    int numOfPosts; // number of posts that the word exists in
}PostingList;



/***************/
/*** PL NODE ***/
/***************/
/* All functions about Posting List nodes start with PLN_   */

/* Initializes a new PL Node and returns a pointer to it.   */
/* Takes as parameter the document id. Returns NULL if err. */
PLNode* PLN_Init(int);

/* Prints info of given PLNode */
void PLN_Print(PLNode*);

/* Increases time of occurences of given PLNode by one */
void PLN_increaseTimes(PLNode*);



/********************/
/*** POSTING LIST ***/
/********************/
/* All functions about Posting List start with PL_ */

/* Initializes a new Posting List and returns a pointer to  */
/* it. Takes as parameter the document id. Returns NULL in  */
/* case of error.                                           */
PostingList* PL_Init();

/* Takes as argument a posting and returns 1 if empty or 0  */
/* if not.                                                  */
int PL_isEmpty(PostingList*);

/* Returns the number of posts of specific posting list     */
int PL_getNumOfPosts(PostingList*);

/* Inserts(pushes) node at start of list */
void PL_Push(PostingList*, PLNode*);

/* Returns nod e with given id. If not found, returns NULL.  */
PLNode* PL_FindNode(PostingList*, int);

/* Checks if start of posting list equals to id */
PLNode* PL_checkStart(PostingList*, int);

/* Deletes(pops) the first node. If list is empty, returns -1 */
int PL_Pop(PostingList*);

/*  Given the posting and documents, find the score of each */
/*  document for the specific word(of posting list)         */
void PL_getScores(PostingList*, heapDoc**, DocMap*);

/* Destroy given list and all of its nodes. Removes the first */
/* node, until all nodes are deleted                          */
void PL_Destroy(PostingList*);

/* Prints info of given Posting List */
void PL_Print(PostingList*);


