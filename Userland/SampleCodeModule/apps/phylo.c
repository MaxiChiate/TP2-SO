#include <Apps/apps.h>


#define LEFT(i) (i+N-1) % N
#define RIGHT(i) (i+1)% N
#define THINKING  0
#define EATING  1
#define HUNGRY  2
#define INIT_PHILOS 5
#define MAX_TIME 5000 //miliseconds
#define PHILOS_BLOCK 5

static uint32_t N = 0;
static uint32_t size = INIT_PHILOS;

int philosopher(int argc, char ** argv);


typedef struct {
    int state;
    int8_t left_fork;
    int8_t right_fork;
    int64_t pid;
} philosopher_t;

philosopher_t * philos;

int8_t mutex;
int8_t update_mutex;

static void think(int i) {
    sleep(rand() % MAX_TIME);     // Random time
}

static void eat(int i) {
    sleep(rand() % MAX_TIME);       // Random time
}


static void add_philosopher(){
    down(update_mutex);

        if(N> size){
            philos= dum_realloc(philos,size,size+PHILOS_BLOCK);
            size+=PHILOS_BLOCK;
        }
        philos[N].state = THINKING;
            
        if(N>0){
            down(mutex);
                philos[N].left_fork= philos[LEFT(N)].left_fork;
                philos[N].right_fork= new_sem(1);
                philos[RIGHT(N)].right_fork = philos[N].right_fork;
            up(mutex);
        }
        else{
            down(mutex);
                philos[N].left_fork= new_sem(1);
                philos[N].right_fork= new_sem(1);
            up(mutex);
        }
        char number[4];
        itoa(N, number);
        char *argv[] = {number, NULL};
        philos[N].pid= spawn_process(&philosopher,1,argv,1,true);
        N++;

    up(update_mutex);
}

int philosopher(int argc, char ** argv) {


}

void phylo(int argc, char ** argv){

    philos = malloc(sizeof(philosopher_t)* INIT_PHILOS);

    mutex= new_sem(1);
    update_mutex= new_sem(1);

    for(int i=0;i<INIT_PHILOS;i++){
        add_philosopher();
    }

    suicide();
}
