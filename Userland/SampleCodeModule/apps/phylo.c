#include <Apps/apps.h>


#define LEFT(i) (i+N-1) % N
#define RIGHT(i) (i+1)% N
#define THINKING  0
#define EATING  1
#define HUNGRY  2
#define KILLED 3
#define INIT_PHILOS 5
#define TIME_THINK 1000 //miliseconds
#define TIME_EAT 500 //miliseconds
#define PHILOS_BLOCK 5
#define ADD_CHAR 'a'
#define DEL_CHAR 'r'
#define EXIT_CHAR 'q'

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
    sleep(TIME_THINK);
}

static void eat(int i) {
    sleep(TIME_EAT);
}

static int can_eat(int i) {

    return philos[LEFT(i)].state != EATING && philos[RIGHT(i)].state != EATING;
}
static void return_forks(int i) {
    if(philos[i].state== KILLED){
        return;
    }
    philos[i].state = THINKING; 
    up(philos[i].left_fork);
    up(philos[i].right_fork);
}
static void take_forks(int i) {
    if(philos[i].state== KILLED){
        return;
    }
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
        spawn_process((int64_t) &philosopher,1,argv,1,true);
        N++;

    up(update_mutex);
}

static void remove_philosopher(){

    down(update_mutex);
        N--;
        kill_sem(philos[N].left_fork);
        kill_sem(philos[N].right_fork);
        philos[N].state=KILLED;

    up(update_mutex);
}


void philosopher(int argc, char ** argv) {
    int i = atoi(argv[0]);
    philos[i].pid= current_pid();
    bool flag=true;
    while (flag) {
        think(i);
        down(mutex);
        take_forks(i);
        up(mutex);
        if (philos[i].state == EATING) {
            down(update_mutex);
                show_state(); 
            up(update_mutex);
            eat(i);
            down(mutex);
                return_forks(i);
            up(mutex);
        }
        else if (philos[i].state== KILLED){
            flag= false;
        }
    }
    suicide();

}

void phylo(int argc, char ** argv){
    argv++;

    philos = malloc(sizeof(philosopher_t)* INIT_PHILOS);

    mutex= new_sem(1);
    update_mutex= new_sem(1);

    for(int i=0;i<INIT_PHILOS;i++){
        add_philosopher();
    }
    char c;
    char last_c= '0';
    bool flag=true;
    while (flag)
    {
        getChar(&c);

        if(c == ADD_CHAR && c!=last_c){
            add_philosopher();
            print("Add Philopher\n");
        }
        else if(c == DEL_CHAR && c!=last_c){
            
            if(N> INIT_PHILOS){
                remove_philosopher();
                print("Remove Philopher\n");
            }
            else{
                print("Reached minimun amount of philosophers\n");
            }
        }
        else if(last_c== EXIT_CHAR && c!=last_c){
            print("Exit phylo\n");
            flag=false;
        }
        last_c=c;

    }

    while(N>0){
        remove_philosopher();
    }

    sleep(TIME_EAT*3);
    kill_sem(mutex);
    kill_sem(update_mutex);
    free(philos);
    putEnter();
    suicide();
}
