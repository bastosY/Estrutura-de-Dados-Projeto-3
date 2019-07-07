#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef struct servers{

    char nameServer[10000];
    char **hashServer;
    int quantidade;

}server;

server initServer(int maxCapacity, int numbServer){

    char name[101];

    server newServer;
    sprintf(name, "S%d", numbServer);
    strcpy(newServer.nameServer, name);

    newServer.hashServer = (char**)malloc(maxCapacity*sizeof(char*));

    for(int i = 0 ; i < maxCapacity ; i++){
        newServer.hashServer[i] = (char*)malloc(10000*sizeof(char));
        strcpy(newServer.hashServer[i], " ");
    }

    newServer.quantidade = 0;

    return newServer;

}

int checksum(char searchedWord[10000]){

    
    int count = 1;
    int valueWord = searchedWord[0];
    while(searchedWord[count] != '\0'){

        if(searchedWord[count] != ' '){

            valueWord ^= searchedWord[count];
        }

        count++;
    }
    
    return valueWord;
}


int functionH1(int valueWord){
    return( 7919 * valueWord);
}

int functionH2(int valueWord){
    return( 104729 * valueWord + 123 );
}


int main(int argc, char const *argv[])
{

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    int numbServers = 0, maxCapacity = 0, numbSearch = 0, numbWords = 0, valueWord = 0, position = 0, positionCollider = 0;
    char word[10000];

    fscanf(input, "%d %d", &numbServers, &maxCapacity);

    server *arrayServers = (server*)malloc(numbServers*sizeof(server));

    for(int i = 0; i < numbServers; i++){
        arrayServers[i] = initServer(maxCapacity, i);
    }

    fscanf(input, "%d", &numbSearch);
        int testCollision = 0;

    while (fscanf(input, "%d %[^\n]s", &numbWords, word) != EOF ){

        valueWord = checksum(word);
        position = functionH1(valueWord) % numbServers;
        
        for(int i = 0 ; i < maxCapacity ; i++){
            if(!(strcmp(arrayServers[position].hashServer[i], " "))){
                strcpy(arrayServers[position].hashServer[i], word);
                arrayServers[position].quantidade++;
                fprintf(output, "[%s] %s", arrayServers[position].nameServer, arrayServers[position].hashServer[0]);
                for(int z = 1 ; z < arrayServers[position].quantidade ; z++){
                   fprintf(output,", %s", arrayServers[position].hashServer[z]);
                }
                fprintf(output,"\n");
                testCollision = 0;
                break;
            }
            else{
                testCollision = 1;
            }
        }

        if(testCollision){
            for(int i = 1 ; i < numbServers; i++){
                positionCollider = ( functionH1(valueWord) + (i*functionH2(valueWord)) ) % numbServers;
                if(arrayServers[positionCollider].quantidade < maxCapacity){
                    strcpy(arrayServers[positionCollider].hashServer[arrayServers[positionCollider].quantidade], word);
                    arrayServers[positionCollider].quantidade++;
                    fprintf(output, "%s->%s\n", arrayServers[position].nameServer, arrayServers[positionCollider].nameServer);
                    fprintf(output, "[%s] %s", arrayServers[positionCollider].nameServer, arrayServers[positionCollider].hashServer[0]);
                    for(int z = 1 ; z < arrayServers[positionCollider].quantidade ; z++){
                        fprintf(output,", %s", arrayServers[positionCollider].hashServer[z]);
                    }
                    fprintf(output,"\n");
                    break;
                }
            }
        }
    }

    for(int i = 0 ; i < numbServers ; i++){
        for(int z = 0 ; z < maxCapacity ; z++){
            free(arrayServers[i].hashServer[z]);
        }
        free(arrayServers[i].hashServer);
    }
    
    fclose(input);
    fclose(output);

    return 0;


}
