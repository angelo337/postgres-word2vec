#ifndef INDEX_UTILS_H
#define INDEX_UTILS_H

#include "postgres.h"
#include "utils/array.h"

typedef struct TopKEntry{
  int id;
  float distance;
}TopKEntry;

typedef struct TopKWordEntry{
  char* word;
  float distance;
}TopKWordEntry;

typedef TopKEntry* TopK;

typedef struct CoarseQuantizerEntry{
    int id;
    float* vector;
}CoarseQuantizerEntry;


typedef struct CodebookEntry{
  int pos;
  int code;
  float* vector;
} CodebookEntry;

typedef struct CodebookEntryComplete{
  int pos;
  int code;
  float* vector;
  int count;
} CodebookEntryComplete;

typedef struct WordVectors{
  int* ids;
  float** vectors;
} WordVectors;

typedef struct Blacklist{
  int id;
  bool isValid; // id and next are only valid if isValid = true
  struct Blacklist* next;
} Blacklist;

typedef enum {ORIGINAL, NORMALIZED, PQ_QUANTIZATION, CODEBOOK, RESIDUAL_QUANTIZATION, COARSE_QUANTIZATION, RESIDUAL_CODBOOK} tableType;

typedef enum {PARAM_PVF, PARAM_W} parameterType;

typedef CodebookEntry* Codebook;

typedef CodebookEntryComplete* CodebookWithCounts;

typedef CoarseQuantizerEntry* CoarseQuantizer;

void updateTopK(TopK tk, float distance, int id, int k, int maxDist);

void updateTopKWordEntry(char** term, char* word);

bool inBlacklist(int id, Blacklist* bl);

void addToBlacklist(int id, Blacklist* bl, Blacklist* emptyBl);

float squareDistance(float* v1, float* v2, int n);

void shuffle(int* input, int* output, int inputSize, int outputSize);

CoarseQuantizer getCoarseQuantizer(int* size);

Codebook getCodebook(int* positions, int* codesize, char* tableName);

CodebookWithCounts getCodebookWithCounts(int* positions, int* codesize, char* tableName);

WordVectors getVectors(char* tableName, int* ids, int idsSize);

void getArray(ArrayType* input, Datum** result, int* n);

void getTableName(tableType type, char* name, int bufferSize);

void getParameter(parameterType type, int* param);

char **split(const char *str, char sep);

void updateCodebook(float** rawVectors, int rawVectorsSize, int subvectorSize, CodebookWithCounts cb, int cbPositions, int cbCodes, int** nearestCentroids, int* countIncs);

void updateCodebookRelation(CodebookWithCounts cb, int cbPositions, int cbCodes, char* tableNameCodebook, int* countIncs, int subvectorSize);

void updateProductQuantizationRelation(int** nearestCentroids, char** tokens, int cbPositions, CodebookWithCounts cb, char* pqQuantizationTable, int rawVectorsSize, int* cqQuantizations);

void updateWordVectorsRelation(char* tableName, char** tokens, float** rawVectors, int rawVectorsSize, int vectorSize);

int compare (const void * a, const void * b);

void convert_bytea_int32(bytea* bstring, int32** output, int32* size);

void convert_bytea_int16(bytea* bstring, int16** output, int* size);

void convert_bytea_float4(bytea* bstring, float4** output, int* size);

void convert_int32_bytea(int32* input, bytea** output, int size);

void convert_int16_bytea(int16* input, bytea** output, int size);

void convert_float4_bytea(float4* input, bytea** output, int size);

#endif /*INDEX_UTILS_H*/
