#include <Arduino.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "SPIFFS.h"

#define MAX_LINE_LEN 256
#define HASH_SIZE 100

// Node for hashmap (linked list)
typedef struct HashNode {
    char *key;
    void *value;
    struct HashNode *next;
} HashNode;

// Hashmap structure for each type
typedef struct {
    HashNode **buckets;
} HashMap;

HashMap *stringStringMap;
HashMap *stringCharMap;
HashMap *stringLongMap;

// HashMap function prototypes
HashMap* createHashMap();
void destroyHashMap(HashMap* map);
void putStringString(HashMap* map, const char *key, const char *value);
void putStringChar(HashMap* map, const char *key, char value);
void putStringLong(HashMap* map, const char *key, long long value);
void* get(HashMap* map, const char *key);

// Hash function
unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % HASH_SIZE;
}

// Create a new hashmap
HashMap* createHashMap() {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (HashNode **)calloc(HASH_SIZE, sizeof(HashNode *));
    return map;
}

// Destroy hashmap and free memory
void destroyHashMap(HashMap* map) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashNode *node = map->buckets[i];
        while (node) {
            HashNode *temp = node;
            node = node->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}

// Insert into <string, string> hashmap
void putStringString(HashMap* map, const char *key, const char *value) {
    unsigned int index = hash(key);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

// Insert into <string, char> hashmap
void putStringChar(HashMap* map, const char *key, char value) {
    unsigned int index = hash(key);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->value = malloc(sizeof(char));
    *((char *)newNode->value) = value;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

// Insert into <string, long long> hashmap
void putStringLong(HashMap* map, const char *key, long long value) {
    unsigned int index = hash(key);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->value = malloc(sizeof(long long));
    *((long long *)newNode->value) = value;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

// Extract value from hashmap by key
void* get(HashMap* map, const char *key) {
    unsigned int index = hash(key);
    HashNode *node = map->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

// Function to trim spaces from both ends of a string
void trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
}


// Function to parse a line from the file
void parseLine(const char *line) {
    char key[MAX_LINE_LEN];
    char value[MAX_LINE_LEN];

    // Read the key and value
    if (sscanf(line, " %255[^=] = %255[^\n] ", key, value) != 2) {
        printf("Error: invalid line format\n");
        return;
    }

    // Trim spaces
    trim(key);
    trim(value);

    Serial.print("Key: ");
    Serial.println(key);

    Serial.print("Value: ");
    Serial.println(value);

    // Determine the value type based on the value format
    if (value[0] == '"' && value[strlen(value) - 1] == '"') {
        // String value
        value[strlen(value) - 1] = '\0'; // Remove the trailing quote
        putStringString(stringStringMap, key, value + 1); // Skip the leading quote
    } else if (value[0] == '\'' && value[strlen(value) - 1] == '\'') {
        // Char value
        putStringChar(stringCharMap, key, value[1]); // Only one char between the quotes
    } else {
        // Long value
        long long numValue;
        if (sscanf(value, "%lld", &numValue) == 1) {
            putStringLong(stringLongMap, key, numValue);
        } else {
            printf("Error: invalid numeric value for key %s\n", key);
        }
    }
}

bool spiffs_init(int attempts){
  Serial.print("Attemping to begin the FS: ");
  int attempt_counter = 0;
  while ((!SPIFFS.begin(true)) && (++attempt_counter < attempts) ) {
    Serial.print(attempt_counter);
    Serial.print(", ");
    SPIFFS.format();
  }
  if (SPIFFS.begin(true)){
    Serial.println("done");
    return true;
  } else{
    Serial.println("fail");
    return false;
  }
}

bool parseParams(){
  stringStringMap = createHashMap();
  stringCharMap = createHashMap();
  stringLongMap = createHashMap();

  File fd = SPIFFS.open("/params.txt","r");
  
  if(!fd){
    Serial.println("Failed to open file");
    return false;
  }

  while (fd.position() < fd.size()) {
    String line = fd.readStringUntil('\n');

    Serial.println("// ");
    Serial.print(line);
    Serial.print("\\ ");

    parseLine(line.c_str());
  }
  fd.close();
  return true;
}

bool connectWifi() {
  char *wifi = (char *)get(stringStringMap, "wifi");
  printf("wifi = %s\n", wifi);

  char passwordChar = *((char *)get(stringCharMap, "password"));
  printf("password (first char) = %c\n", passwordChar);
  return true;
}

void setup() {
  Serial.begin(9600);

  spiffs_init(10);

  parseParams();
  
  connectWifi();  
}

void loop(){

}
