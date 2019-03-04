#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node{
    int* key;
    struct Node* next;
} Node;

typedef struct {
    Node* first;
    int mtf;
    int trans;
} List;

int isEmpty(List* list){
    if(list->first == NULL)
        return 1;
    else
        return 0;
}

void printlist(List* list){
    Node* node = list->first;

    while(node != NULL) {
        int d = *(int*) node->key;
        printf("%d -> ", d);

        node = node->next;
    }
    printf("end\n");
}

int delete(List* list, int* key){
    Node* node = list->first;
    Node* prev = NULL;

    while(node != NULL){

        list->mtf++;
        list->trans++;

        if (memcmp(key, node->key, sizeof(int))==0){

            if(prev != NULL){
                prev->next = node->next;
            } else {
                list->first = node->next;
            }

            return 1;
        }

        prev = node;
        node = node->next;
    }

    return 0;
}

int findMTF(List* list, int* key){
    Node* node = list->first;
    Node* prev = NULL;

    while(node != NULL){
        list->mtf++;
        
        if(memcmp(key, node->key, sizeof(int)) == 0){
            if (prev != NULL){
                prev->next = node->next;
                node->next = list->first;
                list->first = node;
            }

            return 1;
        }

        prev = node;
        node = node->next;

    }


    return 0;
}

int findTRANS(List* list, int* key){
    Node* node = list->first;
    Node* prev = NULL;
    Node* prevprev = NULL;

    while (node != NULL){
        list->trans++;

        if(memcmp(key, node->key, sizeof(int)) == 0){
            
            if(prev != NULL){
                
                if(prevprev != NULL){
                    prevprev->next = node;
                    prev->next = node->next;
                    node->next = prev;
                } else {
                    list->first = node;
                    prev->next = node->next;
                    node->next = prev;
                }
            }

            return 1;
        }

        prevprev = prev;
        prev = node;
        node = node->next;
    }


    return 0;
}

int insert(List* list, int* key){
    Node* node = malloc(sizeof(Node));
    
    node->next = list->first;
    node->key = key;

    list->first = node;

    return 1;
}

int main(){
    const int size = 100;

    int array[size];
    for (int i = 0; i < size; i++){
        array[i] = i+1;
    }

    srand(time(NULL));
    for (int i = 0; i < size - 1; i++){
        int random = rand() % size;
        int temp = array[i];
        array[i] = array[random];
        array[random] = temp;
    }

    List trans = {NULL,0,0};
    List mtf = {NULL,0,0};

    for (int i = 0; i < size; i++){
        insert(&trans, &array[i]);
        insert(&mtf, &array[i]);
    }

    //printlist(&trans);
    //printlist(&mtf);

    for(int j = size; j >= 1; j--){
        for(int i = 1; i<=size; i++){
            findTRANS(&trans, &i);
            findMTF(&mtf, &i);
        }
        delete(&trans, &j);
        delete(&mtf, &j);
    }
    printf("Comparisons:\ntrans: %d\nmtf: %d\n",trans.trans,mtf.mtf);

}
