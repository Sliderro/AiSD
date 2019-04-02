#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

static int asc_desc_flag = -1;
static int stat_flag = 0;
static char type_flag = '0';

typedef struct{
    int comparisons;
    int swaps;
    float t;
} Stat; //this structure keeps info about operations made by sorting algorithm

//declaration of functions used in program
void swap(int* a, int* b, Stat *stat); 
void insertion_sort(int* array, int size, Stat *stat);
void mq_insertion_sort(int* array, int begin, int end, Stat *stat);
void select_sort(int* array, int size, Stat *stat);
int quick_sort_partition(int* array, int begin, int end, Stat *stat);
void quick_sort(int* array, int begin, int end, Stat *stat);
int mquick_sort_partition(int* array, int begin, int end, Stat *stat);
void mquick_sort(int* array, int begin, int end, Stat *stat);
int left(int i);
int right(int i);
void max_heapify(int* array, int i, int heap_size, Stat *stat);
void build_max_heapify(int* array, int size, Stat *stat);
void heap_sort(int* array, int size, Stat *stat);
void print_array(int* array, int size);
void randomize_array(int* array, int size, Stat *stat);
void print_stat(int n, Stat *stat, FILE *f);
void clear_stat(Stat *stat);
void avg_stat(int n, Stat *stat, Stat *avgStat);

int main(int argc, char **argv){

    srand(time(NULL));
    FILE *f;
    int c;
    char *filename;
    
    while (1)
    {
        static struct option long_options[] = {
            {"asc",     no_argument,        0, 'a'},
            {"desc",    no_argument,        0, 'd'},
            {"stat",    required_argument,  0, 's'},
            {"type",    required_argument,  0, 't'},
            {0, 0, 0, 0}
        }; //defining options that can be used when executing the program
        int option_index = 0;

        c = getopt_long (argc, argv, "ads:t:", long_options, &option_index);
        if (c == -1)// when c == -1 no more options are used
            break;

        switch (c) {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;
            
            case 'a':
                asc_desc_flag = 1; 
                break;

            case 'd':
                asc_desc_flag = 0;
                break;

            case 's':
                stat_flag = 1;
                filename = malloc(strlen(optarg) + 1);
                strcpy(filename, optarg);
                break;

            case 't':
                if(strcmp(optarg, "quick")==0){
                    type_flag = 'q';
                } else if(strcmp(optarg, "heap")==0){
                    type_flag = 'h';
                } else if(strcmp(optarg, "select")==0){
                    type_flag = 's';
                } else if(strcmp(optarg, "insert")==0){
                    type_flag = 'i';    
                } else if(strcmp(optarg, "mquick")==0){
                    type_flag = 'm';
                } else {
                    type_flag = 'e';
                }
                    
                break;

            case '?':
                break;

            default:
                abort ();
        }
    }

    if(stat_flag){
        f = fopen(filename, "w");
        fprintf(f,";;;quick;;;;;select;;;;;insert;;;;;heap;;;;;mquick\n");
        fprintf(f, "n;c;s;t;c/n;s/n;c;s;t;c/n;s/n;c;s;t;c/n;s/n;c;s;t;c/n;s/n;c;s;t;c/n;s/n\n");
        int k;
        scanf("%d", &k);
        int *array;
        int *arraycopy;
        int counter = 0;
        Stat *stat = malloc(5*sizeof(Stat));
        Stat *avgStat = malloc(5*sizeof(Stat));
        for(int j=1; j<=100; j++){
            for(int l=0; l<5; l++){
                clear_stat(&avgStat[l]);
            }
            int size = j * 100;
            fprintf(f,"%d",size);
            printf("%d\n", size);
            for(int i=0; i<k; i++){
                array = malloc(size*sizeof(int));
                arraycopy = malloc(size*sizeof(int));
                for(int h=0; h<size; h++){
                    array[h]=h+1;
                    arraycopy[h]=h+1;
                }
                randomize_array(array,size,NULL);
                for(int l=0; l<5; l++){
                    clear_stat(&stat[l]);
                }
                for(int l=0; l<size; l++){
                    arraycopy[l]=array[l];
                }
                quick_sort(arraycopy,0,size-1,&stat[0]);
                //print_stat(size, &stat,NULL);
                for(int l=0; l<size; l++){
                    arraycopy[l]=array[l];
                }
                select_sort(arraycopy,size,&stat[1]);
                //print_stat(size, &stat,NULL);
                for(int l=0; l<size; l++){
                    arraycopy[l]=array[l];
                }
                insertion_sort(arraycopy,size,&stat[2]);
                for(int l=0; l<size; l++){
                    arraycopy[l]=array[l];
                }
                heap_sort(arraycopy,size,&stat[3]);
                for(int l=0; l<size; l++){
                    arraycopy[l]=array[l];
                }
                mquick_sort(arraycopy,0,size-1,&stat[4]);
                //fprintf(f,"\n");
                for(int l=0; l<5; l++){
                    avg_stat(i,&stat[l],&avgStat[l]);
                }
                free (array);
                free (arraycopy);
            }
            for(int l=0;l<5;l++){
                print_stat(size,&avgStat[l],f);
            }
            fprintf(f,"\n");
        }
        fclose(f);
    } else if (type_flag != '0' && asc_desc_flag != -1){
        Stat stat = {0,0,0};
        if(type_flag == 'q'){
            int size;
            printf("How many elements you want to sort?\n");
            scanf("%d",&size);
            int *array = malloc(size*sizeof(int));
            printf("Now pass the elements:\n");
            for (int i=0; i<size; i++){
                scanf("%d", &array[i]);
            }
            quick_sort(array,0,size-1,&stat);
            print_array(array,size);
            print_stat(size,&stat,NULL);
        } else if(type_flag == 'h'){
            int size;
            printf("How many elements you want to sort?\n");
            scanf("%d",&size);
            int *array = malloc(size*sizeof(int));
            printf("Now pass the elements:\n");
            for (int i=0; i<size; i++){
                scanf("%d", &array[i]);
            }
            heap_sort(array,size,&stat);
            print_array(array,size);
            print_stat(size,&stat,NULL);
        } else if(type_flag == 's'){
            int size;
            printf("How many elements you want to sort?\n");
            scanf("%d",&size);
            int *array = malloc(size*sizeof(int));
            printf("Now pass the elements:\n");
            for (int i=0; i<size; i++){
                scanf("%d", &array[i]);
            }
            select_sort(array,size,&stat);
            print_array(array,size);
            print_stat(size,&stat,NULL);
        } else if(type_flag == 'i'){
            int size;
            printf("How many elements you want to sort?\n");
            scanf("%d",&size);
            int *array = malloc(size*sizeof(int));
            printf("Now pass the elements:\n");
            for (int i=0; i<size; i++){
                scanf("%d", &array[i]);
            }
            insertion_sort(array,size,&stat);
            print_array(array,size);
            print_stat(size,&stat,NULL);
        } else if(type_flag == 'm'){
            int size;
            printf("How many elements you want to sort?\n");
            scanf("%d",&size);
            int *array = malloc(size*sizeof(int));
            printf("Now pass the elements:\n");
            for (int i=0; i<size; i++){
                scanf("%d", &array[i]);
            }
            mquick_sort(array,0,size-1,&stat);
            print_array(array,size);
            print_stat(size,&stat,NULL);
        } else {
            printf("%s is wrong argument for \"-t/--type\"\n", optarg);
        }
    } else {
        if(asc_desc_flag == -1)
            printf("Option \"-t/--type\" requires option \"-a/--asc/-d/--desc\" to be executed\n");
        else
            printf("Program need arguments to be executed\n");
    }
    


    while (optind < argc){
        printf("%s\n", argv[optind++]);
    }


    
    return 0;
}

void swap(int* a, int* b, Stat *stat){
    int k;
    k = *a;
    *a = *b;
    *b = k;
    if (stat != NULL)
        stat->swaps+=2;
}

void insertion_sort(int* array, int size, Stat *stat){
    clock_t start = clock();
    int key;
    int i;
    for(int j = 1; j < size; j++){
        key = array[j];
        i = j - 1;
        if(asc_desc_flag){
            stat->comparisons++;
            while (i >= 0 && array[i]>key){
                stat->comparisons++;
                array[i+1]=array[i];
                stat->swaps++;
                i--;
            }
        } else {
            stat->comparisons++;
            while (i >= 0 && array[i]<key){
                stat->comparisons++;
                array[i+1]=array[i];
                stat->swaps++;
                i--;
            }
        }
        array[i+1] = key;
        stat->swaps++;
    }
    clock_t end = clock();
    stat->t = (float)(end - start)/CLOCKS_PER_SEC;
}

void mq_insertion_sort(int* array, int begin, int end, Stat *stat){
    int key;
    int i;
    for(int j = begin; j < end+1; j++){
        key = array[j];
        i = j - 1;
        if(asc_desc_flag){
            stat->comparisons++;
            while (i >= 0 && array[i]>key){
                stat->comparisons++;
                array[i+1]=array[i];
                stat->swaps++;
                i--;
            }
        } else {
            stat->comparisons++;
            while (i >= 0 && array[i]<key){
                stat->comparisons++;
                array[i+1]=array[i];
                stat->swaps++;
                i--;
            }
        }
        array[i+1] = key;
        stat->swaps++;
    }
}


void select_sort(int* array, int size, Stat *stat){
    clock_t start = clock();
    int min;
    for(int j = 0; j < size - 1; j++){
        min = j;
        for(int i = j + 1; i < size; i++){
            if(asc_desc_flag){
                stat->comparisons++;
                if(array[i] < array[min]){
                    min = i;
                }
            } else {
                stat->comparisons++;
                if(array[i] > array[min]){
                    min = i;
                }
            }
        }
        swap(&array[min], &array[j], stat);
    }
    clock_t end = clock();
    stat->t = (float)(end - start)/CLOCKS_PER_SEC;
}


int quick_sort_partition(int* array, int begin, int end, Stat *stat){
    int x = array[end];
    int i = begin - 1;
    for(int j = begin; j < end; j++){
        if(asc_desc_flag){
            stat->comparisons++;
            if(array[j] <= x){
                i++;
                swap(&array[i], &array[j], stat);
            }
        } else {
            stat->comparisons++;
            if(array[j] >= x){
                i++;
                swap(&array[i], &array[j], stat);
            }
        }
        
    }
    swap(&array[i+1], &array[end], stat);
    return i+1;
}

int mquick_sort_partition(int* array, int begin, int end, Stat *stat){
    int k[] = {array[begin], array[(begin+end)/2], array[end]};
    insertion_sort(k,3,stat);
    stat->comparisons++;
    if(k[1]==array[begin]){
        swap(&array[begin],&array[end], stat);
    } else if(k[1]==array[(begin+end)/2]){
        stat->comparisons++;
        swap(&array[(begin+end)/2],&array[end], stat);
    } else {
        stat->comparisons++;
    }
    int x = array[end];
    int i = begin - 1;
    for(int j = begin; j < end; j++){
        if(asc_desc_flag){
            stat->comparisons++;
            if(array[j] <= x){
                i++;
                swap(&array[i], &array[j], stat);
            }
        } else {
            stat->comparisons++;
            if(array[j] >= x){
                i++;
                swap(&array[i], &array[j], stat);
            }
        }
        
    }
    swap(&array[i+1], &array[end], stat);
    return i+1;
}



void quick_sort(int* array, int begin, int end, Stat *stat){
    clock_t start = clock();
    int q;
    if (begin < end) {
        q = quick_sort_partition(array, begin, end, stat);
        quick_sort(array, begin, q - 1, stat);
        quick_sort(array, q + 1, end, stat);
    }
    clock_t c_end = clock();
    if(begin == 0){
        stat->t = (float)(c_end - start)/CLOCKS_PER_SEC;
    }
}

void mquick_sort(int* array, int begin, int end, Stat *stat){
    clock_t start = clock();
    if(end - begin >= 15){
        int q;
        if (begin < end) {
            q = mquick_sort_partition(array, begin ,end, stat);
            mquick_sort(array, begin, q - 1, stat);
            mquick_sort(array, q + 1, end, stat);
        }
    } else {
        mq_insertion_sort(array,begin,end, stat);
    }
    clock_t c_end = clock();
    if(begin == 0){
        stat->t = (float)(c_end - start)/CLOCKS_PER_SEC;
    }
}



int left(int i){
    return 2 * i;
}

int right(int i){
    return 2 * i + 1;
}

void max_heapify(int* array, int i, int heap_size, Stat *stat){
    int max;
    int l = left(i);
    int r = right(i);
    if(asc_desc_flag){
        stat->comparisons++;
        if (l <= heap_size && array[l-1] > array[i-1]){
            max = l;
        } else {
            max = i;
        }
        stat->comparisons++;
        if (r <= heap_size && array[r-1] > array[max-1]){
            max = r;
        }
    } else {
        stat->comparisons++;
        if (l <= heap_size && array[l-1] < array[i-1]){
            max = l;
        } else {
            max = i;
        }
        stat->comparisons++;
        if (r <= heap_size && array[r-1] < array[max-1]){
            max = r;
        }
    }
    if (max != i){
        swap(&array[i-1], &array[max-1], stat);
        max_heapify(array,max,heap_size, stat);
    }
}


void build_max_heapify(int* array, int size, Stat *stat){
    for(int i = size; i >= 1; i--){
        max_heapify(array,i,size, stat);
    }
}

void heap_sort(int* array, int size, Stat *stat){
    clock_t start = clock();
    int heap_size = size;
    build_max_heapify(array,size, stat);
    for(int i = size; i >= 2; i--){
        swap(&array[0], &array[i-1], stat);
        heap_size--;
        max_heapify(array,1,heap_size, stat);
    }
    clock_t end = clock();
    stat->t = (float)(end - start)/CLOCKS_PER_SEC;
}


void print_array(int* array, int size){
    for(int i = 0; i < size; i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}

void randomize_array(int* array, int size, Stat *stat){
    for (int i = 0; i < size; i++){
        int random = rand() % size;
        swap(&array[i],&array[random], stat);
    }
}

void print_stat(int n, Stat *stat, FILE *f){
    int c = stat->comparisons;
    int s = stat->swaps;
    float t = stat->t;
    if(f == NULL){
        printf("c: %d, s: %d, t: %f\n", c,s,t);
    } else {
        float cn = (float) c/n;
        float sn = (float) s/n;
        fprintf(f,";%d;%d;%f;%f;%f",c,s,t,cn,sn);
    }
}

void clear_stat(Stat *stat){
    stat->comparisons = 0;
    stat->swaps = 0;
    stat->t = 0;
}

void avg_stat(int n, Stat *stat, Stat *avgStat){
    avgStat->comparisons = (avgStat->comparisons * n + stat->comparisons)/(n+1);
    avgStat->swaps = (avgStat->swaps * n + stat->swaps)/(n+1);
    avgStat->t = (avgStat->t * n + stat->t)/(n+1);
}