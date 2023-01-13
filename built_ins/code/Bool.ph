
namespace built_ins {

    struct Void {

    };

    struct Bool {

        Bool();

        void equal(bool __equal) {

        *this;
        __equal;

        exec 10 1;

        }

    };

    struct Int {

        void equal(int __to_add);

        int add(int __value);

        void add_assign(int __value);

        int mul(int) { return 100; }

        bool equal_to(int);

        bool not_equal_to(int);

    };

    struct Pointer {

        Pointer();
        Pointer(int __value);

        ~Pointer();

        void equal(pointer);
        void equal(int);

        pointer add(int, int);

    };

}

void built_ins::Int::equal(int __to_add) {
    
    *this;
    __to_add;

    exec 10 4;

}

int built_ins::Int::add(int __value) {

    int _result;

    _result;
    *this;
    __value;

    exec 16 4;

    return _result;

}

void built_ins::Int::add_assign(int __value) {

    *this;
    *this;
    __value;

    exec 16 4;

}

bool built_ins::Int::equal_to(int __value) {

    bool _result;

    _result;
    *this;
    __value;

    exec 19 4;

    return _result;

}

bool built_ins::Int::not_equal_to(int __value) {

    bool _result;

    _result;
    *this;
    __value;

    exec 20 4;

    return _result;

}


void built_ins::Pointer::equal(pointer __eq) {
    
    *this;
    __eq;

    exec 10 2;

}

void built_ins::Pointer::equal(int __eq) {



}


pointer built_ins::Pointer::add(int __v, int __so) {

    pointer _return;

    _return;
    *this;
    __v;

    __v;
    __v;
    __so;
    exec 27 4;

    exec 16 4;

    return _return;

}

struct Ey {

    int j,k,l,m,n,o,p;

};

namespace test {

    void test_fufu(bool) {

    }

    int test_func() {

        int k;

        k = 100;
        
        
    }



}
