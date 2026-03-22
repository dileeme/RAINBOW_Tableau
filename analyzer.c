#include <stdio.h>
#include <string.h>
#include "rainbow.h"
#include "md5_helper.h"

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

int looks_salted(const char *input) {
    return (strncmp(input, "$2b$", 4) == 0 ||
            strncmp(input, "$2a$", 4) == 0 ||
            strncmp(input, "$5$",  3) == 0 ||
            strncmp(input, "$6$",  3) == 0 ||
            strncmp(input, "$1$",  3) == 0);
}

int is_md5_hex(const char *s) {
    if (strlen(s) != 32) return 0;
    for (int i = 0; i < 32; i++) {
        char c = s[i];
        if (!((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F')))
            return 0;
    }
    return 1;
}

void analyze(const char *input) {
    printf("----------------------------------------------\n");
    printf("Input  : %s\n", input);

    if (looks_salted(input)) {
        printf("Result : SALTED HASH DETECTED\n");
        printf("Reason : Salt prefix found — rainbow table is useless.\n");
        printf("         Every user's hash is unique even for same password.\n\n");
        return;
    }

    char hash[HASH_HEX_LEN];
    if (is_md5_hex(input)) {
        strncpy(hash, input, HASH_HEX_LEN);
        printf("Mode   : Direct hash lookup\n");
    } else {
        md5_hex(input, hash);
        printf("Mode   : Plaintext hashed then looked up\n");
        printf("MD5    : %s\n", hash);
    }

    const char *found = lookup_hash(hash);
    if (found) {
        printf("Result : CRACKED  ->  \"%s\"\n", found);
    } else {
        printf("Result : NOT FOUND in table\n");
    }
    printf("\n");
}

int main() {
    printf("=== analyzer ===\n");
    printf("Reads rainbow_table.txt, then accepts input interactively.\n\n");

    if (!load_table("rainbow_table.txt")) return 1;
    printf("Table loaded: %d entries\n\n", table_count);

    char input[256];
    printf("Enter hash or plaintext ('quit' to exit):\n\n");

    while (1) {
        printf("> ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) continue;
        if (strcmp(input, "quit") == 0) break;
        analyze(input);
    }

    printf("Done.\n");
    return 0;
}
