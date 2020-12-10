#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "interpret.h"

extern char* named(int);
extern VALUE* create_value(void);
extern void print_value(VALUE*);
extern void print_leaf(NODE*, int);

extern VALUE* frame_check(TOKEN*, FRAME*);
extern VALUE* frame_assign(TOKEN*, FRAME*, VALUE*);
extern FRAME* frame_extend(FRAME*, NODE*, NODE*);

VALUE* run_interpret(NODE* term, FRAME* frame) {
    return interpret(term, frame);
}

static VALUE* ret_val = NULL;

static NODE* formals(CLOSURE* f) {
    return NULL;
}

static bool is_equal(VALUE* v1, VALUE* v2) {
    if(v1->type != v2->type) {
        return false;
    }

    if(v1->type == CONSTANT) {
        bool result = v1->v.integer == v2->v.integer;
        return v1->v.integer == v2->v.integer;
    }
    return v1->v.string == v2->v.string;
}

static VALUE* interpret_leaf(NODE* term, FRAME* frame) {
    VALUE* val = create_value();
    TOKEN* t = (TOKEN*) term->left;
    if(t == NULL) {
        return NULL;
    }
    val->type = t->type;

    VALUE* bound = frame_check(t, frame);
    if(bound != NULL) {
        return bound;
    }

    if(val->type == CONSTANT) {
        val->v.integer = t->value;
    } else if(val->type == STRING_LITERAL) {
        val->v.string = t->lexeme;
    } else if(val->type == IDENTIFIER) {
        val->v.string = t->lexeme;
    }

    return val;
}

static VALUE* interpret_assign(NODE* term, FRAME* frame) {
    if(term == NULL) return NULL;

    TOKEN* t = (TOKEN*) term->left->left;

    VALUE* bound = frame_check(t, frame);
    if(bound != NULL) {
        VALUE* val = interpret(term->right, frame);
        //printf("Reassigning identifier to: ");
        //print_value(val);
        //puts(" ");
        frame_assign(t, frame, val);
        return NULL; //statements do not return
    } else {
        VALUE* val = interpret(term->right, frame);
        //printf("Assigning %s to: ", t->lexeme);
        //print_value(val);
        //puts(" ");
        frame_declaration(t, frame);
        frame_assign(t, frame, val);
        return NULL;
    }
}

static VALUE* interpret_greater(NODE* term, FRAME* frame) {
    int result = interpret(term->left, frame)->v.integer > interpret(term->right, frame)->v.integer;
    //printf("Evaluated >: %d\n", result);
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = result;
    return val;
}

static VALUE* interpret_lesser(NODE* term, FRAME* frame) {
    int result = interpret(term->left, frame)->v.integer < interpret(term->right, frame)->v.integer;
    //printf("Evaluated <: %d\n", result);
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = result;
    return val;
}

static VALUE* interpret_div(NODE* term, FRAME* frame) {
    int result = interpret(term->left, frame)->v.integer / interpret(term->right, frame)->v.integer;
    //printf("Evaluated /: %d\n", result);
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = result;
    return val;
}

static VALUE* interpret_mul(NODE* term, FRAME* frame) {
    int result = interpret(term->left, frame)->v.integer * interpret(term->right, frame)->v.integer;
    //printf("Evaluated *: %d\n", result);
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = result;
    return val;
}

static VALUE* interpret_mod(NODE* term, FRAME* frame) {
    int result = interpret(term->left, frame)->v.integer % interpret(term->right, frame)->v.integer;
    //printf("Evaluated %%: %d\n", result);
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = result;
    return val;
}

static VALUE* interpret_add(NODE* term, FRAME* frame) {
    int result = interpret(term->left, frame)->v.integer + interpret(term->right, frame)->v.integer;
    //printf("Evaluated +: %d\n", result);
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = result;
    return val;
}

static VALUE* interpret_sub(NODE* term, FRAME* frame) {
    int result = interpret(term->left, frame)->v.integer - interpret(term->right, frame)->v.integer;
    //printf("Evaluated -: %d\n", result);
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = result;
    return val;
}

static VALUE* interpret_equality(NODE* term, FRAME* frame) {
    bool result = is_equal(interpret(term->left, frame), interpret(term->right, frame));
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = result;
    //val->v.boolean = result; -- TODO bool functionality (currently stored as constant)
    return val;
}

static VALUE* interpret_inequality(NODE* term, FRAME* frame) {
    bool result = is_equal(interpret(term->left, frame), interpret(term->right, frame));
    VALUE* val = create_value();
    val->type = CONSTANT;
    val->v.integer = -1*result;
    //val->v.boolean = result; -- TODO bool functionality (currently stored as constant)
    return val;
}

static VALUE* interpret_punct(NODE* term, FRAME* frame) {
    char operator = term->type;
    switch(operator) {
        case '/':
            return interpret_div(term, frame);
        case '%':
            return interpret_mod(term, frame);
        case '*':
            return interpret_mul(term, frame);
        case '+':
            return interpret_add(term, frame);
        case '-':
            return interpret_sub(term, frame);
        case '>':
            return interpret_greater(term, frame);
        case '<':
            return interpret_lesser(term, frame);
        case '=':
            return interpret_assign(term, frame);
        case '~':
            interpret(term->left, frame);
            return interpret(term->right, frame);
        case ';':
            {
                VALUE* first = interpret(term->left, frame);
                VALUE* second = interpret(term->right, frame);
                if(first) return first;
                return second;
            }
        default:
            return NULL;
    }
}

static VALUE* interpret_if(NODE* term, FRAME* frame) {
    //printf("%s\n", named(term->type));
    VALUE* result = interpret(term->left, frame);
    if(result->v.integer > 0) {
        //puts("Condition was true!");
        if(term->right->type == ELSE) {
            result = interpret(term->right->left, frame);
        } else {
            result = interpret(term->right, frame);
        }
    } else {
        //puts("Condition was false!");
        if(term->right->type == ELSE) {
            result = interpret(term->right->right, frame);
        } else {
            result = NULL;
        }
    }
    return result;
}

static VALUE* interpret_else(NODE* term, FRAME* frame) {

    return NULL;
}

void print_formals(NODE* formals) {
    if(!formals) return;
    if(formals->type == LEAF) {
        print_leaf(formals, 0);
        return;
    } else {
        printf("%d %s\n", formals->type, named(formals->type));
    }
    print_formals(formals->left);
    print_formals(formals->right);
    return;
}

static VALUE* interpret_declaration(NODE* term, FRAME* frame) {
    NODE* d = term->left;
    NODE* code = term->right;

    NODE* type = d->left;
    NODE* F = d->right;

    TOKEN* t = (TOKEN*) F->left->left;


    //printf("Found function: %s\t%p\n", t->lexeme, t);

    VALUE* bound = frame_check(t, frame);
    if(bound != NULL) {
        printf("ERROR: FUNCTION ALREADY DEFINED\n");
        exit(-1);
    } else {
        VALUE* val = create_value();
        CLOSURE* f = malloc(sizeof(CLOSURE));

        if(strcmp(t->lexeme, "main") == 0) {
            //puts("Found main, starting program");
            return interpret(code, frame);
        }

        f->name = t;
        f->formals = F->right;

        f->frame = frame;
        f->code = code;
        val->v.f = f;
        frame_declaration(t, frame);
        frame_assign(t, frame, val);
        return val;
    }

    return NULL;
}

static VALUE* lexical_call(VALUE* function, NODE* args, FRAME* frame) {
    CLOSURE* f = function->v.f;

    //handle built-ins
    if(strcmp(function->v.string, "print_str") == 0 || strcmp(function->v.string, "print_int") == 0) {
        print_value(interpret(args, frame));
        printf("\n");
        return NULL;
    }

    //printf("Calling function: %s\n", name->lexeme);

    FRAME* new_frame = frame_extend(frame, f->formals ? f->formals->right : f->formals, args);
    new_frame->next = f->frame;

    VALUE* res = interpret(f->code, new_frame);
    return res;
}

static VALUE* interpret_apply(NODE* term, FRAME* frame) {
    //term is the APPLY node

    VALUE* function = interpret(term->left, frame);
    NODE* args = term->right;
    //printf("Applying: ");
    //print_value(function);
    //puts(" ");
    //printf("args: ");
    //printf("%d\n", ((TOKEN*) args->left)->value);
    return lexical_call(function, args, frame);
}

static VALUE* interpret_return(NODE* term, FRAME* frame) {
    VALUE* val = interpret(term->left, frame);
    val->retval = 1;
    //printf("Returning: ");
    //print_value(val);
    //puts(" ");

#if 1
    ret_val = val;
    return val;
#else
    if(ret_val) {
        return ret_val;
    } else {
        ret_val = val;
        return val;
    }
#endif
}

VALUE* interpret(NODE* term, FRAME* frame) {
    if(term == NULL) return ret_val;
    switch(term->type) {
        case LEAF:
            return (VALUE*) interpret_leaf(term, frame);
        case RETURN:
            return (VALUE*) interpret_return(term, frame);
        case EQ_OP:
            return (VALUE*) interpret_equality(term, frame);
        case NE_OP:
            return (VALUE*) interpret_inequality(term, frame);
        case IF:
            return (VALUE*) interpret_if(term, frame);
        case APPLY:
            return (VALUE*) interpret_apply(term, frame);
        case ELSE:
            return (VALUE*) interpret_else(term, frame);
        default:
            if(ispunct(term->type)) {
                return (VALUE*) interpret_punct(term, frame);
            } else if(term->type == '~') {
                return interpret_punct(term, frame);
            } else if(term->type == 'D') {
                  return interpret_declaration(term, frame);
            }
            break;
    }
}

void cleanup_interpret(NODE* tree, FRAME* frame) {
    //free(frame);
    if(tree == NULL) return;
    //cleanup_interpret(tree->left, NULL);
    //cleanup_interpret(tree->right, NULL);
    //free(tree);
}
