/*
 * POSIX Real Time Example
 * using a single pthread as RT thread
 */
#include <inttypes.h>
#include <limits.h>     //for PTHREAD_STACK_MIN
#include <pthread.h>    //to create and manage POSIX threads
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>   //for mlockall()
#include <unistd.h>     //for sleep() and usleep()
#include <time.h>       //to mesure execution duration
#include <math.h>


float calculateSD(float data[]) {
    float sum = 0.0, mean, standardDeviation = 0.0;
    int i;
    for(i=0; i<10000; ++i){
      //  printf(" %f ", data[i]);
        sum += data[i];
      }
    mean = sum/10000.0;
    for(i=0; i<10000; ++i)
        standardDeviation += pow(data[i] - mean, 2);
    return sqrt(standardDeviation/10000);
}

float times_us[10000];

void wasting_time(int thread_displayed, int delay_us){

    long   start_us; // Milliseconds
    long   end_us; // Milliseconds
    struct timespec start;
    struct timespec end;
    float  cumulative_duration_us = 0;
    float  average_duration_us;
    float  standard_deviation;

    int   i;
    int   count = 0;

    for(i=0;i<10000;i++){
        clock_gettime(CLOCK_REALTIME, &start);
        usleep(delay_us);
        clock_gettime(CLOCK_REALTIME, &end);
        start_us    = start.tv_sec*1.0e9 + start.tv_nsec/1.0e0;
        end_us      = end.tv_sec*1.0e9   + end.tv_nsec/1.0e0;
        times_us[i] = (float) ((end_us - start_us) / 1000.0);
        cumulative_duration_us += times_us[i];
        //printf("start thread %d (ns):    %ld\n", number_displayed, duration_us);
    }
    average_duration_us = cumulative_duration_us / 10000.0;
    standard_deviation  = calculateSD(times_us);
    printf("\ncumulative_duration_us thread %d (us):    %f\n", thread_displayed, cumulative_duration_us);
    printf("average_duration_us    thread %d (us):    %f\n", thread_displayed, average_duration_us);
    printf("standard_deviation     thread %d (us):    %f\n", thread_displayed, standard_deviation);
}

void *thread_func1(void *data){
      /*  long   start_ms; // Milliseconds
        long   end_ms; // Milliseconds
        struct timespec start;
        struct timespec end;
        long   duration_ms;
        clock_gettime(CLOCK_REALTIME, &start);*/

        wasting_time(1, 10000);

      /*  clock_gettime(CLOCK_REALTIME, &end);
        start_ms = start.tv_sec*1.0e3 + start.tv_nsec/1.0e6;
        end_ms   = end.tv_sec*1.0e3   + end.tv_nsec/1.0e6;
        duration_ms = end_ms - start_ms;
        printf("start thread 1 (ms):    %ld\nend thread 1 (ms):       %ld\nthread 1 (ms):  %ld\n", start_ms, end_ms, duration_ms);
*/
        return NULL;
}

void *thread_func2(void *data){
      /*  long   start_ms; // Milliseconds
        long   end_ms; // Milliseconds
        struct timespec start;
        struct timespec end;
        long   duration_ms;
        clock_gettime(CLOCK_REALTIME, &start);*/

//        wasting_time(2, 1);

      /*  clock_gettime(CLOCK_REALTIME, &end);
        start_ms = start.tv_sec*1.0e3 + start.tv_nsec/1.0e6;
        end_ms   = end.tv_sec*1.0e3   + end.tv_nsec/1.0e6;
        duration_ms = end_ms - start_ms;
        printf("start thread 2 (ms):    %ld\nend thread 2        (ms):       %ld\nthread 2              (ms):  %ld\n", start_ms, end_ms, duration_ms);
*/
        return NULL;
}

int main(int argc, char* argv[]){
        struct sched_param param1;
        struct sched_param param2;
        pthread_attr_t attr1;
        pthread_attr_t attr2;
        pthread_t thread1;
        pthread_t thread2;
        int ret;

        /* Lock memory */
        if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
                printf("mlockall failed: %m\n");
                exit(-2);
        }


        /* Initialize pthread attributes (default values) */
        ret = pthread_attr_init(&attr1);
        if (ret) {
                printf("init pthread attributes failed\n");
                goto out;
        }

        /* Set a specific stack size  */
        ret = pthread_attr_setstacksize(&attr1, PTHREAD_STACK_MIN);
        if (ret) {
            printf("pthread setstacksize failed\n");
            goto out;
        }

        /* Set scheduler policy and priority of pthread */
        ret = pthread_attr_setschedpolicy(&attr1, SCHED_FIFO);
        if (ret) {
                printf("pthread setschedpolicy failed\n");
                goto out;
        }
        param1.sched_priority = 99;
        ret = pthread_attr_setschedparam(&attr1, &param1);
        if (ret) {
                printf("pthread setschedparam failed\n");
                goto out;
        }
        /* Use scheduling parameters of attr */
        ret = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
        if (ret) {
                printf("pthread setinheritsched failed\n");
                goto out;
        }


        /* Initialize pthread attributes (default values) */
        ret = pthread_attr_init(&attr2);
        if (ret) {
                printf("init pthread attributes failed\n");
                goto out;
        }

        /* Set a specific stack size  */
        ret = pthread_attr_setstacksize(&attr2, PTHREAD_STACK_MIN);
        if (ret) {
            printf("pthread setstacksize failed\n");
            goto out;
        }

        /* Set scheduler policy and priority of pthread */
        ret = pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
        if (ret) {
                printf("pthread setschedpolicy failed\n");
                goto out;
        }
        param2.sched_priority = 99;
        ret = pthread_attr_setschedparam(&attr2, &param2);
        if (ret) {
                printf("pthread setschedparam failed\n");
                goto out;
        }
        /* Use scheduling parameters of attr */
        ret = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
        if (ret) {
                printf("pthread setinheritsched failed\n");
                goto out;
        }


        /* Create a pthread with specified attributes */
        ret = pthread_create(&thread1, &attr1, thread_func1, NULL);
        if (ret) {
                printf("create pthread failed\n");
                goto out;
        }
    /*    ret = pthread_create(&thread2, &attr2, thread_func2, NULL);
        if (ret) {
                printf("create pthread failed\n");
                goto out;
        }*/

        /* Join the thread and wait until it is done */
        ret = pthread_join(thread1, NULL);
        if (ret)
                printf("join pthread failed: %m\n");
    /*    ret = pthread_join(thread2, NULL);
        if (ret)
                printf("join pthread failed: %m\n");*/

out:
        return ret;
}
