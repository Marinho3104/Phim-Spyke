
namespace built_ins {

    struct Int {

        void equal(int);

        int add(int);

        int mul(int);

    };

}

void built_ins::Int::equal(int __eq) { *this; __eq; exec 10 4; }

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
