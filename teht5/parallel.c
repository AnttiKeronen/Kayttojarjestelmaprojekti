#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/sysinfo.h>

typedef struct {
    size_t laskuri;
    char c;
} RLEPair;

typedef struct {
    char *data;
    size_t alku;
    size_t loppu;
    RLEPair *tulos;
    size_t tuloskoko;
    size_t tuloskapa;
} ThreadArgs;


void* compressori(void* arg) {
    ThreadArgs* t = (ThreadArgs*)arg;


    t->tuloskapa = 1024;
    t->tulos = malloc(sizeof(RLEPair) * t->tuloskapa);
    if (!t->tulos) { perror("malloc error"); exit(1); }

    size_t indeksi = 0;
    size_t laskuri = 1;

    for (size_t i = t->alku + 1; i < t->loppu; i++) {
        if (t->data[i] == t->data[i-1]) {
            laskuri++;
        } else {
            if (indeksi >= t->tuloskapa) {
                t->tuloskapa *= 2;
                t->tulos = realloc(t->tulos, sizeof(RLEPair) * t->tuloskapa);
                if (!t->tulos) { perror("realloc errori"); exit(1); }
            }
            t->tulos[indeksi++] = (RLEPair){laskuri, t->data[i-1]};
            laskuri = 1;
        }
    }

    if (indeksi >= t->tuloskapa) {
        t->tuloskapa += 1;
        t->tulos = realloc(t->tulos, sizeof(RLEPair) * t->tuloskapa);
        if (!t->tulos) { perror("realloc"); exit(1); }
    }
    t->tulos[indeksi++] = (RLEPair){laskuri, t->data[t->loppu-1]};
    t->tuloskoko = indeksi;

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file...\n", argv[0]);
        return 1;
    }

    int maara = get_nprocs();
    pthread_t *threads = malloc(sizeof(pthread_t) * maara);
    ThreadArgs *args = malloc(sizeof(ThreadArgs) * maara);

    for (int f = 1; f < argc; f++) {
        int fd = open(argv[f], O_RDONLY);
        if (fd < 0) { perror("open"); continue; }

        struct stat st;
        if (fstat(fd, &st) < 0) { perror("fstat"); close(fd); continue; }
        size_t tiedostonkoko = st.st_size;
        if (tiedostonkoko == 0) { close(fd); continue; }

        char *data = mmap(NULL, tiedostonkoko, PROT_READ, MAP_PRIVATE, fd, 0);
        if (data == MAP_FAILED) { perror("map errori"); close(fd); continue; }

        size_t lohkokoko = (tiedostonkoko + maara - 1)/maara;

        for (int i = 0; i < maara; i++) {
            args[i].data = data;
            args[i].alku = i * lohkokoko;
            args[i].loppu = (i == maara-1) ? tiedostonkoko : (i+1) * lohkokoko;
            pthread_create(&threads[i], NULL, compressori, &args[i]);
        }

        for (int i = 0; i < maara; i++) pthread_join(threads[i], NULL);

        // merge results across threads
        RLEPair edellinen = args[0].tulos[0];
        for (int t = 0; t < maara; t++) {
            for (size_t i = (t==0)?1:0; i < args[t].tuloskoko; i++) {
                RLEPair nykynen = args[t].tulos[i];
                if (nykynen.c == edellinen.c) edellinen.laskuri += nykynen.laskuri;
                else {
                    fwrite(&edellinen.laskuri, sizeof(size_t), 1, stdout);
                    fwrite(&edellinen.c, sizeof(char), 1, stdout);
                    edellinen = nykynen;
                }
            }
        }

        fwrite(&edellinen.laskuri, sizeof(size_t), 1, stdout);
        fwrite(&edellinen.c, sizeof(char), 1, stdout);

        for (int i = 0; i < maara; i++) free(args[i].tulos);
        munmap(data, tiedostonkoko);
        close(fd);
    }

    free(threads);
    free(args);
    return 0;
}
