#include <pthread.h>
#include <functional>
#include <stdio.h>
#include <time.h>
#include "stamp.h"

struct multi {
    std::function<void(int,int)> lamb;
    int low1;
    int high1;
    int stride1;
    int stride2;
    int size;
};

struct vect{

    std::function<void(int)> lamb;
    int low;
    int high;
    int stride;
};

void * vect_wrap(void* func){
    struct vect * para  = (struct vect*)(func);

    for(int i = para->low; i<para->high ;i+= para->stride){
        (para->lamb)(i);
    }
    return NULL;
}

void *wrapper(void * func){
    std::function<void()> *out  = reinterpret_cast<std::function<void()>*>(func);
    (*out)();
    return NULL;
}

void* mmulti_wrap(void * data){
    struct multi * p1 = (struct multi*)(data);

    for(int i = p1->low1; i < p1->high1; i += p1->stride1){
        for(int j = 0; j < p1->size; j += p1->stride2){ 
            ((p1->lamb))(i,j);
        }
    }
    return NULL;
}

void execute_tuple(std::function<void()> &&lambda1, std::function<void()> &&lambda2){
    pthread_t p1,p2;
    clock_t start,end;
    start = clock();

    std::function<void()>* func1 = new std::function<void()>(lambda1);
    std::function<void()>* func2 = new std::function<void()>(lambda2);
    
    if(pthread_create(&p1,NULL,wrapper,func1)){
        printf("Error in making thread.\n");
        return;
    }

    if(pthread_join(p1,NULL) != 0){
        printf("Error joining the thread\n");
    }

    if(pthread_create(&p2,NULL,wrapper,func2)){
        printf("Error in making thread.\n");
        return;
    }

    if(pthread_join(p2,NULL) != 0){
        printf("Error joining the thread\n");
    }

    end = clock();
    double duration = (double)(end - start)/CLOCKS_PER_SEC;
    printf("StaMp Statistics: Threads = 2, Parallel execution time = %lf seconds\n",duration);
    return;
}

void parallel_for(int low1, int high1, int stride1, int low2, int high2, int stride2,
std::function<void(int, int)> &&lambda, int numThreads){
    clock_t start,end;
    start = clock();
    
    std::function<void(int,int)> func = lambda;
    pthread_t threads[numThreads];

    int part1 = (high1 - low1)/numThreads;
    int rem1 = (high1 - low1)%numThreads;
    int size1 = low1;
    

    struct multi* thread_param[numThreads];
    for(int i = 0; i < numThreads; i++){
        thread_param[i] = ((struct multi*)  malloc(sizeof(struct multi)));
    }

     
    for(int i = 0; i < numThreads; i++){

        thread_param[i]->lamb = func;

        thread_param[i]->low1 = size1;
        size1 += part1;
        if(i < rem1){ size1++;}
        thread_param[i]->high1 = size1;

        thread_param[i]->stride1 = stride1;
        thread_param[i]->stride2 = stride2;

        thread_param[i]->size = high2;
        void * data = (void *) thread_param[i];
        pthread_create(&threads[i],NULL,mmulti_wrap,data);        
    }

    for(int i = 0; i < numThreads; i++){
        pthread_join(threads[i],NULL);
    }

    end = clock();
    double duration = (double)(end - start)/CLOCKS_PER_SEC;
    printf("StaMp Statistics: Threads = %d, Parallel execution time = %lf seconds\n", numThreads,duration);
    return;
}

void parallel_for(int low, int high, int stride, std::function<void(int)> &&lambda, int numThreads){
    clock_t start,end;
    start = clock();
    std::function<void(int)> func = lambda;
    pthread_t threads[numThreads];

    int part = (high - low)/numThreads;
    int rem = (high - low)%numThreads;

    int size = low;
    struct vect * para[numThreads];
    for(int i = 0; i < numThreads; i++){
        para[i] = (struct vect*) malloc(sizeof(vect));
    }

    for(int i = 0; i< numThreads; i++){

        para[i]->lamb = func;
        para[i]->low = size;

        size = part + size;

        if(i < rem){size++;}

        para[i]->high = size;
        para[i]->stride = stride;

        void * ptr = (void*) para[i];
        pthread_create(&threads[i],NULL,vect_wrap,ptr);
    }

    for(int i = 0; i < numThreads; i++){
        pthread_join(threads[i],NULL);
    }
    
    end = clock();
    double duration = (double)(end - start)/CLOCKS_PER_SEC;
    printf("StaMp Statistics: Threads = %d, Parallel execution time = %lf seconds\n", numThreads,duration);
    return;
}

void parallel_for(int high,  std::function<void(int)> &&lambda, int numThreads){
    clock_t start,end;
    start = clock();

    std::function<void(int)> func = lambda;
    pthread_t threads[numThreads];

    int part = (high)/numThreads;
    int rem = (high)%numThreads;

    int size = 0;
    
    struct vect * para[numThreads];
    for(int i = 0; i < numThreads; i++){
        para[i] = (struct vect*) malloc(sizeof(vect));
        
    }

    for(int i = 0; i< numThreads; i++){

        para[i]->lamb = func;
        para[i]->low = size;

        size = part + size;

        if(i < rem){size++;}

        para[i]->high = size;

        para[i]->stride = 1;

        void * ptr = (void*) para[i];

        pthread_create(&threads[i],NULL,vect_wrap,ptr);
    }

    for(int i = 0; i < numThreads; i++){
        pthread_join(threads[i],NULL);
    }

    end = clock();
    double duration = (double)(end - start)/CLOCKS_PER_SEC;
    printf("StaMp Statistics: Threads = %d, Parallel execution time = %lf seconds\n", numThreads,duration);
    return;
}

void parallel_for(int high1, int high2,  std::function<void(int, int)>   &&lambda,
int numThreads){
    clock_t start,end;
    start = clock();

    std::function<void(int,int)> func = lambda;
    pthread_t threads[numThreads];

    int part1 = (high1)/numThreads;
    int rem1 = (high1)%numThreads;
    int size1 = 0;
    

    struct multi* thread_param[numThreads];
    for(int i = 0; i < numThreads; i++){
        thread_param[i] = ((struct multi*)  malloc(sizeof(struct multi)));
    }

     
    for(int i = 0; i < numThreads; i++){

        thread_param[i]->lamb = func;

        thread_param[i]->low1 = size1;
        size1 += part1;
        if(i < rem1){ size1++;}
        thread_param[i]->high1 = size1;

        thread_param[i]->stride1 = 1;
        thread_param[i]->stride2 = 1;

        thread_param[i]->size = high2;
        void * data = (void *) thread_param[i];
        pthread_create(&threads[i],NULL,mmulti_wrap,data);       
    }

    for(int i = 0; i < numThreads; i++){
        pthread_join(threads[i],NULL);
    }

    end = clock();
    double duration = (double)(end - start)/CLOCKS_PER_SEC;
    printf("StaMp Statistics: Threads = %d, Parallel execution time = %lf seconds\n", numThreads,duration);
    return;
}