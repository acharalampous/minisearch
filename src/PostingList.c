/*******************************/
/*PostingList.c */

/* Name:    Andreas Charalampous
 * A.M :    1115201500195
 * e-mail:  sdi1500195@di.uoa.gr
 */
/********************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "aclib.h"
#include "PostingList.h"

/*  Implementation of all functions of Posting List
 *  (PL_) and its nodes (PLN_). Definitions of them
 *  found in PostingList.h
 */


///////////////////
//** LIST NODE **//
///////////////////
PLNode* PLN_Init(int id){
    PLNode* newNode = malloc(sizeof(PLNode));

    if(newNode == NULL) // error while allocating space
        return NULL;

    // Set newNode data //
    newNode->id = id;
    newNode->times = 1;
    newNode->next = NULL;

    return newNode;
}


void PLN_Print(PLNode* node){
    printf("id: %d | times: %d\n", node->id, node->times);
}


void PLN_increaseTimes(PLNode* node){
    node->times++;
}



//////////////////////
//** POSTING LIST **//
//////////////////////
PostingList* PL_Init(){
    PostingList* newList = malloc(sizeof(PostingList));
    if(newList == NULL)
        return NULL; // error during allocation

    newList->start = NULL;
    newList->numOfPosts = 0;

    return newList;
}


int PL_isEmpty(PostingList* list){
    return (list->start == NULL) ? 1 : 0;
}

int PL_getNumOfPosts(PostingList* ps){
    if(ps == NULL)
        return -1;
    else
        return ps->numOfPosts;
}


void PL_Push(PostingList* list, PLNode* newNode){
    if(PL_isEmpty(list)) // empty list
        newNode->next = NULL;
    else // not empty
        newNode->next = list->start; // point to current first

    list->start = newNode; // insert at start

    // increase number of nodes in list //
    list->numOfPosts++;
}


PLNode* PL_FindNode(PostingList* list, int id){
    PLNode* temp; // guide through nodes of list
    temp = list->start;
    while(temp != NULL){ // scan whole list
        int current = temp->id;
        if(current == id){ // post found
            return temp;
        }
        else if(current < id){ // list is sorted in descended order
            break; // so the id is not in list
        }
        else{ // move to next post
            temp = temp->next;
        }
    }
    return NULL; // 404

}

PLNode* PL_checkStart(PostingList* list, int id){
    PLNode* temp = list->start;
    if(temp->id == id){
        return temp;
    }
    else
        return NULL;
}


int PL_Pop(PostingList* list){
    PLNode* temp = list->start;
    if(temp == NULL) // empty list
        return -1;

    list->start = temp->next; // new list's first node
    free(temp);

    // Decrease number of posts //
    list->numOfPosts--;
    return 1; // success
}


void PL_getScores(PostingList* pl, heapDoc** hDocs, DocMap* dMap){
    PLNode* temp = pl->start;

    int numOfPosts = pl->numOfPosts; // number of documents which contain word
    int numOfDocs = dMap->numOfDocs; // total documents
    int totalWords = dMap->totalWords; // total words of all documents

    float k1 = 1.2;
    float b = 0.75;
    int avgWords = totalWords / numOfDocs;

    /* Guide through list and update the score of each document */
    while(temp != NULL){
        float score = 0.0;

        int id = temp->id;
        int times = temp->times;
        int wordsInDoc = dMap->Docs[id]->numOfWords;

        float IDF1 = numOfDocs - numOfPosts + 0.5;
        float IDF2 = numOfPosts + 0.5;
        float IDF = (float)(log10((float)(IDF1 / IDF2)));

        float score1 = IDF * times * (k1 + 1);
        float score2 = times + k1 * (1 - b + (b * (float)(wordsInDoc / avgWords)));
        score = (float)(score1 / score2);

        if(hDocs[id] == NULL){ // first time the document is found
            hDocs[id] = malloc(sizeof(heapDoc));
            hDocs[id]->id = id;
            hDocs[id]->score = 0.0;
        }
        hDocs[id]->score += score; // update score

        temp = temp->next;
    }
}


void PL_Destroy(PostingList* list){
    PLNode* temp = list->start;
    while(temp != NULL){ // until every node is deleted
        temp = temp->next; // point to next to delete current
        PL_Pop(list); // delete first node
    }

    free(list);
}


void PL_Print(PostingList* list){
    PLNode* node = list->start;
    int nodeNum = 1;
    printf("Printing list: [number of posts: %d]\n", list->numOfPosts);
    while(node != NULL){
        printf("      +Printing post [%d]: ", nodeNum);
        PLN_Print(node);
        node = node->next;
        nodeNum++;
    }
}
