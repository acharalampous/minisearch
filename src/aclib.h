/*******************************/
/* aclib.h */

/* Name:    Andreas Charalampous
 * A.M :    1115201500195
 * e-mail:  sdi1500195@di.uoa.gr
 */
/********************************/
#pragma once
#include "DocMap.h"

/*  Header file for all variant functions and structs used
 *  to complete the minisearch engine.
 */


/*  Struct that will be used complete the /search command.  */
/*  Holds the id and score of each document in queries. A   */
/*  table will store all heapDocs. Functions were           */
/*  implemented so that the table can be heapsorted.        */
typedef struct heapDoc{
    int id;
    float score;
}heapDoc;

/*  All function starting by S_ are used for heapsort in    */
/*  search queries.                                         */

/*  Takes as parameter a table that stores heapDocs and     */
/*  sorts it using heapsort. The number of the last cell is */
/*  passed so that heapsort is succesfull                   */
void S_HeapSort(heapDoc**, int);


/*  Takes as argument the tables, 2 indexes and returns the */
/*  index of the maximum element                            */
int S_Max(heapDoc**, int, int);


/*  Swaps the places of 2 elements in a table               */
void S_Swap(heapDoc**, int, int);


/*  Gets the maximum value in heap table, which is in the   */
/*  first cell of it. Then it brings the last one on top and*/
/*  pushes it back down, until the heap is created again    */
heapDoc* S_GetMax(heapDoc**, int*);


/*  Print the document result in the form (i) (id) [score]""*/
void S_PrintResult(heapDoc*, DocMap*, char**, int, int);

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

/*  Start searching from the end of file, until the last    */
/*  line is reached. Then it reads the id. This is the      */
/*  number of lines. Finally it is returned                 */
int getNumOfLines(FILE* fp);


/*  Overpass the spaces( /t/n) in string and return pointer */
/*  to it                                                   */
char* discardSpaces(char* src);


/*  Get the id of the given document(line). Basically it    */
/*  points to the first word of document. Then returns the  */
/*  pointer.                                                */
char* getID(char* src);


/*  Given a string, it check char-char to see if integer.   */
/*  Is yes, returns 1, else 0.                              */
int isNumber(char* str);


/*  strcpy Alternative. Allocates spaces and returns pointer*/
/*  to the copied string                                    */
char* myStrCopy(char*, int);


/*  Print options(/search,/df,/tf/exit..)                   */
void printOptions();


/*  Print the valid form of given parameters                */
void exeParameters();
