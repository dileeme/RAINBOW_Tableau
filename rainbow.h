#ifndef RAINBOW_H
#define RAINBOW_H

#define TABLE_SIZE    30
#define HASH_HEX_LEN  33
#define MAX_PLAIN_LEN 64

typedef struct {
    char hash[HASH_HEX_LEN];
    char plaintext[MAX_PLAIN_LEN];
} RainbowEntry;

/* Shared table — defined in table_builder, used by analyzer and lookup */
extern RainbowEntry table[TABLE_SIZE];
extern int table_count;

#endif
