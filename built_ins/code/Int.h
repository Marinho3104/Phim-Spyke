
#ifndef BUILT_INS_PRIMITIVE_INT
#define BUILT_INS_PRIMITIVE_INT


namespace built_ins {

    struct Bool;

    struct Int {

        Int();
        
        Int(int);

        /* Arithmetic */

        int add(int); // +
        int sub(int); // -
        int mul(int); // *
        int div(int); // /
        int mod(int); // %

        /* Increment Decrement */

        int increment();
        int decrement();
        int increment_left();
        int decrement_left();

        /* Assign */

        void equal(int);
        void add_assign(int);
        void sub_assign(int);
        void mul_assign(int);
        void div_assign(int);
        void mod_assign(int);

        /* Relational */

        bool equal_to(int);
        bool not_equal_to(int);

    };

}

#endif