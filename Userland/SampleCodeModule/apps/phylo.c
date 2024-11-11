#include <Apps/apps.h>


#define LEFT(i) (i+N-1) % N
#define RIGHT(i) (i+1)% N
#define THINKING  0
#define EATING  1
#define HUNGRY  2
#define INIT_PHILOS 5
#define TIME_EAT 3000 //miliseconds
#define TIME_THINK 500 //miliseconds
#define PHILOS_BLOCK 5

static uint32_t N = 0;
static uint32_t size = INIT_PHILOS;

void philosopher(int argc, char ** argv);


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
    sleep(rand() % TIME_EAT);
}

static void eat(int i) {
    sleep(rand() % TIME_THINK);
}

static int can_eat(int i) {

    return philos[LEFT(i)].state != EATING && philos[RIGHT(i)].state != EATING;
}
static void return_forks(int i) {
    philos[i].state = THINKING; 
    up(philos[i].left_fork);
    up(philos[i].right_fork);
}
static void take_forks(int i) {
    philos[i].state = HUNGRY;
    if (can_eat(i)) {
        philos[i].state = EATING;
        down(philos[i].left_fork);
        down(philos[i].right_fork);
    }
}
static void show_state() {
    for (int i = 0; i < N; i++) {
        if (philos[i].state == EATING) { 
            putChar('E');
        } else {
            putChar('.');
        }
        putChar(' ');
    }
    print("\n");
}

static void add_philosopher(){
    down(update_mutex);

        if(N> size){
            philos= dum_realloc(philos,size,size+PHILOS_BLOCK);
            size+=PHILOS_BLOCK;
        }
        philos[N].state = THINKING;
            
        if(N>0){
            philos[N].left_fork= new_sem(1);
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
        philos[N].pid= spawn_process((int64_t) &philosopher,1,argv,1,true);
        N++;

    up(update_mutex);
}

void philosopher(int argc, char ** argv) {

    int i = atoi(argv[0]);

    while (1) {
        think(i);
        down(mutex);
        take_forks(i);
        up(mutex);
        if (philos[i].state == EATING) {
            show_state();
            eat(i);
            down(mutex);
            return_forks(i);
            up(mutex);
        }
    }

}

void phylo(int argc, char ** argv){

    philos = malloc(sizeof(philosopher_t)* INIT_PHILOS);

    mutex= new_sem(1);
    update_mutex= new_sem(1);

    for(int i=0;i<INIT_PHILOS;i++){
        add_philosopher();
    }

    while (1)
    {
    }
    

    suicide();
}
