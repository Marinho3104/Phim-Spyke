
namespace built_ins {

    struct Pointer {

        Pointer();

        Pointer(pointer); 
        
        Pointer(int*);

        void equal(pointer);

        pointer add(int, int);

        pointer add(int);

        void add(int, int);

        void add(int);
        
        void add_assign(int);

        void add_assign(int, int);

    };

}

built_ins::Pointer::Pointer() {}

built_ins::Pointer::Pointer(pointer __pntr) { *this; __pntr; exec 10 2; }

built_ins::Pointer::Pointer(int __v) { *this; __v; exec 10 2; }

void built_ins::Pointer::equal(pointer __eq) { *this; __eq; exec 10 2; }

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









void main() {

    int marinho = 100;

    int _var[1];

    int k;

    *_var;
    k;

}
