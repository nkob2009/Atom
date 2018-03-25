#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

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

    srand(priv->seed);
//    usleep(priv->t_num*100);
    usleep(dur = ((float)rand())/10000);
//    putchar(hello[(int)__sync_add_and_fetch(&cnt,1)]);
    printf("%d(%d)-> ",priv->t_num,dur);

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
        rtn = pthread_create(&thdata[i].th, NULL, thread_function, (void *) (&thdata[i]));
        if (rtn != 0) {
            fprintf(stderr, "pthread_create() #%0d failed for %d.", i, rtn);
            exit(EXIT_FAILURE);
        }
    }


    sleep(1);
    printf("cnt = %d\n",cnt);

    /* join */
    for (i = 0; i < strlen(hello); i++) {
        pthread_join(thdata[i].th, NULL);
    }

    free(thdata);
    exit(EXIT_SUCCESS);

}
/* ------------------------------------------------------------------------- */
		
