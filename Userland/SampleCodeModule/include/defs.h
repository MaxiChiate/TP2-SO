#ifndef DEFS_H
#define DEFS_H

        // https://www.onlinegdb.com/online_c_debugger
// Script util para armar estas matrices: (ejemplo @capitals)

    // #include <stdio.h>

    // int main()
    // {
    //     printf("{");
        
    //     for(int i=0; i<257; ++i)    {
    //         printf(" /*");
    //         if(i<100)
    //             printf("0");
    //         if(i<10)
    //             printf("0");
    //         printf("%d*/%d, ",i, i >= 'A' && i <= 'Z');
    //     }
        
    //     printf("}");

    //     return 0;
    // }

    
//  [0-9]
    extern unsigned char digits[257];

//  [A-Z]
    extern unsigned char capitals[257];

#endif