#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


int main(int argc, char *argv[]) {  // alustus kaikelle mitä koodissa tarvitaan
    FILE *syote, *tulos;  //pointterit tiedostoihin
    char *rivit[10000];   // kiinteä taulukko riveille
    int maara = 0;   // kuinka monta riviä
    char *rivi = NULL;  // rivien lukua varten
    size_t pituus = 0;  // kuinka paljon muistia riveille
    ssize_t luettu;  // paljonko merkkejä luettu

    // jos liikaa argumentteja
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // avaa tiedoston
    if (argc >= 2) {
        syote = fopen(argv[1], "r");
        if (syote == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    } else {
        syote = stdin;
    }

    // jos käyttäjä antaa täydet käskyt
    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) { // onko tiedostot samat?
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        tulos = fopen(argv[2], "w"); // avataan tulostiedosto
        if (tulos == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    } else {
        tulos = stdout; // tuloste ruudulle jos ei täydet käskyt.
    }

    // lukee rivit taulukkoon
    while ((luettu = getline(&rivi, &pituus, syote)) != -1) {
        rivit[maara] = strdup(rivi);
        if (rivit[maara] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        maara++;
    }
    free(rivi);
    fclose(syote);

    // tulosta rivit käänteisessä järjestyksessä
    for (int i = maara - 1; i >= 0; i--) {
        fprintf(tulos, "%s", rivit[i]);
        free(rivit[i]);
    }

    if (tulos != stdout) fclose(tulos);

    return 0;
}
