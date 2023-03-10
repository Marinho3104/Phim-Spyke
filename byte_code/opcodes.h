

#define BYTE_CODE_STACK_MEMORY_ALLOCATE 0
#define BYTE_CODE_HEAP_MEMORY_ALLOCATE 1

#define BYTE_CODE_LOAD 2
#define BYTE_CODE_LOAD_GLOBAL 3

#define BYTE_CODE_CALL 4

#define BYTE_CODE_LOAD_IMPLICIT_VALUE 5

#define BYTE_CODE_END_CODE_BLOCK 8

#define BYTE_CODE_NOP 9

#define BYTE_CODE_MEMORY_COPY 10

#define BYTE_CODE_SET_INTO_STACK 11
#define BYTE_CODE_GET_FROM_STACK 12

#define BYTE_CODE_CLEAN_STACK 13

#define BYTE_CODE_COPY_PREVIOUS_STACK_DATA 14

#define BYTE_CODE_CLOSE_STACK_FRAME 15

#define BYTE_CODE_BINARY_ADD 16
#define BYTE_CODE_BINARY_SUB 17

#define BYTE_CODE_COPY_PREVIOUS_STACK_DATA_REMOVE 18

#define BYTE_CODE_BINARY_EQUAL_TO 19
#define BYTE_CODE_BINARY_NOT_EQUAL_TO 20 
#define BYTE_CODE_BINARY_GREATER_THAN 21 
#define BYTE_CODE_BINARY_LESS_THAN 22
#define BYTE_CODE_BINARY_GREATER_THAN_EQUAL_TO 23 
#define BYTE_CODE_BINARY_LESS_THAN_EQUAL_TO 24

#define BYTE_CODE_IF 25

#define BYTE_CODE_CALL_SUB 26

#define BYTE_CODE_BINARY_MUL 27
#define BYTE_CODE_BINARY_DIV 28
#define BYTE_CODE_BINARY_MOD 29

#define BYTE_CODE_BINARY_INC 30
#define BYTE_CODE_BINARY_DEC 31

#define BYTE_CODE_EXECUTE_PREVIOUS_STACK 32

#define BYTE_CODE_JUMP 33

#define BYTE_CODE_SET_INDEX 34

#define BYTE_CODE_CLOSE_STACK_FRAME_SPECIFIC 35