
namespace built_ins {

    struct Int {

        Int();
        
        Int(int);

        int add(int);

        int mul(int);


        void equal(int);


        bool equal_to(int);

        bool not_equal_to(int);

    };

}

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
