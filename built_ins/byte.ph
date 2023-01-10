
namespace built_ins {

    struct Pointer {

        //pointer add(pointer);

    };

    struct Void {

    };

    struct Bool {

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

struct Test_Struct {

    int m, a, r, i, n, h, o;

};

namespace test {

    void test_pointer(pointer __test_pointer) {

        // *__test_pointer;

        sizeof(*__test_pointer);

        //*((int*)__test_pointer) = 123456;

    }

    int test_func() {

        //int marinho = 123;

        //::Test_Struct test;

        bool* j, *k;

        //*j = 123;

        test_pointer(j);

        //sizeof(test);

        
    }



}
