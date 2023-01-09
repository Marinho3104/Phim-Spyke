
namespace built_ins {

    struct Void {

    };

    struct Byte {

    };
    
    struct Int {

        void equal(int __to_add);

        int add(int __value);

        void add_assign(int __value);

        int mul(int) { return 100; }

    };

    struct Hey {

        byte hh;

    };

    struct Test_Struct {

        int j;

        // static int _static;

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

int jk;

namespace test {

    //::built_ins::Test_Struct _test_;
    //::built_ins::Test_Struct _test_1;

    int test_func() {

        ::built_ins::Test_Struct _test_struct;
        ::built_ins::Test_Struct _test_struct_;

        jk = 1000;

        _test_struct.j = 12345;
        

        _test_struct_.j = 123;
        //_test_struct_._static = 500;

        _test_struct.j;
        //_test_struct._static;

        _test_struct_.j;
        //_test_struct_._static;

        jk;

    }



}
