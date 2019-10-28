/*******************************/
/* acutils.c */

/* Name:    Andreas Charalampous
 * A.M :    1115201500195
 * e-mail:  sdi1500195@di.uoa.gr
 */
/********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h> // for ioctl()
#include <sys/ioctl.h> // ioctl()

#include "aclib.h"

/*  All functions implementions that are defined in aclib.h */


void S_HeapSort(heapDoc** hDoc, int lastCell){

    int left; // will keep index of left child of each node
    int right; // right child
    int maxChild; // will keep the index of the biggest score in comparisons
    int parent = ((lastCell + 1) / 2) - 1; // starting point of heapsort

    for(int i = parent; i >= 0; i--){ // from middle of table until start
        left = (i * 2) + 1; // calculate left child index
        right = (i * 2) + 2; // right child index

        if(right <= lastCell){ // has right child
            maxChild = S_Max(hDoc, left, right);
        }
        else // does not have right child, so we compare with left child
            maxChild = left;

        int newParent = S_Max(hDoc, i, maxChild); // compare parent and largest child
        if(newParent != i){ // parent was swapped
            S_Swap(hDoc, i, newParent);

            /* At this point, the parent must be moved as down as it can go */
            /* in order to accomplish heap sort                             */
            int temp = newParent; // point to the new index of parent

            while(1){ // Same as above, each time we update parent's index
                int left = (temp * 2) + 1; // left child of temp
                int right = (temp * 2) + 2; // right child of temp

                if(left > lastCell)// no children
                    break;
                else{ // has at least left child
                    if(right <= lastCell){ // has right child
                        maxChild = S_Max(hDoc, left, right);
                    }
                    else
                        maxChild = left;
                }

                int newMax = S_Max(hDoc, temp, maxChild);
                if(newMax != temp){
                    S_Swap(hDoc, temp, newMax);
                    temp = newMax;
                }
                else // fix = 1;
                    break;
            }
        }
    }
}


int S_Max(heapDoc** hDoc, int index1, int index2){
    if(hDoc[index1] == NULL)
        return index2; // index2 cant be smaller
    else if(hDoc[index2] == NULL)
        return index1; // index1 cant be smaller
    else{ // return the max
        return (hDoc[index1]->score > hDoc[index2]->score) ? index1 : index2;
    }
}

void S_Swap(heapDoc** hDoc, int el1, int el2){
    heapDoc* temp = hDoc[el1];
    hDoc[el1] = hDoc[el2];
    hDoc[el2] = temp;
}

heapDoc* S_GetMax(heapDoc** hDoc, int* HeapLast){
    if(*HeapLast == -1) // the table is "empty"
        return NULL;

    int i = 0;
    int left;
    int right;
    int maxChild;

    heapDoc* maxDoc = hDoc[i]; // get document with maximum score, it is at top

    S_Swap(hDoc, i, *HeapLast); // swap biggest score with the last cell, to "hide" it
    (*HeapLast)--;

    /* Like in heapsort, we keep pushing the first element, which was placed last before */
    /* as down as it can go                                                              */
    while(1){
        left = (i * 2) + 1; // left child
        right = (i * 2) + 2; // right child

        if(left > *HeapLast)// parent is on the bottom, so heap is fixed
            break;
        else{ // has at least left child
            if(right <= *HeapLast){ // has right child
                maxChild = S_Max(hDoc, left, right);
            }
            else
                maxChild = left;
        }

        int newParent = S_Max(hDoc, i, maxChild);
        if(newParent != i){
            S_Swap(hDoc, i, newParent);
            i = newParent;
        }
        else // fix = 1; // parent is bigger than both of children, so heap is fixed
            break;
    }

    return maxDoc;
}

void S_PrintResult(heapDoc* maxDoc, DocMap* dMap, char** keys, int numOfKeys, int counter){
    /* Will create a big string with all the necessary info */
    char istr[5]; // string for counter
    char idstr[12]; // string for id
    char sstr[30]; // string for score
    char* par = ". () [] ";
    char* text = dMap->Docs[maxDoc->id]->text;

    /* Convert everything to string */
    sprintf(istr, "%d", counter);
    sprintf(idstr, "%d", maxDoc->id);
    sprintf(sstr, "%f", maxDoc->score);

    /* Calculate the space that is needed for the new big string */
    long unsigned int infoLength = strlen(istr) + strlen(idstr) + strlen(sstr) + strlen(par);
    long unsigned int newLength = infoLength + strlen(text);


    char* fullResult = malloc(sizeof(char) * newLength + 1);
    char* underline = malloc(sizeof(char) * newLength + 1); // will be used to underline keyWords

    fullResult[newLength] = '\0';

    /* at start the underline string has only spaces */
    for(int un = 0; un < newLength;un++)
        underline[un] = ' ';
    underline[newLength] = '\0';

    /* Create the big string */
    strcpy(fullResult, istr);
    strcat(fullResult, ". (");
    strcat(fullResult, idstr);
    strcat(fullResult, ") [");
    strcat(fullResult, sstr);
    strcat(fullResult, "] ");
    strcat(fullResult, text);

    /* For every tab in the final string, we put a tab in the respective cell of */
    /* the underline string, in order to keep alignment between the text and ^   */
    char* start = fullResult;
    char* fTab = strstr(start, "\t");
    while(fTab != NULL){ // until no other tab is found
        int toTab = fTab - start; // offset of tab
        underline[toTab] = '\t';
        start = fTab + 1;
        fTab = strstr(start,"\t");
    }


    /*  At this point we search for all keywords in the final text, to create the   */
    /*  underline string                                                            */
    for(int i = 0; i < numOfKeys; i++){
        char* start = fullResult + infoLength; // point at start of text
        char* toFind = malloc(sizeof(char) * strlen(keys[i]) + 3);
        strcpy(toFind, " ");
        strcat(toFind, keys[i]);
        strcat(toFind, " ");
        toFind[strlen(keys[i]) + 2] = '\0';

        /* Search for keyword */
        char* word = strstr(start, toFind); // got start of keyword
        while(word != NULL){
            int offset = word - (fullResult + infoLength) + 1; // find offset

            int endword = infoLength + offset + strlen(keys[i]); // find end of keyword
            for(int off = infoLength + offset; off < endword; off++)
                underline[off] = '^'; // put respective ^s in underline string

            start = word + strlen(keys[i]) + 1; // bypass word
            word = strstr(start, toFind); // look for next keyword
        }
        free(toFind);
    }

    struct winsize ws; // for terminal width

    ioctl(0, TIOCGWINSZ, &ws);
    int width = ws.ws_col;

    char* resTemp = fullResult; // delete fullResult later
    char* undTemp = underline; // to delete underline later
    char* spaces = underline;
    if(width < newLength){ // cant print string in one line
        int charsLeft = newLength; // chars left until end of text
        while(charsLeft > 0){
        /* If we print the first line we must start from far left, as examples */
            if(charsLeft == newLength){
                printf("%.*s\n", width, fullResult); // print as much space there is in terminal

                /* Create a copy of underline, until the specific char as width */
                /* and if there is ^ in it, we must print it                    */
                char* und = malloc(sizeof(char) * width + 1);
                und[width] = '\0';
                strncpy(und, underline, width);
                //memcpy(und, underline, width);
                char* chk = strstr(und,"^");

                if(chk != NULL) // ^ was found
                    printf("%.*s\n", width, underline);

                /* Overpass all written characters */
                underline += width;
                fullResult += width;
                charsLeft -= width;
                free(und);
            }
            else{ // we must align the text
                printf("%.*s",(int)infoLength,spaces); // print spaces to align
                printf("%.*s\n", width - (int)infoLength, fullResult);

                char* und = malloc(sizeof(char) * width - infoLength + 1);
                und[width-infoLength] = '\0';
                strncpy(und, underline, width - infoLength);
                //memcpy(und, underline, width - infoLength);
                char* chk = strstr(und,"^");

                if(chk != NULL){
                    printf("%.*s",(int)infoLength,spaces);
                    printf("%.*s\n", width - (int)infoLength, underline);

                }

                underline += width - infoLength;
                fullResult += width - infoLength;
                charsLeft -= width - infoLength;


                free(und);
            }
        }
        free(undTemp);
        free(resTemp);
    }
    putchar('\n');
    putchar('\n');

}


int getNumOfLines(FILE* fp){
    fseek(fp, 0, SEEK_END); // point at end of text file
    fseek(fp, -1, SEEK_CUR); // point before EOF

    int lastInt = 0; // checks if character before \n is int, so no error
    int BOF = 0; // 0: not at begin of file, -1 at begin of file
    char ch; // current read character, moving backwards
    int flag = 0; // 0: didnt find character which no space
    int numOfLines = 0;
    /* Moving from the end, we search for the first \n, after we find a character */
    while(BOF == 0){ // until we reach the start.. fseek will return -1
        ch = fgetc(fp);

        if(flag == 0){ // havent found character which is not space
            if(ch != '\n' && ch != '\t' && ch != ' '){
                flag = 1;
                if(ch >= 48 && ch <= 57){ // last character read was number
                    lastInt = 1;
                }
            }
        }
        else{
            if(ch == '\n'){
                if(lastInt != 1){
                    printf("[ERR] Invalid file format. Abort\n");
                    return -1;
                }
                fscanf(fp, "%d", &numOfLines); // get number of last line
                fseek(fp, 0, SEEK_SET);
                return numOfLines;
            }
            else{
                if(ch >= 48 && ch <= 57) // last character read was number
                    lastInt = 1;
                else
                    lastInt = 0;
            }

        }
        BOF = fseek(fp, -2, SEEK_CUR); // if we reach start, fseek will return -1
    }

    printf("[ERR] Invalid file format. Abort\n");
    fseek(fp, 0, SEEK_SET);
    return -1;
}


char* discardSpaces(char* src){
    char* delim = src;
    while(*delim != '\n'){
        if(*delim == '\t' || *delim == ' ' )
            delim++;
        else
            break;
    }

    return delim;
}

char* getID(char* src){
    char* delim = src;
    int length = 0; // size of first word which is the id
    while(*delim != '\t' && *delim != ' ' && *delim != '\n'){
        length++;
        delim++;
    }
    if(length == 0)
        return NULL;

    char* dest = malloc(sizeof(char) * length + 1);
    dest[length] = '\0';
    delim = src;
    int index = 0;
    while(*delim != '\t' && *delim != ' ' && *delim != '\n' ){
        dest[index++] = *delim;
        delim++;
    }

    return dest;

}

int isNumber(char* str){
    char* temp = str;
    char ch = *temp;
    while(ch != '\0'){
        if(ch >= 48 && ch <= 57){
            ch = *(++temp);
        }
        else{
            return 0;
        }
    }
    return 1;
}

char* myStrCopy(char* src, int srclength){
    char* dest = NULL;
    dest = malloc(sizeof(char) * srclength + 1);
    dest[srclength] = '\0';

    memcpy(dest, src, srclength);
    return dest;

}

void exeParameters(){
    printf("[ERR] Invalid Parameters given. Execute again with the following options:\n");
    printf("\t./minisearch -i docName -k K\n");
    printf("\t./minisearch -i docName\n");
    printf("\t./minisearch -k K -i docName\n");
    printf("-docName: Name of text file containing documents\n");
    printf("-K: Number of results of each query\n");
}

void printOptions(){
    putchar('\n');
    putchar('\n');
    printf("Choose one of the following options:\n");
    printf("-------------------------------------\n");
    printf("(1) /search q1 q2 ... q10");
    printf(" [Make a search for the most relative documents]\n");
    printf("(2) /df q");
    printf(" [Print document frequency vector of all words or of given]\n");
    printf("(3) /tf id q");
    printf(" [Print term frequency of given word in given document\n");
    printf("(4) /exit");
    printf(" [Exit the application]\n");
}

