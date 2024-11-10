#ifndef DEFS_H
#define DEFS_H
    
    #include <bool.h>

    #ifndef NULL
    
        #define NULL ( (void *) 0x0000)
        
    #endif

    #ifndef EOF

        #define EOF (-1)

    #endif


//  [0-9]
    extern unsigned char digits[256];

//  [A-Z]
    extern unsigned char capitals[256];

//  [aeiouAEIOU]
    extern unsigned char vocals[256];

#endif
