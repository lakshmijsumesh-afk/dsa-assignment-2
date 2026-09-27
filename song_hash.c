/* ============================================================
   Music Application - Song ID Hash Table
   Division Method Hashing (with chaining) vs Linear Search
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10          /* primary table size used for the assignment */
#define TABLE_SIZE_ALT 11      /* prime size used only for the bonus comparison */
#define N 8                    /* number of song IDs */

/* ---------- Linked list node for chaining ---------- */
typedef struct Node {
    int id;
    struct Node *next;
} Node;

Node *table[TABLE_SIZE];
int collisionCount = 0;

/* ---------- Utility: print the whole table ---------- */
void displayTable(Node *tbl[], int size) {
    for (int i = 0; i < size; i++) {
        printf("  [%2d] -> ", i);
        Node *temp = tbl[i];
        if (temp == NULL) {
            printf("EMPTY");
        }
        while (temp != NULL) {
            printf("%d", temp->id);
            if (temp->next != NULL) printf(" -> ");
            temp = temp->next;
        }
        printf("\n");
    }
}

/* ---------- Insert using Division Method + chaining ---------- */
void insert(Node *tbl[], int size, int id, int *collisions) {
    int index = id % size;
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->id = id;
    newNode->next = NULL;

    if (tbl[index] != NULL) {
        printf("  >>> COLLISION: Song ID %d also hashes to index %d "
               "(already occupied) -> chained.\n", id, index);
        (*collisions)++;
        Node *temp = tbl[index];
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    } else {
        tbl[index] = newNode;
    }
    printf("Inserted %d at index %d. Table now:\n", id, index);
    displayTable(tbl, size);
    printf("\n");
}

/* ---------- Search using hashing (division method) ---------- */
int searchHash(Node *tbl[], int size, int key, int *comparisons) {
    *comparisons = 0;
    int index = key % size;
    (*comparisons)++;              /* 1 operation to compute the hash */
    Node *temp = tbl[index];
    while (temp != NULL) {
        (*comparisons)++;          /* 1 comparison per node visited */
        if (temp->id == key) return index;
        temp = temp->next;
    }
    return -1;
}

/* ---------- Plain linear search on the original array ---------- */
int searchLinear(int arr[], int n, int key, int *comparisons) {
    *comparisons = 0;
    for (int i = 0; i < n; i++) {
        (*comparisons)++;
        if (arr[i] == key) return i;
    }
    return -1;
}

int main() {
    int songIDs[N] = {105, 210, 315, 420, 525, 630, 735, 840};

    for (int i = 0; i < TABLE_SIZE; i++) table[i] = NULL;

    printf("================================================================\n");
    printf(" PART (a): INSERTION INTO HASH TABLE (Division Method, size=%d)\n", TABLE_SIZE);
    printf(" h(key) = key mod %d\n", TABLE_SIZE);
    printf("================================================================\n\n");

    for (int i = 0; i < N; i++) {
        insert(table, TABLE_SIZE, songIDs[i], &collisionCount);
    }

    printf("Total collisions during insertion: %d\n", collisionCount);
    double loadFactor = (double)N / TABLE_SIZE;
    printf("Load factor = n/m = %d/%d = %.2f\n\n", N, TABLE_SIZE, loadFactor);

    printf("================================================================\n");
    printf(" PART (b): SEARCH COMPARISON - Hashing vs Linear Search\n");
    printf("================================================================\n\n");

    int searchIDs[] = {420, 735, 105, 999};   /* 999 is NOT present */
    int numSearch = sizeof(searchIDs) / sizeof(searchIDs[0]);

    printf("%-10s %-10s %-18s %-10s %-18s\n",
           "Key", "Found?", "Hash Comparisons", "Found?", "Linear Comparisons");
    printf("---------------------------------------------------------------\n");

    int totalHashCmp = 0, totalLinearCmp = 0;

    for (int i = 0; i < numSearch; i++) {
        int key = searchIDs[i];
        int cmpH, cmpL;
        int posH = searchHash(table, TABLE_SIZE, key, &cmpH);
        int posL = searchLinear(songIDs, N, key, &cmpL);

        totalHashCmp += cmpH;
        totalLinearCmp += cmpL;

        printf("%-10d %-10s %-18d %-10s %-18d\n",
               key,
               (posH != -1) ? "Yes" : "No", cmpH,
               (posL != -1) ? "Yes" : "No", cmpL);
    }

    printf("\nTotal comparisons -> Hashing: %d   Linear Search: %d\n", totalHashCmp, totalLinearCmp);
    printf("Average comparisons -> Hashing: %.2f   Linear Search: %.2f\n\n",
           (double)totalHashCmp / numSearch, (double)totalLinearCmp / numSearch);

    /* ---------------- BONUS: same keys, prime table size 11 ---------------- */
    printf("================================================================\n");
    printf(" BONUS: Re-hashing with a PRIME table size (%d) to test effect\n", TABLE_SIZE_ALT);
    printf(" of table-size choice on collisions -> h(key) = key mod %d\n", TABLE_SIZE_ALT);
    printf("================================================================\n\n");

    Node *table2[TABLE_SIZE_ALT];
    for (int i = 0; i < TABLE_SIZE_ALT; i++) table2[i] = NULL;
    int collisions2 = 0;

    for (int i = 0; i < N; i++) {
        int idx = songIDs[i] % TABLE_SIZE_ALT;
        printf("  %d mod %d = %d\n", songIDs[i], TABLE_SIZE_ALT, idx);
    }
    printf("\n");
    for (int i = 0; i < N; i++) {
        insert(table2, TABLE_SIZE_ALT, songIDs[i], &collisions2);
    }
    printf("Total collisions with table size %d: %d\n", TABLE_SIZE_ALT, collisions2);
    printf("Load factor = %d/%d = %.2f\n", N, TABLE_SIZE_ALT, (double)N / TABLE_SIZE_ALT);

    return 0;
}
