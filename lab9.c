#include <stdio.h>
#include <stdlib.h>

// RecordType
#define HASH_SIZE 23

struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct Recordtype* next;
};

// Fill out this structure
struct HashType
{
	struct RecordType* hashArray[HASH_SIZE];
};

// Compute the hash function
int hash(int x)
{
	return x % HASH_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	for (i=0;i<hashSz;++i)
	{
        if (pHashArray->hashArray[i] != NULL)
        {
			printf("Index %d -> ", i);
            struct RecordType* currRecord = pHashArray->hashArray[i];
            printf("%d %c %d", currRecord->id, currRecord->name, currRecord->order);
            while (currRecord->next != NULL)
            {
                currRecord = currRecord->next;
                printf(" -> %d %c %d", currRecord->id, currRecord->name, currRecord->order);
            }
			printf("\n");
		}
	}
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create a hash table
    struct HashType hashTable;
    for (int i = 0; i < HASH_SIZE; ++i) {
        hashTable.hashArray[i] = NULL; // Initialize all pointers to NULL
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i) {
        struct RecordType* currRecord = pRecords + i;
        int hashIndex = hash(currRecord->id);
        if (hashTable.hashArray[hashIndex] == NULL) {
            // If the slot is empty, insert the record
            hashTable.hashArray[hashIndex] = currRecord;
        } else {
            // If the slot is not empty, handle collision
            // Append the record to the end of the chain
            struct RecordType* temp = hashTable.hashArray[hashIndex];
            while (temp->next != NULL) {
                temp = temp->next;
            }
			//add record to the end of the chain after finding null node
            temp->next = currRecord;
        }
    }

    // Display records in the hash table
    displayRecordsInHash(&hashTable, HASH_SIZE);

    // Free dynamically allocated memory for pRecords
    free(pRecords);

    return 0;
}