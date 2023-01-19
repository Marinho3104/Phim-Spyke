
namespace built_ins {

    struct Int {

        Int();
        
        Int(int);

        int add(int);

        int mul(int);


        void equal(int);


        bool equal_to(int);

        bool not_equal_to(int);

        int increment();

        int decrement();

        int increment_left();
        
        int decrement_left();

    };

}

built_ins::Int::Int(int __val) { *this; __val; exec 10 4; }

int built_ins::Int::add(int __add) {

    int __rtr; __rtr;

    *this; __add; exec 16 4;

    return __rtr;
}

int built_ins::Int::mul(int __rslt) {

    int __rtr;

    __rtr;
    *this;
    __rslt;
    exec 27 4;

    return __rtr;
}


void built_ins::Int::equal(int __eq) { *this; __eq; exec 10 4; }


bool built_ins::Int::equal_to(int __comp) { bool __rslt; __rslt; *this; __comp; exec 19 4; return __rslt; }

bool built_ins::Int::not_equal_to(int __comp) { bool __rslt; __rslt; *this; __comp; exec 20 4; return __rslt; }


int built_ins::Int::increment() { return *this + 1; }

//int built_ins::Int::decrement() { return *this - 1; }

int built_ins::Int::increment_left() { int _rtr = *this; *this++; return _rtr; }

//int built_ins::Int::decrement_left() { int _rtr = *this; *this--; return _rtr; }

