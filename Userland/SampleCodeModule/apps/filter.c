#include <apps/apps.h>


void filter(int argc, char ** argv) {

    char buff[BUFF_SIZE] = {'\0'};
    char ans[BUFF_SIZE] = {'\0'};

    int j=0;
    int length = read_into_buffer(buff, BUFF_SIZE);

    for(int i = 0; i < length; i++)  {

        ans[j++] = (isVocal(buff[i]) || buff[i] == KEY_ENTER || buff[i] == KEY_TAB) 
            ?  buff[i] : ' ';
    }
    ans[j] = '\0';

    putEnter();
    print(ans);
    suicide();

}