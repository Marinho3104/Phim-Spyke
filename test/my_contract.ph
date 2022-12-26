 
 
namespace ola {
    
    struct Test_Struct {

        Test_Struct* add(Test_Struct);

        Test_Struct* test_func();

    };

    byte ola, ola_address;
    Test_Struct testing;

 
    ::built_ins::Byte* test(byte) {

        testing.test_func()->test_func();
        
        test(*(ola + &ola_address));

    }
    
}
