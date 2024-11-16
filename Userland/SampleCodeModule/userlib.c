#include <userlib.h>

static int assert_id = -1;

void assert(bool comparation, const char * optional_msg)    {

    assert_id++;

    if(!comparation)    {

        char buff[5] = {'\0'};
        itoa(assert_id, buff);
        print("Error assertion ");
        print(buff);
        print(". ");
        
        if(optional_msg) {

            print(optional_msg);
        }
        
        putEnter();

        suicide();
    }
}


void print(const char * str)  {

    print2(str, strLength(str));
}

void print2(const char * str, unsigned int dim)   {

    int64_t args[] = {(int64_t) str, (int64_t) strLength(str)};

    _int80(SYS_STANDARD_WRITE, args);
}

void puts(const char * str)   {

    print(str);
    putEnter();
}

void print_ps(ps_t * ps) {

    print("sp: ");
    printUinteger((unsigned int) ps->sp);
    print("\t");

    print("bp: ");
    printUinteger((unsigned int) ps->bp);
    print("\t");

    print("id: ");
    printUinteger((unsigned int) ps->id);
    print("\t");

    print("parent: ");
    printUinteger((unsigned int) ps->parent_id);
    print("\t");    
                    
    print("state: ");
    putChar((char) ps->state);
    print("\t");    

    print("fg: ");
    printUinteger((unsigned int) ps->foreground);
    print("\t");

    print("prio: ");
    printUinteger((unsigned int) ps->priority);
    print("\t");

    print("name: ");
    print((char *) ps->name);
    print("\n");

    print("\n");
}

void print_all_ps(ps_t ** to_print) {

    for (int i = 0; to_print[i] != NULL; i++) {
        print_ps(to_print[i]);
    }
}

int getChar() {

    char s[1];

    int64_t args[] = {(int64_t) s, 1};

    int read = (int) _int80(SYS_STANDARD_READ, args);

    return read > 0 ? s[0] : EOF;
}

int scan(int fd, char * buffer)  {

    int64_t args[] = {fd, (int64_t) buffer};

    return (int) _int80(SYS_READ_ALL, args);
}

int read_into_buffer(char * buff, unsigned int len)  {

    int i = 0;
    char c;

    while (true) {

        c = getChar();

        if (c == EOF || c == '\0') {
            buff[i] = '\0';
            return i;
        }

        if (c == KEY_BACKSPACE) {
            if (i != 0) {
                buff[--i] = '\0';
            }
        } else if (i < len - 1) {
            buff[i++] = c;
        }
    }
}


void putChar(char c)    {

    char s [2] = {c, '\0'};

    print2(s, 1);
}

char getAndPrintChar()  {

    char c;
    putChar(c = getChar());
    return c;
}

void clear()    {

    _int80(SYS_CLEAR, NULL);
}

void time_getter()  {

    _int80(SYS_TIME, NULL);
}

void sleep(int64_t ms){
    
    int64_t args[] = {ms}; 
    _int80(SYS_SLEEP, args);
}

void putEnter() {
    putChar(KEY_ENTER);
}

void putnEnters(unsigned int n)    {

    while(n-->0)  
        putEnter();
    
}

void putBackSpace() {
    putChar('\b');
}

void putTab()   {
    putChar(KEY_TAB);
}
char* stringNormalizer(char* origString, unsigned int strlen){
    int i=0, j=0;
    while (origString[i]!='\0' && j < strlen - 1){
        if (isUpper(origString[i])){
            origString[j++]=origString[i++]+32;
        } else if (origString[i]==' '){
            origString[j++]=' ';
            while (origString[i]==' ')
                i++;
        } else
            origString[j++]=origString[i++];
    }
    origString[j]='\0';
    return origString;
}

static int get_arg( char * origString, char * arg, unsigned int max_dim)    {
 
    int i=0, k=0;

    if (origString[i] != '\0' && arg != NULL)   {

        while (origString[i] != ' ' && origString[i] != '\0')   {

            if(i == max_dim)    {

                return -1;
            }

            arg[k++] = origString[i++];
        }

        arg[k] = '\0';

    }
    return i;
}


int stringTrimmerBySpace( char * origString, char * function_name, char ** argv, unsigned int max_dim) {
    
    int i = 0, argc = 0;

    if(origString != NULL && function_name != NULL)     {

        while (origString[i] != ' ' && origString[i] != '\0') {
         
            if(i == max_dim)    {

                return -1;
            }

            function_name[i] = origString[i];
            i++;
        }

        function_name[i] = '\0';
        
        if (argv != NULL)   {

            i += (origString[i] == ' '); // Si hay espacio, lo saco

            int arg_lenght;
            while( (arg_lenght = get_arg(origString + i, argv[argc], max_dim)) )  { 

                if(arg_lenght < 0 || arg_lenght >= max_dim) {

                    return -1;
                }

                i += arg_lenght;
                i += (origString[i] == ' '); // Por si hay espacio

                argc++;
            }

            if(origString[i] != '\0')  { 
        // No llego al final entonces quiere hacer overflow en el buffer, chau pichi    
                return -1;
            }
        }
    }

    return argc;
}

unsigned int strLength(const char * str) {
    int i=0;
    while(str[i++]!='\0');
    return i-1;
}

unsigned int strEquals( char * s1,  char * s2)  {
    int i=0;
    while(s1[i]!='\0' && s2[i]!='\0') {
        if(s1[i]!=s2[i])
            return 0;
        ++i;
    }
    return s1[i] == '\0' && s2[i] == '\0';
}

bool isDigit(unsigned char c) {

    return (bool) digits[c];
}

bool isUpper(unsigned char c)    {

    return (bool) capitals[c];
}

bool isVocal(unsigned char c)   {

    return (bool) vocals[c];
}

unsigned int strToUint( char * s) {
    
    unsigned int ans = 0;
    int i=0;
    while(isDigit((unsigned char)s[i])) {
     
    //  putChar(s[i]);
        ans = ans*10 + s[i++]-0x30;

    }

// si s[i]!=0 entonces no lo recorrio entero
// si i == 0 entonces es el string vacio
    if(s[i]||!i)    {
        
        print("Error en strToUint: El string ingresado no representa un numero valido\n");
        return (unsigned int) -1;
    }
    
    return ans;
}

int getUlongDigits(unsigned long n )    {

// n=0 es un caso especial pues 0 = 00 = 000...0, etc...
    if(!n)  return 1;

    int digits=0;

    do  
        digits++;

    while(n/=10);
        
    return digits;
}

int getUintDigits(unsigned int n)   {
    
    return getUlongDigits((unsigned long) n);
}

void uLongToString(unsigned long input, char * ans, int digits)   {

// Se asume que ans tiene la longitud necesaria para que entre la
// cantidad de digitos de input y el '\0' al final.
    ans[digits] = '\0';
    
    int i=digits-1;

    do  
        ans[i--] = (input % 10) + 0x30;  

    while(input/=10);

}

void uIntToString(unsigned int input, char * ans, int digits)   {
    
    uLongToString((unsigned long) input, ans, digits);
}


void printUinteger(unsigned int n)   {

    int digits = getUintDigits(n);
    char s[digits+1];
    uIntToString(n, s, digits);
    print(s);
}

void printUlong(unsigned long n)   {

    int digits = getUlongDigits(n);
    char s[digits+1];
    uLongToString(n, s, digits);
    print(s);
}

unsigned long pow(unsigned long x, unsigned long n) {
    
// Se hace uso de la potenciacion por cuadratura.

// Source: 
//      https://en.wikipedia.org/wiki/Exponentiation_by_squaring


// x^0 = 1, incluso si x=0.
    if (!n)  

        return 1;

// n impar.     
    if (n%2) 
        
        return x * pow(x * x, (n - 1) / 2);

// n par.
        return pow(x * x, n / 2);

}

int abs(int value){
    if(value < 0){
        return -value;
    }
    return value;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}

void strcopy(char* dest, char* string){
    if(dest == NULL){
        return;
    }
    int i;
    for(i  = 0; string[i] != '\0'; i++){
        dest[i] = string[i];
    }
    dest[i] = '\0';
}

void itoa(int value, char* buff){
    int i = 0;
    int val = abs(value);

    if(val == 0){
        buff[i++] = '0';
    }

    while(val != 0){
        int resto = val % 10;
        if(resto < 10){
            buff[i++] = resto + '0';
        }else {
            buff[i++] = resto + 'A' - 10;
        }
        val /= 10;
    }

    if (value < 0) {
        buff[i++] = '-';
    }
    int dim = i;

    int j = 0;
    i -= 1;
    char aux;
    while(j < i){
        aux = buff[j];
        buff[j] = buff[i];
        buff[i] = aux;
        j++;
        i--;
    }
    buff[dim] = 0;
}

int atoi(char* value){
    int is_negative = 0;
    int aux = 0;
    if(value[0] == '-'){
        is_negative = 1;
    }
    for (int i = is_negative; value[i] != '\0'; ++i){
        if(value[i] >= '0' && value[i] <= '9'){
            aux = aux * 10 + value[i] - '0';
        }
    }

    if(is_negative){
        aux = -aux;
    }
    return aux;
}
