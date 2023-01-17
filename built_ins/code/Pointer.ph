
namespace built_ins {

    struct Pointer {

        Pointer();

        Pointer(pointer); 
        
        Pointer(int*);

        void equal(pointer);

        void equal(pointer, int);

        pointer add(int, int);

        pointer add(int);

        void add(int, int);

        void add(int);
        
        void add_assign(int);

        void add_assign(int, int);
        
        spyke bracket(int, int);

    };

}


built_ins::Pointer::Pointer() {  }

built_ins::Pointer::Pointer(pointer __pntr) { *this; __pntr; exec 10 2; }

built_ins::Pointer::Pointer(int __v) { *this; __v; exec 10 2; }


void built_ins::Pointer::equal(pointer __eq) { *this; __eq; exec 10 2; }

void built_ins::Pointer::equal(pointer __eq, int) { *this; __eq; exec 10 2; }


pointer built_ins::Pointer::add(int __off, int __s) {

    int __rslt;
    
    __rslt = __off * __s;

    pointer __rtr;

    __rtr;
    *this;
    __rslt;
    exec 16 2;

    return __rtr;

}

pointer built_ins::Pointer::add(int __off) {

    pointer __rtr;

    __rtr;
    *this;
    __off;
    exec 16 2;

    return __rtr;

}


void built_ins::Pointer::add_assign(int __off) { *this; *this; __off; exec 16 2; }

void built_ins::Pointer::add_assign(int __off, int __s) { *this; *this; __off * __s; exec 16 2; }


spyke built_ins::Pointer::bracket(int __idx, int __s) {

    int __rslt;
    
    __rslt = __idx * __s;

    spyke* __rtr;

    __rtr;
    *this;
    __rslt;
    exec 16 2;

    exec 32 2;
    *__rtr;

}

void chng(int __to_change) {

    __to_change = 100;

}

void main() {

    int _to_change = 10;

    chng(_to_change);

    _to_change;

}

/*
int m;
    int _var[100];
    
    //*_var = 101;

    _var[0];


    pointer array[100];

    *array = 1234;

    int* _pntr;
    
    _pntr = array + 1;

    *_pntr = 1010;

    int h, hh;
    
    h = array[0];
    hh = array[1];

    h;
    hh;
    
*/
