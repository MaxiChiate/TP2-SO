#include <userlib.h>

void print(char * str)  {

    print2(str, strLength(str));
}

void print2(char * str, unsigned int dim)   {

    int64_t args[] = { STRING, (int64_t) str, (int64_t) dim };

    _int80(SYS_READ, args);

    args[0] = STDOUT;
    args[1] = (int64_t) dim;

    _int80(SYS_WRITE, args);
}

unsigned char getChar() {

    int64_t arg = RETURN_CHAR;

    return (unsigned char) _int80(SYS_WRITE, &arg);
}

void putChar(char c)    {

    char s [] = {c, '\0'};

    print2(s, 1);
}

char getAndPrintChar()  {

    int64_t arg = RETURNANDSTDOUT_CHAR;

    return (char) _int80(SYS_WRITE, &arg);
}

void clear()    {

    _int80(SYS_CLEAR, NULL);
}

void time_getter()  {

    _int80(SYS_TIME, NULL);
}

void putEnter() {
    putChar('\n');
}

void putnEnters(unsigned int n)    {

    while(n-->0)  
        putEnter();
    
}

void putBackSpace() {
    putChar('\b');
}

void putTab()   {
    putChar('\t');
}
char* stringNormalizer(char* origString){
    int i=0, j=0;
    while (origString[i]!='\0'){
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

static int get_arg( char * origString, char * arg)    {
 
    int i=0, k=0;

    if (origString[i] != '\0' && arg != NULL)   {

        while (origString[i] != ' ' && origString[i] != '\0')   {

            arg[k++] = origString[i++];
        }

        arg[k] = '\0';

    }
    return i;
}


int stringTrimmerBySpace( char * origString, char * function_name, char ** argv) {
    
    int i = 0, argc = 0;

    if(origString != NULL && function_name != NULL)     {

        while (origString[i] != ' ' && origString[i] != '\0') {
         
            function_name[i] = origString[i];
            i++;
        }

        function_name[i] = '\0';
        
        if (argv != NULL)   {

            i += (origString[i] == ' '); // Si hay espacio, lo saco

            int arg_lenght;
            while( (arg_lenght = get_arg(origString + i, argv[argc])) )  { 

                if(arg_lenght > 14 + 1) {

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

unsigned int strLength( char * str) {
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

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}