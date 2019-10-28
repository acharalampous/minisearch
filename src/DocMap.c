/*******************************/
/* DocMap.c */

/* Name:    Andreas Charalampous
 * A.M :    1115201500195
 * e-mail:  sdi1500195@di.uoa.gr
 */
/********************************/
#include <stdio.h>
#include <stdlib.h>

#include "DocMap.h"

/*  Implementation of all functions of DocMap(DM)
 *  and DocInfo(DI). Definitions of them found in DocMap.h
 */

//////////////////
//** DOC INFO **//
//////////////////
DocInfo* DI_Init(){
    DocInfo* newDoc = malloc(sizeof(DocInfo));

    if(newDoc == NULL) // error while allocating space
        return NULL;

    // Set newDoc data //
    newDoc->id = -1;
    newDoc->text = NULL;
    newDoc->numOfWords = 0;

    return newDoc;
}


void DI_setId(DocInfo* Doc, int id){
    Doc->id = id;
}


void DI_setText(DocInfo* Doc, char* text){
    Doc->text = text;
}


void DI_setWords(DocInfo* Doc, int numOfWords){
    Doc->numOfWords = numOfWords;
}


void DI_Destroy(DocInfo* Doc){
    free(Doc->text);
    free(Doc);
}



/////////////////
//** DOC MAP **//
/////////////////
DocMap* DM_Init(int numOfDocs){
    int i;
    DocMap* newMap = malloc(sizeof(DocMap));

    if(newMap == NULL) // error while allocating space
        return NULL;

    /* Create space for all Documents   */
    newMap->Docs = malloc(sizeof(DocInfo*) * numOfDocs);
    if(newMap->Docs == NULL) // error while allocating space
        return NULL;

    for(i = 0; i < numOfDocs; i++){
        newMap->Docs[i] = NULL;
    }

    // Set newDoc data //
    newMap->numOfDocs = numOfDocs; // set number of documents
    newMap->totalWords = 0;

    return newMap;
}


void DM_setTotalWords(DocMap* dMap, int totalWords){
    dMap->totalWords = totalWords;
}


void DM_newDoc(DocMap* dMap, int id, char* text, int words){
    dMap->Docs[id] = DI_Init();
    DocInfo* newDoc = dMap->Docs[id];

    DI_setId(newDoc, id);
    DI_setText(newDoc, text);
    DI_setWords(newDoc, words);

    dMap->totalWords += words;
}


void DM_Destroy(DocMap* dMap){
    int num = dMap->numOfDocs;
    for(int i = 0; i < num; i++){ // destroy all documents
        if(dMap->Docs[i] != NULL){
            DI_Destroy(dMap->Docs[i]);
            dMap->Docs[i] = NULL;
        } // if null found, there arent any other docs
        else
            break;
    }
    free(dMap->Docs);

    free(dMap);

}
