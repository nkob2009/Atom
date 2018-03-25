#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

/* --------------------------------- DEFS ---------------------------------- */

struct thdata {
    char                letter;
    pthread_t           th;
    int 		t_num;
    int			seed;
};

/* ------------------------------------------------------------------------- */

int cnt = -1;
int lng = 0;
char                hello[128] = "Hello World!\n";
int result[12];
struct timeval tv = {0};
struct timeval tvr[12];

/* ------------------------------- FUNCTIONS ------------------------------- */

/*****************************************************************************

 FUNCTION    : void *thread_function (void *thdata)

 DESCRIPTION : Thread function.

               * Argument
                 void *

               * Return
                 void *

 ATTENTION   :

 *****************************************************************************/
void *thread_function(void *thdata)
{

    struct thdata       *priv = (struct thdata *)thdata;
    int dur;
    int order;

    srand(priv->seed);
//    usleep(priv->t_num*100);
//    usleep(dur = ((float)rand())/10000);
    usleep(dur = ((float)rand())/100000);
//    putchar(hello[(int)__sync_add_and_fetch(&cnt,1)]);
    order = __sync_add_and_fetch(&cnt,1);
//    printf("(%d:%d:%d)-> ",order,priv->t_num,dur);
    result[order] = priv->t_num;
    gettimeofday(&tv, NULL);
    tvr[order]=tv;
    /* done */
    return (void *) NULL;

}/* Main 

 *****************************************************************************/
int main (void)
{

    int                 rtn, i;
    struct thdata       *thdata;
    char                hello[128] = "Hello World!\n";


    /* initialize thread data */
    thdata = calloc(sizeof(struct thdata), strlen(hello));
    if (thdata == NULL) {
        perror("calloc()");
        exit(EXIT_FAILURE);
    }
    srand((unsigned)time(NULL));
//    for (i = 0; i < strlen(hello); i++) {
    for (i = strlen(hello);i>0; i--) {
        thdata[i].letter = hello[i];
	thdata[i].t_num = i;
	thdata[i].seed = rand();
    }

    for (i=0;i<strlen(hello);i++){
        rtn = pthread_create(&thdata[i].th, NULL, thread_function, (void *) (&thdata[i]));
        if (rtn != 0) {
            fprintf(stderr, "pthread_create() #%0d failed for %d.", i, rtn);
            exit(EXIT_FAILURE);
        }
    }

    sleep(1);

    for (i=0;i<strlen(hello);i++){
      printf("%d:%d:%d ; ",i,result[i],(int)tvr[i].tv_usec);
    }

    printf("cnt = %d\n",cnt);

    /* join */
    for (i = 0; i < strlen(hello); i++) {
        pthread_join(thdata[i].th, NULL);
    }

    free(thdata);
    exit(EXIT_SUCCESS);

}
/* ------------------------------------------------------------------------- */
		
