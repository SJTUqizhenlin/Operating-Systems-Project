//Sort a list of numbers using two separate threads
//by sorting half of each list separately then
//recombining the lists

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
#define NUMBER_OF_THREADS 3
static void *sorter(void *params);    /* thread that performs basic sorting algorithm */
static void *merger(void *params);    /* thread that performs merging of results */

// array before sort
static int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};
int result[SIZE];

typedef struct
{
    int from_index;
    int to_index;
} parameters;

int main (int argc, const char * argv[])
{
    pthread_t pt1;
    pthread_t pt2;
    pthread_t pt3;

    int m;
    printf("The array sort before: ");
    for(m = 0;m<SIZE;++m){
        printf("%d ",list[m]);
    }
    printf("\n");

    /* establish the first sorting thread */
    parameters data;
    data.from_index = 0;
    data.to_index = (SIZE/2);
    pthread_create(&pt1, NULL, sorter, &data);

    /* establish the second sorting thread */
    parameters data2;
    data2.from_index = (SIZE/2);
    data2.to_index = SIZE;
    pthread_create(&pt2, NULL, sorter, &data2);

    /* now wait for the 2 sorting threads to finish */
    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);
    printf("The result before merging: ");
    for(m = 0;m<SIZE;++m){
        printf("%d ",list[m]);
    }
    printf("\n");
    /* establish the merge thread */
    parameters data3;
    data3.from_index = 0;
    data3.to_index = (SIZE/2);

    pthread_create(&pt3, NULL, merger, &data3);
    /* wait for the merge thread to finish */
    pthread_join(pt3, NULL);
    /* output the sorted array */
    printf("The result after merging: ");
    for(m=0;m<SIZE;++m){
        printf("%d ",list[m]);
    }
    return 0;
}

static void *sorter(void *params)
{
    parameters *p = (parameters *)params;

    //SORT 

    int begin = p->from_index;
    int end = p->to_index;
    int z;
    // output the array recieved
    for(z = begin; z <end; z++){
        printf("The array recieved is: %d\n", list[z]);
    }
    printf("\n");
    //sort the array from begin to end
    int t;
    int i,j;
    for(i=begin; i<end; i++)
    {
        for(j=begin; j< end-1; j++)
        {
            if(list[j] > list[j+1])
            {
                t = list[j];
                list[j] = list[j+1];
                list[j+1] = t;
            }
        }
    }
    // output the array sorted
    int k;
    for(k = begin; k<end; k++){
            printf("The sorted array: %d\n", list[k]);
    }
    printf("\n");
    pthread_exit(NULL);
}

static void *merger(void *params)
{
    parameters* p = (parameters *)params;

   //MERGE
    int begin = p->from_index;
    int begin2 = p->to_index;

    int index1 = begin,index2 = begin2, index3 = SIZE;
    int count = 0;
    //merge the two half arrays
    while(count<=SIZE){
        if(index1<begin2 && index2<index3){
            if(list[index1]<=list[index2]){
                result[count] = list[index1];
                index1 += 1;
            }
            else{
                result[count] = list[index2];
                index2 += 1;
            }
        }
        else if(index1<begin2){
            result[count] = list[index1];
            index1 += 1;
        }
        else{
            result[count] = list[index2];
            index2 += 1;}
        count += 1;
    }
    // copy the result to the origin array
    int d;
    for(d=0; d<SIZE; d++)
    {
        list[d] = result[d];
    }
    pthread_exit(NULL);
}
