
#ifndef BUILT_INS_PRIMITIVE_BOOL
#define BUILT_INS_PRIMITIVE_BOOL


namespace built_ins {

    struct Int;

    struct Bool {

        Bool();

        Bool(bool);

        Bool(int);

        /* Assign */

        void equal(bool);
        
        void equal(int);

    };

}

#endif