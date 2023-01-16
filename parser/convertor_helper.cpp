#include "convertor_helper.h"

#include "token_definitions.h"
#include "ast_definitions.h"
#include "ast.h"
#include "linked_List.h"
#include "byte_code.h"
#include "ast.h"
#include "ast_helper.h"
#include "opcodes.h"
#include "tokenizer_helper.h"
#include "convertor.h"
#include "ast_nodes.h"

#include <iostream>
#include <string.h>
 
utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNode(Ast_Node* __node) {

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>();
    _byte_code->destroy_content = 0;

    if (!__node) goto return_;

    switch (__node->node_type)
    {
    case AST_NODE_NAME_SPACE:

        // parser::convertor_control->allocBlock();
        
        parser::convertor_control->setBlock(
            ((Ast_Node_Name_Space*) __node)->declarations,
            convertor_control->byte_code_blocks->first->object
        );
    
        goto return_; break;
    
    case AST_NODE_CODE_BLOCK: 
        {
            ((Ast_Node_Code_Block*) __node)->body_position = parser::convertor_control->allocBlock();
            
            parser::convertor_control->setBlock(
                ((Ast_Node_Code_Block*) __node)->code,
                convertor_control->byte_code_blocks->last->object
            );

            byte_code::Byte_Code* _call_function = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

            new (_call_function) byte_code::Byte_Code(
                BYTE_CODE_CALL_SUB,
                ((Ast_Node_Code_Block*) __node)->body_position
            );

            _byte_code->add(_call_function);
        
            goto return_; break;
        }
    case AST_NODE_FUNCTION_DECLARATION:

        getByteCodeOfNodeFunctionDeclaration(
            (Ast_Node_Function_Declaration*) __node
        );

        goto return_; break;
        
   
    case AST_NODE_VARIABLE:

        _byte_code->add(
            getByteCodeOfNodeVariable(
                (Ast_Node_Variable*) __node
            )
        );

        goto return_; break;

    case AST_NODE_VALUE:

        _byte_code->add(
            getByteCodeOfNodeValue(
                (Ast_Node_Value*) __node
            )
        );

        goto return_; break;



    case AST_NODE_BYTE_CODE:

        _byte_code->add(
            getByteCodeOfNodeByteCode(
                (Ast_Node_Byte_Code*) __node
            )
        );
        goto return_; break;
    case AST_NODE_SIZE_OF:

        _byte_code->add(
            getByteCodeOfNodeFunctionSizeOf(
                (Ast_Node_Function_Size_Of*) __node
            )
        );
        goto return_; break;
    default: break;
    }

    delete _byte_code;

    switch (__node->node_type)
    {
    case AST_NODE_VARIABLE_DECLARATION: 
        
        _byte_code = 
            getByteCodeOfNodeVariableDeclaration(
                (Ast_Node_Variable_Declaration*) __node
            ); 
        
        break;
        
    case AST_NODE_FUNCTION_CALL:

        _byte_code = 
            getByteCodeOfNodeFunctionCall(
                (Ast_Node_Function_Call*) __node
            );

        goto return_; break;
    case AST_NODE_EXPRESSION: 

        ((Ast_Node_Expression*) __node)->getResultDeclaration();
    
        _byte_code = getByteCodeOfNodeExpression(
            (Ast_Node_Expression*) __node
        );
        
        break;
    case AST_NODE_STRUCT_DECLARATION:

        _byte_code = getByteCodeOfNodeStructDeclaration(
            (Ast_Node_Struct_Declaration*) __node
        );

        goto return_; break;
    case AST_NODE_POINTER_OPERATION:

        _byte_code = getByteCodeOfNodePointerOperation(
            (Ast_Node_Pointer_Operation*) __node
        );

        break;
    case AST_NODE_PARENTHESIS:

        _byte_code = getByteCodeOfNodeParenthesis(
            (Ast_Node_Parenthesis*) __node
        );

        break;
    case AST_NODE_ACCESSING:

        _byte_code = getByteCodeOfNodeAccessing(
            (Ast_Node_Accessing*) __node
        );

        break;
    case AST_NODE_RETURN:

        _byte_code = getByteCodeOfNodeReturn(
            (Ast_Node_Return*) __node
        );

        break;
    case AST_NODE_CAST:

        _byte_code = getByteCodeOfNodeCast(
            (Ast_Node_Cast*) __node
        );

        break;
    case AST_NODE_IF:

        _byte_code = getByteCodeOfNodeIf(
            (Ast_Node_If*) __node
        );

        break;
    default: std::cout << "erorr yey" << std::endl; exit(1); break;
    }

return_:
    return _byte_code;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeVariableDeclaration(Ast_Node_Variable_Declaration* __node_variable_declaration) {

    parser::convertor_control->print("Node Variable Declaration - Byte Code");
    std::cout << convertor_control->block_in_set->current_allocation_size << std::endl;

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code_rtr = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    _byte_code_rtr->destroy_content = 0;

    byte_code::Byte_Code* _variable = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
    new (_variable) byte_code::Byte_Code(
        BYTE_CODE_STACK_MEMORY_ALLOCATE,
        __node_variable_declaration->type->getSize()
    );
    _byte_code_rtr->add(_variable);

    __node_variable_declaration->address = 
        convertor_control->block_in_set->current_allocation_size;

    convertor_control->block_in_set->current_allocation_size += _variable->argument;


    if (__node_variable_declaration->array_length) {

        byte_code::Byte_Code* _load_var = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        new (_load_var) byte_code::Byte_Code(
            BYTE_CODE_LOAD,
            __node_variable_declaration->address
        );
        _byte_code_rtr->add(_load_var);


        byte_code::Byte_Code* _load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        new (_load) byte_code::Byte_Code(
            BYTE_CODE_LOAD,
            convertor_control->block_in_set->current_allocation_size
        );
        _byte_code_rtr->add(_load);

        byte_code::Byte_Code* _byte_code_chng = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        new (_byte_code_chng) byte_code::Byte_Code(
            BYTE_CODE_SET_INTO_STACK,
            0
        );
        _byte_code_rtr->add(_byte_code_chng);


        _temp = getByteCodeOfNodeExpression(
            (Ast_Node_Expression*) __node_variable_declaration->array_length
        );
        _byte_code_rtr->join(
            _temp
        );
        delete _temp;


        __node_variable_declaration->type->pointer_level--;

        byte_code::Byte_Code* _stack_off = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
        new (_stack_off) byte_code::Byte_Code(
            BYTE_CODE_STACK_OFF,
            __node_variable_declaration->type->getSize()
        );
        _byte_code_rtr->add(_stack_off);        

        __node_variable_declaration->type->pointer_level++;

    }

    return _byte_code_rtr;


}

void parser::getByteCodeOfNodeFunctionDeclaration(Ast_Node_Function_Declaration* __node_function_declaration) {

    parser::convertor_control->print("Node Function Declaration - Byte Code");

    byte_code::Byte_Code_Block* _previous_byte_code_block = convertor_control->block_in_set;

    if (!__node_function_declaration->forward) 
        __node_function_declaration->body_position = parser::convertor_control->allocBlock();
    else { __node_function_declaration->body_position = __node_function_declaration->forward->body_position; }

    if (!__node_function_declaration->body) {

        convertor_control->block_in_set = _previous_byte_code_block;
        
        return;
        
    }

    std::cout << "Function body -> " << __node_function_declaration->body_position << std::endl;

    byte_code::Byte_Code_Block* _byte_code_block = parser::convertor_control->byte_code_blocks->operator[](__node_function_declaration->body_position);
    byte_code::Byte_Code* _previous_stack, *_copy_memory;
    utils::Linked_List <byte_code::Byte_Code*>* _temp;

    convertor_control->block_in_set = _byte_code_block;

    for (int _ = __node_function_declaration->parameters->count - 1; _ >= 0; _--) {

        int _length = 0;

        _temp = getByteCodeOfNode(
            __node_function_declaration->parameters->operator[](_)
        );
        
        _byte_code_block->block->join(
            _temp
        );

        delete _temp;

        if (_byte_code_block->block->last->object->code == BYTE_CODE_STACK_MEMORY_ALLOCATE) {

            _length = ((Ast_Node_Variable_Declaration*)__node_function_declaration->parameters->operator[](_))->type->getSize();

            byte_code::Byte_Code* _memory_load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

            new (_memory_load) byte_code::Byte_Code(
                BYTE_CODE_LOAD,
                ((Ast_Node_Variable_Declaration*)__node_function_declaration->parameters->operator[](_))->address
            );

            _byte_code_block->block->add(
                _memory_load
            );

        }

        _previous_stack = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_previous_stack) byte_code::Byte_Code(
            BYTE_CODE_COPY_PREVIOUS_STACK_DATA_REMOVE,
            __node_function_declaration->representive_declaration->type->getSize() ? 1 : 0
        );
        
        _byte_code_block->block->add(
            _previous_stack
        );
        
        _copy_memory = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_copy_memory) byte_code::Byte_Code(
            BYTE_CODE_MEMORY_COPY,
            _length
        );

        _byte_code_block->block->add(
            _copy_memory
        );

    }

    if (!__node_function_declaration->is_static && __node_function_declaration->is_struct) {

        _temp = getByteCodeOfNode(
            __node_function_declaration->this_variable
        );
        
        _byte_code_block->block->join(
            _temp
        );

        delete _temp;

        _previous_stack = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_previous_stack) byte_code::Byte_Code(
            BYTE_CODE_COPY_PREVIOUS_STACK_DATA_REMOVE,
            __node_function_declaration->representive_declaration->type->getSize() ? 1 : 0
        );
        
        _byte_code_block->block->add(
            _previous_stack
        );
        
        byte_code::Byte_Code* _byte_code_load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_byte_code_load) byte_code::Byte_Code(
            BYTE_CODE_LOAD,
            convertor_control->block_in_set->current_allocation_size - 2
        );

        _byte_code_block->block->add(_byte_code_load);


        byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_byte_code) byte_code::Byte_Code(
            BYTE_CODE_GET_FROM_STACK,
            0
        );

        _byte_code_block->block->add(_byte_code);
        
        // _copy_memory = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        // new (_copy_memory) byte_code::Byte_Code(
        //     BYTE_CODE_MEMORY_COPY,
        //     2
        // );

        // _byte_code_block->block->add(
        //     _copy_memory
        // );

    }

    parser::convertor_control->setBlock(
        __node_function_declaration->body->code,
        _byte_code_block
    );

    convertor_control->block_in_set = _previous_byte_code_block;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeStructDeclaration(Ast_Node_Struct_Declaration* __node_struct_declaration) {

    parser::convertor_control->print("Node Struct Declaration - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    _byte_code->destroy_content = 0;

    for (int _ = 0; _ < __node_struct_declaration->functions->declarations->count; _++) 

        if (__node_struct_declaration->functions->declarations->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION) {

            std::cout << "Static var" << std::endl;

            _temp = getByteCodeOfNodeVariableDeclaration(
                (Ast_Node_Variable_Declaration*) __node_struct_declaration->functions->declarations->operator[](_)
                );

            _byte_code->join(_temp);
            delete _temp;

        }

        else 
            getByteCodeOfNodeFunctionDeclaration(
                (Ast_Node_Function_Declaration*) __node_struct_declaration->functions->declarations->operator[](_)
            );

    parser::convertor_control->print("Node Struct Declaration End - Byte Code");


    return _byte_code;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeExpression(Ast_Node_Expression* __node_expression) {

    parser::convertor_control->print("Node Expression - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    byte_code::Byte_Code* _call_function;

    _byte_code->destroy_content = 0;

    // std::cout << "Organized set -> " << __node_expression->organized_set << std::endl;

    for (int _ = 0; _ < __node_expression->organized_set->count; _++) {

        switch (__node_expression->organized_set->operator[](_)->node_type)
        {
        case AST_NODE_FUNCTION_DECLARATION:
            {

                if ( ((Ast_Node_Function_Declaration*) __node_expression->organized_set->operator[](_))->representive_declaration->type->getSize()) {
                    byte_code::Byte_Code* _memory_allocation_return = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                    new (_memory_allocation_return) byte_code::Byte_Code(
                        BYTE_CODE_STACK_MEMORY_ALLOCATE,
                        ((Ast_Node_Function_Declaration*) __node_expression->organized_set->operator[](_))->representive_declaration->type->getSize()
                    );

                    // __node_function_call->representive_declaration->address = convertor_control->block_in_set->current_allocation_size;

                    _byte_code->add(_memory_allocation_return);

                    convertor_control->block_in_set->current_allocation_size += _memory_allocation_return->argument;

                    byte_code::Byte_Code* _load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                    new (_load) byte_code::Byte_Code(
                        BYTE_CODE_LOAD,
                        convertor_control->block_in_set->current_allocation_size - _memory_allocation_return->argument
                    );  

                    _byte_code->add(_load);

                }

                

                _call_function = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                new (_call_function) byte_code::Byte_Code(
                    BYTE_CODE_CALL,
                    ((Ast_Node_Function_Declaration*) __node_expression->organized_set->operator[](_))->body_position
                );

                _byte_code->add(
                    _call_function
                );

                break;

            }
        
        default:

            _temp = getByteCodeOfNode(
                __node_expression->organized_set->operator[](_)
            );

            _byte_code->join(
                _temp
            );

            delete _temp;

            break;
        }

    }

    return _byte_code;

}

byte_code::Byte_Code* parser::getByteCodeOfNodeVariable(Ast_Node_Variable* __node_variable) {

    parser::convertor_control->print("Node Variable - Byte Code");

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        __node_variable->representive_declaration->global ? BYTE_CODE_LOAD_GLOBAL : BYTE_CODE_LOAD,
        __node_variable->representive_declaration->address
    );

    return _byte_code;

}

byte_code::Byte_Code* parser::getByteCodeOfNodeValue(Ast_Node_Value* __node_value) {

    parser::convertor_control->print("Node Value - Byte Code");

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        BYTE_CODE_LOAD_IMPLICIT_VALUE,
        getImplicitValueOff(__node_value->implicit_value_position)
    );

    return _byte_code;  

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodePointerOperation(Ast_Node_Pointer_Operation* __node_pointer_operation) {

    parser::convertor_control->print("Node Pointer Operation - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _pointer_operation = getByteCodeOfNode(
        __node_pointer_operation->value
    );

    if (__node_pointer_operation->pointer_level) {

        if (__node_pointer_operation->pointer_level > 0) {

            for (int _ = 0; _ < __node_pointer_operation->pointer_level; _++) {

                byte_code::Byte_Code* _byte_code_memory_allocation = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                new (_byte_code_memory_allocation) byte_code::Byte_Code(
                    BYTE_CODE_STACK_MEMORY_ALLOCATE,
                    2
                );

                convertor_control->block_in_set->current_allocation_size += _byte_code_memory_allocation->argument;

                _pointer_operation->add(_byte_code_memory_allocation);



                byte_code::Byte_Code* _byte_code_load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                new (_byte_code_load) byte_code::Byte_Code(
                    BYTE_CODE_LOAD,
                    convertor_control->block_in_set->current_allocation_size - _byte_code_memory_allocation->argument
                );

                _pointer_operation->add(_byte_code_load);



                byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                new (_byte_code) byte_code::Byte_Code(
                    BYTE_CODE_GET_FROM_STACK,
                    0
                );

                _pointer_operation->add(_byte_code);

            }

            
        }

        else {

            for (int _ = 0; _ < __node_pointer_operation->pointer_level * -1; _++) {

                byte_code::Byte_Code* _byte_code_chng = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                new (_byte_code_chng) byte_code::Byte_Code(
                    BYTE_CODE_SET_INTO_STACK,
                    0
                );

                _pointer_operation->add(_byte_code_chng);

            }
        }

        

    }

    else {



    }

    return _pointer_operation;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeParenthesis(Ast_Node_Parenthesis* __node_parenthesis) {

    parser::convertor_control->print("Node Parenthesis - Byte Code");

    return getByteCodeOfNodeExpression(
        __node_parenthesis->expression
    );

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeFunctionCall(Ast_Node_Function_Call* __node_function_call) {

    parser::convertor_control->print("Node Function Call - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    _byte_code->destroy_content = 0;

    // Add space for memory allocation and load of return value if needed

    for (int _ = 0; _ < __node_function_call->parameters->count; _++) {

        _temp = getByteCodeOfNodeExpression(__node_function_call->parameters->operator[](_));
        
        _byte_code->join(_temp);

        delete _temp;

    }

    if (__node_function_call->representive_declaration->type->getSize()) {
        byte_code::Byte_Code* _memory_allocation_return = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_memory_allocation_return) byte_code::Byte_Code(
            BYTE_CODE_STACK_MEMORY_ALLOCATE,
            __node_function_call->representive_declaration->type->getSize()
        );

        __node_function_call->representive_declaration->address = convertor_control->block_in_set->current_allocation_size;

        convertor_control->block_in_set->current_allocation_size += _memory_allocation_return->argument;

        _byte_code->add(_memory_allocation_return);

        byte_code::Byte_Code* _load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_load) byte_code::Byte_Code(
            BYTE_CODE_LOAD,
            __node_function_call->representive_declaration->address
        );

        _byte_code->add(_load);
    }

    byte_code::Byte_Code* _byte_code_call = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code_call) byte_code::Byte_Code(
        BYTE_CODE_CALL,
        __node_function_call->function_declaration->body_position
    );

    _byte_code->add(_byte_code_call);

    return _byte_code;    

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeAccessing(Ast_Node_Accessing* __node_accessing) {

    parser::convertor_control->print("Node Accessing - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    _byte_code->destroy_content = 0;

    // _temp = getByteCodeOfNode(
    //     __node_accessing->value
    // );

    // _byte_code->join(
    //     _temp
    // );

    // delete _temp;

    while(__node_accessing) {

        switch (__node_accessing->accessing->node_type)
        {
        case AST_NODE_VARIABLE:

            {

                if (__node_accessing->value->representive_declaration->type->declaration->isStaticVariableDeclaration(
                    __node_accessing->accessing->representive_declaration
                )) {
                    std::cout << "Static Variable " << std::endl;

                    byte_code::Byte_Code* _load_byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                    new (_load_byte_code) byte_code::Byte_Code(
                        BYTE_CODE_LOAD_GLOBAL,
                        __node_accessing->accessing->representive_declaration->address
                    );

                    _byte_code->add(
                        _load_byte_code
                    );

                }

                else {

                    __node_accessing->accessing->representive_declaration->address = __node_accessing->value->representive_declaration->address + 
                            __node_accessing->value->representive_declaration->type->declaration->getVariablesOff((Ast_Node_Variable*)__node_accessing->accessing);

                    if (__node_accessing->next) break;

                    byte_code::Byte_Code* _load_byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

                    new (_load_byte_code) byte_code::Byte_Code(
                        BYTE_CODE_LOAD,
                        __node_accessing->accessing->representive_declaration->address
                    );

                    _byte_code->add(
                        _load_byte_code
                    );

                }

            }
            break;
        case AST_NODE_FUNCTION_CALL:

            {

                if (__node_accessing->next) break;

                _temp = getByteCodeOfNode(
                    __node_accessing->accessing
                );

                _byte_code->join(
                    _temp
                );

                delete _temp;

                break;

            }


        default: break;
        }

        __node_accessing = __node_accessing->next;

    }

    //exit(1);

    parser::convertor_control->print("Node Accessing End - Byte Code");

    return _byte_code;

}

byte_code::Byte_Code* parser::getByteCodeOfNodeByteCode(Ast_Node_Byte_Code* __node_byte_code) {

    parser::convertor_control->print("Node Byte Code - Byte Code");

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        __node_byte_code->code,
        __node_byte_code->argument
    );

    return _byte_code;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeReturn(Ast_Node_Return* __node_return) {

    parser::convertor_control->print("Node Return - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    _byte_code->destroy_content = 0;

    byte_code::Byte_Code* _previous_stack_data = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_previous_stack_data) byte_code::Byte_Code(
        BYTE_CODE_COPY_PREVIOUS_STACK_DATA,
        0
    );

    _byte_code->add(_previous_stack_data);

    _temp = getByteCodeOfNodeExpression(
        __node_return->expression
    );

    _byte_code->join(_temp);

    delete _temp;

    byte_code::Byte_Code* _memory_copy = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_memory_copy) byte_code::Byte_Code(
        BYTE_CODE_MEMORY_COPY,
        __node_return->representive_declaration->type->getSize()
    );

    _byte_code->add(_memory_copy);


    byte_code::Byte_Code* _close_stack_frame = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_close_stack_frame) byte_code::Byte_Code(
        BYTE_CODE_CLOSE_STACK_FRAME,
        0
    );

    _byte_code->add(_close_stack_frame);

    return _byte_code;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeIf(Ast_Node_If* __node_if) {

    parser::convertor_control->print("Node If - Byte Code");

    __node_if->condition->getResultDeclaration();

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = getByteCodeOfNodeExpression(__node_if->condition), *_temp;

    byte_code::Byte_Code* _if_condition = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_if_condition) byte_code::Byte_Code(
        BYTE_CODE_IF,
        0
    );

    _byte_code->add(_if_condition);

    _temp = getByteCodeOfNode(__node_if->body->first->object);

    _byte_code->join(_temp);

    delete _temp;

    return _byte_code;

}

byte_code::Byte_Code* parser::getByteCodeOfNodeFunctionSizeOf(Ast_Node_Function_Size_Of* __node_function_size_of) {

    parser::convertor_control->print("Node Function Size Of - Byte Code");

    int _size = __node_function_size_of->getSizeOf();

    char* _value;
    asprintf (&_value, "%i", _size);

    int _position = parser::ast_control->addImplicitValue(_value);
    free(_value);

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        BYTE_CODE_LOAD_IMPLICIT_VALUE,
        getImplicitValueOff(_position)
    );  

    return _byte_code;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeCast(Ast_Node_Cast* __node_cast) {

    parser::convertor_control->print("Node Cast - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    _byte_code->destroy_content = 0;

    _temp = getByteCodeOfNodeVariableDeclaration(__node_cast->representive_declaration);
    _byte_code->join(_temp);
    delete _temp;

    byte_code::Byte_Code* _load = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
    new (_load) byte_code::Byte_Code(
        BYTE_CODE_LOAD,
        __node_cast->representive_declaration->address
    );
    _byte_code->add(_load);

    byte_code::Byte_Code* _load_param = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
    new (_load_param) byte_code::Byte_Code(
        BYTE_CODE_LOAD,
        __node_cast->representive_declaration->address
    );
    _byte_code->add(_load_param);

    _temp = getByteCodeOfNode(__node_cast->value);
    _byte_code->join(_temp);
    delete _temp;

    byte_code::Byte_Code* _call = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));
    new (_call) byte_code::Byte_Code(
        BYTE_CODE_CALL,
        __node_cast->representive_declaration->constructor_declaration->body_position
    );
    _byte_code->add(_call);

    parser::convertor_control->print("Node Cast End - Byte Code");

    return _byte_code;
    
}


int parser::getImplicitValueOff(int __position) {

    int _off = 0;

    for (int _ = 0; _ < __position; _++) {

        int _length = strlen(ast_control->implicit_values_collection->operator[](_));

        if (isInt(ast_control->implicit_values_collection->operator[](_))) _length = 4;

        _off += _length;

    }

    return _off;

}



