#include <keyboard.h>

int caps=0;
int shift=0;
int ctrl=0;

static char kbd_US[81][2] = {
    
    {'\0', '\0'}, {'|', '\0'}, {'1', '!'}, {'2', '"'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '&'}, {'7', '/'}, {'8', '('}, {'9', ')'}, {'0', '='}, {'\'', '?'}, {'\0', '\0'}, {KEY_BACKSPACE, KEY_BACKSPACE},
    {KEY_TAB, KEY_TAB}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'\0', '\0'}, {'+', '*'}, {KEY_ENTER, KEY_ENTER},
    {KEY_CTRL, '\0'}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {'[', '{'}, {']', '}'}, {'\0', '\0'}, {KEY_SHIFT, '\0'}, {'\\', '|'},
    {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'-', '_'}, {KEY_SHIFT, '\0'},
    {'\0', '\0'}, {'\0', '\0'}, {' ', ' '}, {KEY_CAPS_LOCK, '\0'},
    {'\a', '\a'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'}, {'\0', '\0'},
    {KEY_UP_ARROW, '\0'}, {'\0', '\0'}, {'\0', '\0'}, {KEY_LEFT_ARROW, '\0'}, {'\0', '\0'}, {KEY_RIGHT_ARROW, '\0'}, {'\0', '\0'}, {'\0', '\0'}, {KEY_DOWN_ARROW, '\0'}
};


char map(unsigned char c)  {

    char letter = kbd_US[c][0];
    char release_letter = kbd_US[c - 128][0];

    if (letter == KEY_SHIFT || release_letter == KEY_SHIFT) { // Shift key
    
        shift = (letter == KEY_SHIFT);
    }
    else if (letter == KEY_CTRL || release_letter == KEY_CTRL) { // Ctrl key
        
        ctrl = (letter == KEY_CTRL);
    }
    else if (c <= 58)  {

        switch (letter) {

            case KEY_CAPS_LOCK:  {
                
                caps = !caps;
                return '\0';
            }

            default:    {

                if (ctrl)   {
                    
                    if (letter == 'c' || letter == 'C') {

                        return CTRL_C; // Ctrl+C
                    }
                    
                    if (letter == 'd' || letter == 'D') {

                        return CTRL_D; // Ctrl+D
                    }
                }

                //con esto se verifica cual de los dos elems de cada subarray debe devolver. 
                //Además verifica que solo shift puede modificar el retorno de numeros y sus simbolos, caps no los activa

                return ((caps - shift) ? 
                       ((caps && letter >= '0' && letter <= '9') ? letter : kbd_US[c][1]) : 
                       (shift && letter >= '0' && letter <= '9') ? kbd_US[c][1] : letter);
            }
        }
    }

    return '\0';
}


