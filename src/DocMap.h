/*******************************/
/* DocMap.h */

/* Name:    Andreas Charalampous
 * A.M :    1115201500195
 * e-mail:  sdi1500195@di.uoa.gr
 */
/********************************/
#pragma once

/*  Struct that stores the text of each document, id and number
 *  of words. Used for mapping id to text
 */

/* Stores info about a specific document */
typedef struct DocInfo{
    int id; // id of text
    char* text; // the text of document
    int numOfWords; // number of words in text
}DocInfo;

/* Stores all the documents and total number of words */
typedef struct DocMap{
    DocInfo** Docs; // array that all documents are stored
    int numOfDocs; // number of documents
    int totalWords; // number of total words of all documents
}DocMap;



/****************/
/*** DOC INFO ***/
/****************/
/*  All functions about Doc Info struct start with DI_  */

/*  Initializes a new Doc Info struct, with null text   */
/*  and words = 0. Returns NULL if error.               */
DocInfo* DI_Init();

/*  Sets the given id to the given DocInfo  */
void DI_setId(DocInfo*, int);

/*  Sets the given text to the given DocInfo  */
void DI_setText(DocInfo*, char*);

/*  Sets the given total words to the given DocInfo  */
void DI_setWords(DocInfo*, int);

/* Destroy given document Info  */
void DI_Destroy(DocInfo*);



/***************/
/*** DOC MAP ***/
/***************/
/*  All functions about Doc Map struct start with DM_  */

/*  Initializes a new Doc Map struct, with null text    */
/*  and totalWords = 0. Returns NULL if error.          */
DocMap* DM_Init(int);

/*  Update total Words to the given DocInfo.Increases   */
/*  Number of total words                               */
void DM_setTotalWords(DocMap*, int);

/*  Create document in map and assign given id  */
void DM_newDoc(DocMap*, int, char*, int);

/*  Destroy given document Map  */
void DM_Destroy(DocMap*);
