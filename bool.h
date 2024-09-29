#ifndef BOOL_H
#define BOOL_H

    #define true 1 
    #define false !true
	
  typedef unsigned char bool;
    
	bool bool_and(bool a, bool b);

	bool bool_or(bool a, bool b);

	bool bool_not(bool a);

	bool bool_xor(bool a, bool b);

#endif
