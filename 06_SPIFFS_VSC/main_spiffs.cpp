// #include <Arduino.h>

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#include "SPIFFS.h"

#define MAX_LINE_LEN 256
#define HASH_SIZE 100

// Node for hashmap (linked list)
typedef struct HashNode {
    char* key;
    void* value;
    struct HashNode* next;
} HashNode;

// Hashmap structure for each type
typedef struct {
    HashNode **buckets;
} HashMap;

HashMap* ssMap;
HashMap* scMap;
HashMap* slMap;
HashMap* sfMap;

// HashMap function prototypes
HashMap* createHashMap();

void destroyHashMap(HashMap* map);
void putStringString(HashMap* map, const char *key, const char *value);
void putStringChar(HashMap* map, const char *key, char value);
void putStringLong(HashMap* map, const char *key, long long value);
void putStringFloat(HashMap* map, const char *key, float value);

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
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->buckets = (HashNode**)calloc(HASH_SIZE, sizeof(HashNode*));
    return map;
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
void putStringChar(HashMap* map, const char *key, char* value) {
    unsigned int index = hash(key);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->value = malloc(sizeof(char));
    *((char *)newNode->value) = *value;
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


void putStringFloat(HashMap* map, const char * key, float value) {
    unsigned int index = hash(key);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = strdup(key);
    newNode->value = malloc(sizeof(float));
    *((float*)newNode->value) = value;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

// Extract value from hashmap by key
void* get(HashMap* map, const char *key) {
    unsigned int index = hash(key);
    HashNode* node = map->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

void report(const char m) {
    HashMap* map;

    switch (m) {
        case 's': map = ssMap; break;
        case 'c': map = scMap; break;
        case 'l': map = slMap; break;
        case 'f': map = sfMap; break;
    }

    for(unsigned int index = 0; index < HASH_SIZE; ++index){
        HashNode * node = map->buckets[index];
        while (node) {
            switch (m) {
                case 's': printf("\n[ssMap] %s = %s",node->key,(char*) node->value); break;
                case 'c': printf("\n[scMap] %s = %c",node->key,*(char*) node->value); break;
                case 'l': printf("\n[slMap] %s = %lld",node->key,*(long long*) node->value); break;
                case 'f': printf("\n[sfMap] %s = %f",node->key,*(float*) node->value); break;
            }

            node = node->next;
        }
    }
}

void report_all() {
    report('s');
    report('c');
    report('l');
    report('f');
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
    if (sscanf(line, "%255[^=] = %255[^\n;]", key, value) != 2) {
        printf("Error: invalid line format\n");
        return;
    }

    // Trim spaces
    trim(key);
    trim(value);

    printf("Key: ");
    printf("'%s'",key);

    printf(",Value: ");
    printf("'%s'\n",value);

    // Determine the value type based on the value format
    if (value[0] == '"' && value[strlen(value) - 1] == '"') {
        // String value
        value[strlen(value) - 1] = '\0'; // Remove the trailing quote
        putStringString(ssMap, key, value + 1); // Skip the leading quote
    } else if (value[0] == '\'' && value[strlen(value) - 1] == '\'') {
        // Char value
        putStringChar(scMap, key, value+1); // Only one char between the quotes
    } else if (value[0] == '[' && value[strlen(value) - 1] == ']') {


    } else if (strchr(value,'.') != NULL) {
        float floatValue;
        if (sscanf(value, "%f", &floatValue) == 1) {
            putStringFloat(sfMap, key, floatValue);
        }
    } else {
        printf("say?");
        long long longValue;
        if (sscanf(value, "%lld", &longValue) == 1) {
            putStringLong(slMap, key, longValue);
        } else {
            printf("Error: invalid numeric value for key %s\n", key);
        }
    }
}

bool parseParams(){
  ssMap = createHashMap();
  scMap = createHashMap();
  slMap = createHashMap();
  sfMap = createHashMap();

  FILE *fd = fopen("./params.txt", "r");

  if(!fd){
    printf("Failed to open file\n");
    return false;
  }


  char line[MAX_LINE_LEN];
  while (fgets(line, sizeof(line), fd)) {
    // Remove trailing newline
    line[strcspn(line, "\n")] = 0;
    parseLine(line);
  }

  // while (fd.position() < fd.size()) {
  //   String line = fd.readStringUntil('\n');

  //   printf("// \n");
  //   printf(line);
  //   printf("\\ ");

  //   parseLine(line.c_str());
  // }
  // fd.close();
  return true;
}

/*
bool spiffs_init(int attempts){
  printf("Attemping to begin the FS: ");
  int attempt_counter = 0;
  while ((!SPIFFS.begin(true)) && (++attempt_counter < attempts) ) {
    printf(attempt_counter);
    printf(", ");
    SPIFFS.format();
  }
  if (SPIFFS.begin(true)){
    printf("done\n");
    return true;
  } else{
    printf("fail\n");
    return false;
  }
}

bool parseParams(){
  ssMap = createHashMap();
  scMap = createHashMap();
  slMap = createHashMap();

  File fd = SPIFFS.open("/params.txt","r");

  if(!fd){
    printf("Failed to open file\n");
    return false;
  }

  while (fd.position() < fd.size()) {
    String line = fd.readStringUntil('\n');
    parseLine(line.c_str());
  }
  fd.close();
  return true;
}





*/


bool connectWifi() {
    char * ssid = (char *) get(ssMap, "wifi::ssid");
    char * pass = (char *) get(ssMap, "wifi::password");
    printf("Connecting to %s\nPassword is '%s'",ssid, pass);
    return true;
}

void setup() {
  //Serial.begin(9600);

  //spiffs_init(10);

  parseParams();

  connectWifi();
}

void loop(){

}

int main() {
    printf("Hello world\n");

    parseParams();

    connectWifi();


    report('s');
    report('c');
    report('l');
    report('f');

    return 0;
}
