
#include "built_ins/code/Bool.h"
#include "built_ins/code/Int.h"


/* Constructors */

built_ins::Bool::Bool() {}

built_ins::Bool::Bool(bool __v) { *this = __v; }

built_ins::Bool::Bool(int __v) { *this = __v != 0; }


/* Assign */

void built_ins::Bool::equal(bool __eq) { *this; __eq; exec 10 1; }

void built_ins::Bool::equal(int __eq) { *this = __eq != 0; }