#include <stdio.h>
#include <ctype.h>
#include "C.tab.h"

#include <stdlib.h>
#include <stdbool.h>
#include "gentac.h"

extern char* named(int);

extern VALUE* frame_check(TOKEN*, FRAME*);
extern VALUE* frame_assign(TOKEN*, FRAME*, VALUE*);
extern VALUE* frame_name(TOKEN*, FRAME*);

char* named_ops(int i) {
    //printf("Called named ops with i: %d\n", i);
    switch(i) {
        case 0:
            return "+";
        case 1:
            return "-";
        case 2:
            return "*";
        case 3:
            return "/";
        case 4:
            return "%";
        case 5:
            return "CALL";
        case 6:
            return "RET";
        case 7:
            return "";
        case 8:
            return ">";
        case 9:
            return "<";
        default:
            return "???";
    }
}

typedef struct block {
    int nvars;
} BLOCK;

typedef struct call {
    TOKEN* name;
    int arity;
} CALL;

int latest_t = -1;
int latest_a = -1;

TOKEN* get_reg(char type) {
    TOKEN* t = new_token(type);
    if(type == 't') t->value = ++latest_t;
    if(type == 'a') t->value = ++latest_a;
    t->lexeme = NULL;
    t->next = NULL;
    return t;
}

TAC* create_tac(void) {
    TAC* tac = malloc(sizeof(TAC));
    tac->op = 0;
    tac->src1 = NULL;
    tac->src2 = NULL;
    tac->dst = NULL;
    tac->next = NULL;
    return tac;
}

TAC* head = NULL;

void add_tac(TAC* tac) {
    if(head == NULL) {
        head = tac;
        return;
    }
    TAC* current = head;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = tac;
    return;
}

void print_token(TOKEN* tok) {
    if(tok->type == 't' || tok->type == 'a') {
        print_reg(tok);
    } else if(tok->type == CONSTANT) {
        printf("%d", tok->value);
    } else if(tok->type == IDENTIFIER) {
        printf("%s", tok->lexeme);
    } else {
        printf("%s", tok->lexeme);
    }
}

void print_reg(TOKEN* tok) {
    if(tok->type == 't') printf("t%d", tok->value);
    if(tok->type == 'a') printf("a%d", tok->value);
}

void print_tac(TAC* tac) {
    if(tac->dst) {
        print_token(tac->dst);
        printf(" = ");
    }
    if(tac->src1) print_token(tac->src1);
    printf(" %s ", named_ops(tac->op));
    if(tac->src2) print_token(tac->src2);
    puts(" ");
    return;
}

void print_tac_program(TAC* tac) {
    puts("TAC PROGRAM:");
    if(tac == NULL) return;
    TAC* current = tac;
    while(current != NULL) {
        print_tac(current);
        current = current->next;
    }
    return;
}

TAC* leaf_to_token(NODE* term, FRAME* frame) {
    TOKEN* t = (TOKEN*) term->left;
    //printf("%d  %s  %d\n", t->type, t->lexeme, t->value);
    if(t == NULL) return NULL;
    TAC* tac = create_tac();
    tac->src1 = t;
    return tac;
}

TAC* gen_tac_return(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = RET;
    TAC* child = gen_tac(term->left, frame);
    tac->src1 = child->dst ? child->dst : child->src1;
    //printf("Return TAC generated\n");
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_div(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = DIV;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->src1 = left->dst ? left->dst : left->src1;
    tac->src2 = right->dst ? right->dst : right->src1;
    tac->dst = get_reg('t');
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_sub(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = SUB;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->src1 = left->dst ? left->dst : left->src1;
    tac->src2 = right->dst ? right->dst : right->src1;
    tac->dst = get_reg('t');
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_add(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = ADD;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->src1 = left->dst ? left->dst : left->src1;
    tac->src2 = right->dst ? right->dst : right->src1;
    tac->dst = get_reg('t');
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_mul(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = MUL;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->src1 = left->dst ? left->dst : left->src1;
    tac->src2 = right->dst ? right->dst : right->src1;
    tac->dst = get_reg('t');
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_mod(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = MOD;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->src1 = left->dst ? left->dst : left->src1;
    tac->src2 = right->dst ? right->dst : right->src1;
    tac->dst = get_reg('t');
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_greater(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = GREATER;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->src1 = left->dst ? left->dst : left->src1;
    tac->src2 = right->dst ? right->dst : right->src1;
    tac->dst = get_reg('t');
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_lesser(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = LESSER;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->src1 = left->dst ? left->dst : left->src1;
    tac->src2 = right->dst ? right->dst : right->src1;
    tac->dst = get_reg('t');
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_assign(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = STORE;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->dst = left->dst ? left->dst : left->src1;
    tac->src1 = right->dst ? right->dst : right->src1;
    //print_tac(tac);
    add_tac(tac);
    return tac;
}

TAC* gen_tac_punct(NODE* term, FRAME* frame) {
    char operator = term->type;
    switch(operator) {
        case '/':
            return gen_tac_div(term, frame);
        case '%':
            return gen_tac_mod(term, frame);
        case '*':
            return gen_tac_mul(term, frame);
        case '+':
            return gen_tac_add(term, frame);
        case '-':
            return gen_tac_sub(term, frame);
        case '>':
            return gen_tac_greater(term, frame);
        case '<':
            return gen_tac_lesser(term, frame);
        case '=':
            return gen_tac_assign(term, frame);
        case '~':
            return gen_tac(term->right, frame);
        case ';':
            gen_tac(term->left, frame);
            return gen_tac(term->right, frame);
        default:
            return create_tac();
    }
}

TAC* gen_tac(NODE* term, FRAME* frame) {
    if(term == NULL) return NULL;
    switch(term->type) {
        case LEAF:
            return leaf_to_token(term, frame);
        case RETURN:
            return gen_tac_return(term, frame);
        default:
            if(ispunct(term->type)) {
                return (TAC*) gen_tac_punct(term, frame);
            } else {
                //printf("Cannot generate TAC from %s yet!\n", named(term->type));
                gen_tac(term->left, frame);
                gen_tac(term->right, frame);
                break;
            }
    }
}

TAC* run_gen_tac(NODE* term, FRAME* frame) {
    gen_tac(term, frame);
    return head;
}

void cleanup_gen_tac(TAC* head) {
    TAC* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }

}
