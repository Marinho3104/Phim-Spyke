
#include "built_ins/code/Int.h"
#include "built_ins/code/Bool.h"



built_ins::Int::Int() {}

built_ins::Int::Int(int __v) { *this = __v; }


/* Arithmetic */

int built_ins::Int::add(int __a) {

    int _r; 
    
    _r; *this; __a; exec 16 4;

    return _r;

}

int built_ins::Int::sub(int __s) {

    int _r; 
    
    _r; *this; __s; exec 17 4;

    return _r;

}

int built_ins::Int::mul(int __m) {

    int _r; 
    
    _r; *this; __m; exec 27 4;

    return _r;

}

int built_ins::Int::div(int __d) {

    int _r; 
    
    _r; *this; __d; exec 28 4;

    return _r;

}

int built_ins::Int::mod(int __m) {

    int _r; 
    
    _r; *this; __m; exec 29 4;

    return _r;

}


/* Increment Decrement */

int built_ins::Int::increment() { int _v = *this; *this; exec 30 4; return _v; }

int built_ins::Int::decrement() { int _v = *this; *this; exec 31 4; return _v; } 

int built_ins::Int::increment_left() { *this; exec 30 4; return *this; }

int built_ins::Int::decrement_left() { *this; exec 31 4; return *this; }


/* Assign */

void built_ins::Int::equal(int __eq) { *this; __eq; exec 10 4; }

void built_ins::Int::add_assign(int __a) { *this; *this; __a; exec 16 4; }

void built_ins::Int::sub_assign(int __s) { *this; *this; __s; exec 17 4; }

void built_ins::Int::mul_assign(int __m) { *this; *this; __m; exec 27 4; }

void built_ins::Int::div_assign(int __d) { *this; *this; __d; exec 28 4; }

void built_ins::Int::mod_assign(int __m) { *this; *this; __m; exec 29 4; }


/* Relational */

bool built_ins::Int::equal_to(int __cmp) { bool __rslt; __rslt; *this; __cmp; exec 19 4; return __rslt; }

bool built_ins::Int::not_equal_to(int __n_cmp) { bool __rslt; __rslt; *this; __n_cmp; exec 20 4; return __rslt; }
