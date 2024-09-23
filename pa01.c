#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*============================================================================
| Assignment: pa01 - Encrypting a plaintext file using the Hill cipher
|
| Author: Daniel A. Gomez
| Language: c
|
| To Compile: gcc -o pa01 pa01.c
|
| To Execute:  c -> ./pa01 kX.txt pX.txt
| where kX.txt is the keytext file
| and pX.txt is plaintext file
| Note:
| All input files are simple 8 bit ASCII input
| All execute commands above have been tested on Eustis
|
| Class: CIS3360 - Security in Computing - Fall 2023
| Instructor: McAlpin
| Due Date: 2/25/2024
+===========================================================================*/

void textFiltering(char *plainTxt, int len);
void encription(char *unencripted, int **cipherKey, int len);

int main(int argc, char **argv){
    // Opening the key and plain text files.
    FILE *keyFile, *txtFile;

    keyFile = fopen(argv[1], "r");
    txtFile = fopen(argv[2], "r");

    // Preparing strings to receive the contents of the files.
    char txtStr[10000];
    char txtLine[10000];
    txtStr[0] = '\0';

    char keyStrLine[500];
    char *keyStrLines[100];

    // Storing the key file text into an array of strings. 
    int numLines = 0;
    while(fgets(keyStrLine, 500, keyFile) != NULL){    
        keyStrLines[numLines] = strdup(keyStrLine); 
        numLines++;
    }

    // Storing the plain text into a string.
    while(fgets(txtLine, sizeof(txtLine), txtFile)){
        strcat(txtStr, txtLine);
    }

    // Find out the size of the matrix.
    int keySize = atoi(keyStrLines[0]);

    // Filtering the plaintext string so that it can be encrypted.
    textFiltering(txtStr, keySize);

    // Creating a matrix to recieve the integers from the key file.
    int **cipherMrx = malloc(keySize * sizeof(int*)); 
    for(int i = 0; i < keySize; i++){
        cipherMrx[i] = malloc(keySize * sizeof(int));
    }

    // Translating the keyStr into a usable matrix and matrix size for the 
    // sake of encryption.
    for(int i = 0; i < keySize; i++){
        char *token;
        token = strtok(keyStrLines[i + 1], "\t");

        for(int j = 0; j < keySize; j++){
            cipherMrx[i][j] = atoi(token); 

            token = strtok(NULL, "\t");
        }
    }

    // Prints out the key matrix.
    printf("Key matrix:\n");
    for(int i = 0; i < keySize; i++){
        printf("\t");

        for(int j = 0; j < keySize; j++)
            printf("%d\t", cipherMrx[i][j]);

        printf("\n");
    }    

    // Prints out plaintext after filtering it.
    printf("\nPlaintext:\n");
    int counter = 0;
    while(txtStr[counter] != '\0'){
        if(counter % 80 == 0 && counter != 0) 
            printf("\n");
    

        printf("%c", txtStr[counter]);
        counter++;
    }
    printf("\n\n");     

    // Encript. 
    encription(txtStr, cipherMrx, keySize);


    // Print out encripted text.
    printf("Ciphertext:\n");
    counter = 0;
    while(txtStr[counter] != '\0'){
        if(counter % 80 == 0 && counter != 0)
            printf("\n");

        printf("%c", txtStr[counter]);
        counter++;
    }

    for(int i = 0; i < numLines; i++)
        free(keyStrLines[i]);

    for(int i = 0; i < keySize; i++)
        free(cipherMrx[i]);

    free(cipherMrx);

    // Closing opened files. 
    fclose(keyFile);
    fclose(txtFile);    
}

// Takes in the plaintext string and modifies it to remove spaces, non-alphabet letters,
// and lowercase all uppercase letters. 
void textFiltering(char *plainTxt, int len){

    // filteredTxt will temporarily hold the modified plainTxt string as it is being 
    // filtered.
    char filteredTxt[10000];

    // i will keep track of the current index of the plainTxt to evaluate whether it
    // can be copied into the filteredTxt and whether it needs to be lowercased. tracker
    // keeps track of the current index of filteredTxt.
    int i = 0;
    int tracker = 0;

    // Until the plainTxt at index i is null, if the current index contains needed characters
    // it is put in the filteredTxt. Otherwise, i is incremented until null is hit.
    while(plainTxt[i] != '\0'){

        if(isalpha(plainTxt[i])){
            if(isupper(plainTxt[i]))
                filteredTxt[tracker] = tolower(plainTxt[i]);
            else
                filteredTxt[tracker] = plainTxt[i];
            
            i++;
            tracker++;
            continue; 
        }

        i++;
    }

    // The last index is made null and plainTxt copies filteredTxt's string.
    filteredTxt[tracker] = '\0';
    
    if(tracker % len != 0){
        for(int i = 0; i < len; i++){
            filteredTxt[tracker] = 'x'; 
            tracker++;

            if(tracker % len != 0){
                continue;
            }
            else{
                break;
            }
        }
    }

    filteredTxt[tracker] = '\0';
    strcpy(plainTxt, filteredTxt);
}

// Encripts the filtered text string using the cipher key.
void encription(char *unencripted, int **cipherKey, int len){
    int encriptMrx[len];
    char *encriptStr = calloc(10000, sizeof(char)); 
    int tracker = 0;

    while(unencripted[tracker] != '\0'){

        // Initiates the encriptMrx so that it's ready to be encripted
        // and stored in the encriptStr.
        for(int i = 0; i < len; i++){
            encriptMrx[i] = unencripted[tracker];
            encriptMrx[i] -= 'a';
            tracker++;
        }
        tracker -= len;


        for(int i = 0; i < len; i++){

            int holder = 0;
            for(int j = 0; j < len; j++){
                holder += cipherKey[i][j] * encriptMrx[j];
            }

            holder %= 26;
            encriptStr[tracker] = holder; 

            encriptStr[tracker] += 'a';
            tracker++; 
        }
    }

    // Copies the encripted string into the unencripted string. 
    strcpy(unencripted, encriptStr);

    // Free the dynamically allocated memory.
    free(encriptStr);
}


/*=============================================================================
| I Daniel A. Gomez (5351345) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/
