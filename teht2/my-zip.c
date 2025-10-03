#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *fp;           //pointteri tiedostoo
    int char_nyt;       //tän hetkinen merkki, pitää olla int-muodossa jotta fgetc() toimii   
    int char_next;      //seuraava merkki johon vertaillaa
    int counter = 1;        //laskuri määrälle



    if (argc < 2) {
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }

    int i;
    for (i = 1;i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
                printf("my-zip: cannot open file\n"); // jos ei saa tiedostoa auki
                exit(1);
            }
        
        char_nyt = fgetc(fp); // luetaan eka merkki, fgetc() palauttaa merkkiä vastaavan numeronm
        while ((char_next = fgetc(fp)) != EOF) { // tiedoston loppuun
            if (char_next == char_nyt) {
                counter++; // jos sama niin kasvatetaan
            } else {
                fwrite(&counter, 4, 1, stdout); // kirjotetaan 4 byten numero
                fwrite(&char_nyt, 1, 1, stdout); // kirjottaa 1 byten merkin
                char_nyt = char_next;
                counter = 1;
            }
        }
        
        fwrite(&counter, 4, 1, stdout); // vika kirjotus
        fwrite(&char_nyt, 1, 1, stdout); 
        fclose(fp);
    }

    return 0;
}