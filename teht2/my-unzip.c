#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *fp;
    int amount; // määrä joka luetaan
    int chara;  // merkki joka luetaan

    if (argc < 2) {
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    int i;
    for(i = 1; i < argc; i++){
        fp = fopen(argv[i], "rb"); // tähän pitikin laittaa rb, read-BINARY.....

        if (fp==NULL) {
            printf("my-unzip: cannot open file\n");
            exit(1);
        }

        while (fread(&amount, 4, 1, fp) == 1){ // lukee 4 byten kokosen intin
            fread(&chara, 1, 1, fp);    // lukee 1 byten kokosen merkin

            int j;
            for (j=0; j < amount; j++) { // tulostaa merkin j-kertaa, eli amountin verran
                printf("%c",chara);
            }
        }

        fclose(fp);

    }

    printf("\n");
    return 0;
}