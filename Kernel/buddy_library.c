#include <buddy_library.h>

int ceil_custom(double x) {
    int integer_part = (int)x;
    
    // Si x ya es un número entero, simplemente regresamos la parte entera.
    if (x == (double)integer_part) {
        return integer_part;
    }

    // Si x es positivo y tiene parte decimal, redondeamos al entero siguiente.
    return integer_part + 1;
}


int get_order(int x) {
    if (x <= 0) {
        return -1; // Indica un error para valores no positivos
    }

    int integer_part = 0;
    double fraction_part = 0.0;

    // Calcular la parte entera del logaritmo
    while (x >= 2.0) {
        x /= 2.0;
        integer_part++;
    }
    while (x < 1.0) {
        x *= 2.0;
        integer_part--;
    }

    // Calcular la parte fraccionaria usando un método de aproximación
    double fraction = 0.5;
    while (x != 1.0 && fraction > 1e-10) {  // Precisión ajustable
        x *= x;
        if (x >= 2.0) {
            fraction_part += fraction;
            x /= 2.0;
        }
        fraction /= 2.0;
    }

    return ceil_custom(integer_part + fraction_part);
}
