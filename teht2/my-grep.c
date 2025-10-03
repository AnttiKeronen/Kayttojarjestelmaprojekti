#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    FILE *fp; //pointteri tiedostoon
    char *buffer = NULL; // bufferin pointteri getlinee(), tää piti olla null:ikka
    size_t buffersize; // bufferikoko getlinelle()
    size_t linesize; // getline() palauttaa

    char *word; //etsittävä sana
    char *filename; // luettava tiedfosto

    if (argc == 1) {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    word = argv[1];

    if (argc == 2) { // jos ei oo annetua tiedostoa
                     // Copioltin ehdotus käyttää strstr() funktiota vertailuun
                     // https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm

        char stdinbuffer[999]; //bufferi fgetsile
        while (fgets(stdinbuffer, 999, stdin) != NULL) { //lukee stdinputin kunnes tulee nullikka vastaan
            if (strstr(stdinbuffer, word) != NULL) { // strstr kattoo löytyykö "word" koko stringistä, palauttaa NULLin jos EI löydy
                printf("%s", stdinbuffer);
            }
        }
        
    } else {
        int i;
        for (i = 2; i < argc; i++) { // käy kaiki tiedostot
            filename = argv[i];
            fp = fopen(filename, "r");

            if (fp == NULL) {
                printf("my-grep: cannot open file\n"); // jos ei saa tiedostoa auki
                exit(1);
            }
            
            while ((linesize = getline(&buffer, &buffersize, fp)) != -1) { // palauttaa -1 kun tiedosto loppuu
                if (strstr(buffer, word) != NULL) {
                    printf("%s",buffer);
                }
            }

            fclose(fp); // tiedoston sulku
        }
    }


    free(buffer); // muistin vapautus!!!!!!
    return 0;
}