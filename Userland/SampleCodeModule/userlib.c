#include <userlib.h>

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
////asume que el string solo tiene dos palabras separadas por espacio, donde quiero devolver las 2;
int stringTrimmerBySpace(char* origString, char* aux1, char* aux2) {
    aux2[0] = '\0';
    int i = 0, j = 0, k = 0;
    while (origString[i] != ' ' && origString[i] != '\0') {
        aux1[j++] = origString[i++];
    }
    aux1[j] = '\0';
    if (origString[i] != '\0') {
        i++; // //salteo el espacio
        while (origString[i] != '\0') {
            aux2[k++] = origString[i++];
        }
        aux2[k] = '\0';
    }

    return k; // devuelvo longitud de aux2

}

unsigned int strLength(const char * str) {
    int i=0;
    while(str[i++]!='\0');
    return i-1;
}

unsigned int strEquals(char * s1, char * s2)  {
    int i=0;
    while(s1[i]!='\0' && s2[i]!='\0') {
        if(s1[i]!=s2[i])
            return 0;
        ++i;
    }
    return s1[i] == '\0' && s2[i] == '\0';
}

int isDigit(unsigned char c) {
    return digits[c];
}

int isUpper(unsigned char c)    {
    return capitals[c];
}

unsigned int strToUint(char * s) {
    
    unsigned int ans = 0;
    int i=0;
    while(isDigit((unsigned char)s[i])) {
     
     putChar(s[i]);
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