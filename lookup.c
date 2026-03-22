#include <stdio.h>
#include <string.h>
#include "rainbow.h"

RainbowEntry table[TABLE_SIZE];
int table_count = 0;

int load_table(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: cannot open %s\n", filename);
        fprintf(stderr, "       Run ./table_builder first.\n");
        return 0;
    }

    char line[128];
    while (fgets(line, sizeof(line), f) && table_count < TABLE_SIZE) {
        line[strcspn(line, "\n")] = '\0';
        char *colon = strchr(line, ':');
        if (!colon) continue;
        *colon = '\0';
        strncpy(table[table_count].hash,      line,       HASH_HEX_LEN - 1);
        strncpy(table[table_count].plaintext, colon + 1,  MAX_PLAIN_LEN - 1);
        table_count++;
    }

    fclose(f);
    return table_count;
}

const char *lookup_hash(const char *hash) {
    for (int i = 0; i < table_count; i++) {
        if (strncmp(table[i].hash, hash, 32) == 0)
            return table[i].plaintext;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("=== lookup ===\n");

    if (argc < 2) {
        printf("Usage: ./lookup <md5_hash>\n");
        printf("Example: ./lookup 5f4dcc3b5aa765d61d8327deb882cf99\n");
        return 1;
    }

    const char *hash = argv[1];
    if (strlen(hash) != 32) {
        fprintf(stderr, "Error: expected a 32-character MD5 hex string\n");
        return 1;
    }

    int loaded = load_table("rainbow_table.txt");
    if (!loaded) return 1;
    printf("Table loaded: %d entries\n\n", loaded);

    printf("Hash   : %s\n", hash);
    const char *result = lookup_hash(hash);
    if (result) {
        printf("Result : CRACKED\n");
        printf("Plain  : %s\n", result);
    } else {
        printf("Result : NOT FOUND\n");
    }

    return 0;
}
