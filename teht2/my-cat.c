#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

    FILE *fp; //pointteri tiedostoon
    char buffer[999]; // bufferi jolle tallennetaan rivit
    int i; // for looppiin indexi

    if (argc == 1) { // ei tiedostoja annettu, exit
        exit(0);
    }

    for(i = 1; i < argc; i++){ // käy jokasen tiedoston yksitelle
        fp = fopen(argv[i], "r");

        if (fp==NULL){ // jos tiedosto ei avaudu,
            printf("my-cat: cannot open file\n");
            exit(1);
        }

        while (fgets(buffer, 999, fp) != NULL) { // lukee rivin kerrallaa, stoppaa NULL:iin
            printf("%s", buffer);
        }

        fclose(fp);
    }

    return 0;
}