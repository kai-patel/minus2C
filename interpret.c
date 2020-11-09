#include <stdio.h>
#include <ctype.h>
#include "nodes.h"
#include "C.tab.h"

#include <stdlib.h>
#include <stdbool.h>
#include "interpret.h"
#include "frame.h"

extern char* named(int);
extern VALUE* create_value(void);
extern void print_value(VALUE*);
extern void print_leaf(NODE*, int);

extern VALUE* frame_check(TOKEN*, FRAME*);
extern VALUE* frame_assign(TOKEN*, FRAME*, VALUE*);
extern VALUE* frame_name(TOKEN*, FRAME*);

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
        return val;
    } else {
        VALUE* val = interpret(term->right, frame);
        //printf("Assigning %s to: ", t->lexeme);
        //print_value(val);
        //puts(" ");
        frame_declaration(t, frame);
        frame_assign(t, frame, interpret(term->right, frame));
        return val;
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
            return interpret(term->right, frame);
        case ';':
            interpret(term->left, frame);
            return interpret(term->right, frame);
        default:
            return create_value();
    }
}

static VALUE* interpret_if(NODE* term, FRAME* frame) {
    if(interpret(term->left, frame)) {
        puts("Condition was true!");
        return NULL;
    } else {
        puts("Condition was false!");
        return NULL;
    }
    return NULL;
}

VALUE* interpret(NODE* term, FRAME* frame) {
    if(term == NULL) return NULL;
    switch(term->type) {
        case LEAF:
            return (VALUE*) interpret_leaf(term, frame);
        case RETURN:
            { VALUE* val = (VALUE*) interpret(term->left, frame);
                return val;
            }
        case EQ_OP:
            return (VALUE*) interpret_equality(term, frame);
        case NE_OP:
            return (VALUE*) interpret_inequality(term, frame);
        case IF:
            return (VALUE*) interpret_if(term, frame);
        default:
            if(ispunct(term->type)) {
                return (VALUE*) interpret_punct(term, frame);
            } else {
                //printf("%s was passed over (not yet implemented)\n", named(term->type));
                if(term->type == '~') return interpret_punct(term, frame);
                interpret(term->left, frame);
                interpret(term->right, frame);
                break;
            }
    }
}

void cleanup_interpret(NODE* tree, FRAME* frame) {
    //free(frame);
    if(tree == NULL) return;
    //cleanup_interpret(tree->left, NULL);
    //cleanup_interpret(tree->right, NULL);
    //free(tree);
}
