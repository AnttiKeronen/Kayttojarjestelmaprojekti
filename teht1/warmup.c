#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


int main(int argc, char *argv[]) {  // alustus kaikelle mitä koodissa tarvitaan
    FILE *syote, *tulos;  //pointterit tiedostoihin
    char **rivit = NULL;  // dynaaminen taulukko riveille
    int maara = 0;   // kuinka monta riviä
     int kapasiteetti = 0; //taulukon koko
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

    // Lue rivit muistiin dynaamisesti
    while ((luettu = getline(&rivi, &pituus, syote)) != -1) {
        if (luettu > 0 && (rivi[luettu-1] == '\n' || rivi[luettu-1] == '\r')) {
            rivi[luettu-1] = '\0';
            luettu--;
        }

        if (maara >= kapasiteetti) { // kasvata taulukkoa tarvittaessa
            kapasiteetti = kapasiteetti ? kapasiteetti * 2 : 16;
            char **tmp = realloc(rivit, kapasiteetti * sizeof(char *));
            if (!tmp) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
            rivit = tmp;
        }
        rivit[maara] = strdup(rivi); // kopioi rivi omaan muistilohkoon
        if (!rivit[maara]) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        maara++;
    }

    free(rivi);
    fclose(syote);

    // tulosta rivit käänteisessä järjestyksessä
    for (int i = maara - 1; i >= 0; i--) {
        fprintf(tulos, "%s\n", rivit[i]);
        free(rivit[i]);
    }

    free(rivit);

    if (tulos != stdout) fclose(tulos);

    return 0;
}
