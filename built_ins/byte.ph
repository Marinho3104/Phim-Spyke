
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

        ::built_ins::Test_Struct* hey;

        void test_func() {

        }

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

namespace test {

    //::built_ins::Test_Struct _test_;
    //::built_ins::Test_Struct _test_1;

    int test_func() {

        ::built_ins::Test_Struct p, pp, ppp;

        //int p;

        //p = 1000;

        //(int) p += 100;

        //::built_ins::Int::equal(&p, 150);

        //::built_ins::Int::add_assign(&p, 167);
        
        //p + 10;

        //p;

        // p.equal(1010);

        p.hey->hey->hey;

        p;

    }



}
