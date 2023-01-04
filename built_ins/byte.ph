
namespace built_ins {

    struct Byte {

    };
    
    struct Int {

        int add(int) {

            int l,ll,lll,llll;

        }

        int mul(int) {

        }

    };

    struct Test_Struct {

        int j,*k,l;

    };

}

namespace test {

    ::built_ins::Test_Struct _test_;
    ::built_ins::Test_Struct _test_1;

    int test_func_call() {

        int ff;

        ff;
        
    }

    int test_func() {

        ::built_ins::Test_Struct _test;

        _test;

        _test_;

        test_func_call();

    }



}


/*

#define BYTE_NAME byte

namespace built_ins {

    struct Byte {};

    BYTE_NAME ola;

    
    struct Int {

        //int add(int*) {
        //}

        int add(int) {

            int l,ll,lll,llll;

        }

        int mul(int*) {
            int l,ll,lll,llll, lllll, llllll;

        }

        int mul(int) {

        }

        int test_func(int) {

            // this;

        }

    };

}

int j;

 
namespace ola {

    struct Test_Struct {

        int jojo, jojojo;

    };
 
    int var, var2;
    Test_Struct* _test_struct;
 
    int func_test(int*) {
 
        (var2 + var2) * var;

        func_test(&var);
 
        // var.add(var);

        // _test_struct->jojojo;
 
    }
 
 
}
 
*/