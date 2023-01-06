
namespace built_ins {

    struct Void {

    };

    struct Byte {

    };
    
    struct Int {

        int equal(int __to_add) {

            *this;
            __to_add;

            exec 10 4;

            *this + 100;

            return 1;

            // exec 13 0;

        }

        int add(int __value) {

            int _result;

            _result;
            *this;
            __value;

            exec 16 4;

            return _result;

        }

        void add_assign(int __value) {

            *this;
            *this;
            __value;

            exec 16 4;

        }

        int mul(int) { return 100;

        }

    };

    struct Test_Struct {

        int j,*k,l;

    };

}

namespace test {

    //::built_ins::Test_Struct _test_;
    //::built_ins::Test_Struct _test_1;

    int test_func() {

        int p, l;

        l = p = 100;

        //p = 100 + 150 * (1000 + 19);

        //p += 12;

        //::built_ins::Int::equal(&p, 150);

        //::built_ins::Int::add_assign(&p, 167);
        
        p;
        l;

    }



}
