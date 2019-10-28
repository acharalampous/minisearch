#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "PostingList.h"
#include "Trie.h"
#include "DocMap.h"
#include "aclib.h"

int main(int argc, char* argv[]){
    char* textFile; // text document to open and parse
    int K; // number of results of each query

    if(argc == 3){ // ./minisearch -i DocName
        if(!strcmp(argv[1], "-i")){
            textFile = argv[2];
            K = 10;
        }
        else{ // invalid parameter given
            exeParameters();
            printf("Abort.\n");
            return -1;
        }
    }
    else if(argc >= 5){ // ./minisearch -i DocName -k K
        if((!strcmp(argv[1], "-i") && (!strcmp(argv[3], "-k")))){
            if(!isNumber(argv[4])){
                exeParameters();
                printf("*error* Parameter *argv[4]* after -k was not a number.Abort\n");
                return -1;
            }
            textFile = argv[2];
            K = atoi(argv[4]);
        }
        else if((!strcmp(argv[1], "-k") && (!strcmp(argv[3], "-i")))){
            if(!isNumber(argv[2])){
                exeParameters();
                printf("*error* Parameter *argv[2]* after -k was not a number. Abort\n");
                return -1;
            }
            textFile = argv[4];
            K = atoi(argv[2]);
        }
        else{ // invalid parameter given
            exeParameters();
            printf("Abort.\n");
            return -1;
        }
    }
    else{
        exeParameters();
        printf("Abort.\n");
        return -1;
    }


	Trie* tr = Trie_Init(); // Initialize the trie structure
	FILE* fp = fopen(textFile, "r"); // Open file with documents
	if(fp == NULL) {
        printf("[ERR] Error while opening file. File might not exist. Abort.\n");
        Trie_Destroy(tr);
        return -2;
	}

    int numOfLines = getNumOfLines(fp); // get number of file lines
    if(numOfLines == -1){
        printf("[ERR] Invalid format of given document. Abort.\n");
        Trie_Destroy(tr);
        fclose(fp);
        return -3;
    }

    DocMap* dMap = DM_Init(numOfLines + 1); // create empty map
    if(dMap == NULL) {
        printf("[ERR] Error while allocating space for map. Abort\n");
        Trie_Destroy(tr);
        fclose(fp);
        return -4;
	}

	size_t buffsize = 256; // will be used for getline
	char* line = malloc(sizeof(char) * buffsize); // for getline
	char* id; // store id of each line
	int index = 0; // to check validity of each document's id

    /* Create the inverted index */
	while(!feof(fp)){
		getline(&line, &buffsize, fp); // read line from file
        if(feof(fp))
            break;

        char* line1 = discardSpaces(line); // clear all tabs and spaces in front of id
        if(!strcmp(line1, "\n")){ // line had only tabs, spaces and \n
            continue;
        }

		id = getID(line1); // get the id of line
		if(id == NULL)
            continue;

        if(!isNumber(id)){ // invalid id..error
            printf("[ERR] Invalid format of document given.");
            printf(" Id of document #%d was not a number. Abort\n",index);

            DM_Destroy(dMap);
            Trie_Destroy(tr); // destroy trie

            free(line);
            free(id);

            fclose(fp); // close file
            return -5;
        }
        else{
            if(index != atoi(id)){ // not continuous id
                printf("[ERR] Invalid format of textfile given.");
                printf(" Not continuous id between documents. Abort\n");

                DM_Destroy(dMap);
                Trie_Destroy(tr);

                free(line);
                free(id);

                fclose(fp);
                return -6;

            }

            char* text = line1 + strlen(id); // point after id
            text = discardSpaces(text); // discard spaces after id, so we point at start of text
            text = strtok(text, "\n");
            int textSize = strlen(text);

            char* newDoc = myStrCopy(text, textSize); // keep the text for document

            char* token = myStrCopy(newDoc, strlen(newDoc));
            char* toFree = token; // will be used to free token space
            token = strtok(token, " \t\n");

            int totalWords = 0;
            while(token != NULL) {
                Trie_Insert(tr, token, atoi(id)); // insert word in trie
                totalWords++;
                token = strtok(NULL, " \t\n");
            }

            /* Map new document with its id */
            DM_newDoc(dMap, atoi(id), newDoc, totalWords);
            free(id);
            free(toFree);
            index++;
        }
	}

    int counter = dMap->numOfDocs;
    int spaces = 0;
    while(counter >= 10){
        spaces++;
        counter /= 10;
    }
    printf("\n*********************************************************");
    printf("%.*s\n",spaces + 1, "*******");
	printf("* Inverted Index was succesfully created for %d documents *\n",dMap->numOfDocs);
	printf("*********************************************************");
    printf("%.*s\n",spaces + 1, "*******");
    while(1){
        int exitSGN = 0;
        //char* command = malloc(sizeof(char) * 100);
        int option;

        printOptions();


        printf("minisearch>");
        fflush(stdout);
        getline(&line, &buffsize, stdin);
        line = strtok(line, "\n");

        while(line == NULL){
            printf("minisearch>");
            fflush(stdout);
            getline(&line, &buffsize, stdin);
            line = strtok(line, "\n");
        }


        if(line[0] != '/'){
            printf("Invalid option. Command must begin with \"/\"\n");
            continue;
        }

        if (!(strncmp(line, "/search ", 8))) option = 1;

		else if (!(strncmp(line, "/df", 3))) option = 2;

		else if (!(strncmp(line, "/tf ", 4))) option = 3;

		else if (!(strncmp(line, "/exit", 5))) option = 4;

		else{ printf("Invalid option given.\n"); continue;} // read command again


        switch(option){
            case 1:{
                char* keyWords = line + 8; // overpass /search and point to parameters
                discardSpaces(keyWords);
                if(keyWords == NULL){
                    printf("Invalid option given.\n");
                    break;
                }
                Trie_searchKeyWords(tr, dMap, keyWords, K);
                break;
            } // end case 1
            case 2:{
                if(strlen(line) == 3) // /df was given
                    Trie_allWords(tr);
                else{
                    char* word = line + 4;
                    discardSpaces(word); // find the word after /df
                    if(word == NULL){
                        Trie_allWords(tr);
                    }
                    else{
                        word = strtok(word, " \t\n");
                        Trie_printWord(tr, word); // print document frequency of word
                    }
                }
                break;
            } // end case 2
            case 3:{
                char* word = line + 4; // overpass /tf
                discardSpaces(word);
                if(word == NULL || word[0] == '\0'){
                    printf("Invalid option given.Abort\n");
                }
                else{
                    word = strtok(word, " \t\n"); // get ID
                    if(!isNumber(word)){
                        printf("Invalid id given.Abort\n");
                        continue;
                    }
                    else{
                        int id = atoi(word);
                        word = strtok(NULL, " \t\n"); // get word to search for
                        if(word == NULL){
                            printf("Invalid option given.Abort\n");
                            continue;
                        }
                        else
                            Trie_findWordInDoc(tr, id, word);
                    }
                }
                break;
            } // end case 3
            case 4:{
                exitSGN = 1; // exit signal
                break;
            }
        }// end switch:
        if(exitSGN == 1)
            break;
	}


    DM_Destroy(dMap);
    Trie_Destroy(tr); // 2012

	free(line);
	fclose(fp);

	return 0; // SUCCESS
}
