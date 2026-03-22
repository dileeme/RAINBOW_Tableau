#include <stdio.h>
#include <string.h>
#include "rainbow.h"
#include "md5_helper.h"

RainbowEntry table[TABLE_SIZE];
int table_count = 0;

static const char *wordlist[TABLE_SIZE] = {
    "password", "123456", "password123", "admin", "letmein",
    "welcome", "monkey", "dragon", "master", "sunshine",
    "shadow", "michael", "football", "superman", "batman",
    "trustno1", "iloveyou", "princess", "rockyou", "qwerty",
    "abc123", "111111", "1234567", "12345678", "passw0rd",
    "login", "hello", "charlie", "donald", "access"
};

int main() {
    printf("=== table_builder ===\n");
    printf("Building MD5 rainbow table from wordlist...\n\n");

    FILE *out = fopen("rainbow_table.txt", "w");
    if (!out) {
        fprintf(stderr, "Error: could not open rainbow_table.txt for writing\n");
        return 1;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        char hex[HASH_HEX_LEN];
        md5_hex(wordlist[i], hex);
        fprintf(out, "%s:%s\n", hex, wordlist[i]);
        printf("  %s  ->  %s\n", wordlist[i], hex);
    }

    fclose(out);
    printf("\n[+] Table saved to rainbow_table.txt (%d entries)\n", TABLE_SIZE);
    printf("[*] Run ./lookup or ./analyzer next.\n");
    return 0;
}
