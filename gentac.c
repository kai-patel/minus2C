#include <stdio.h>
#include <ctype.h>
#include "C.tab.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "gentac.h"

extern char* named(int);

extern VALUE* frame_check(TOKEN*, FRAME*);
extern VALUE* frame_check_reverse(TOKEN*, FRAME*);
extern VALUE* frame_assign(TOKEN*, FRAME*, VALUE*);

static TAC* head = NULL;
static BB* bb_head = NULL;

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
        case 10:
            return "LABEL";
        case 11:
            return "PROC";
        case 12:
            return "ENDPROC";
        case 13:
            return "IF";
        case 14:
            return "GOTO";
        default:
            return "???";
    }
}


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

int latest_label = -1;
TOKEN* get_label() {
    TOKEN* t = new_token(LABEL);
    char* s = malloc(12*sizeof(char));
    //printf("%c%d\n", latest_label % 127 + 97, latest_label / 26);
    sprintf(s, "label_%c%d", latest_label % 127 + 97, ++latest_label / 26);
    t->lexeme = s;
    //printf("new label: %s\n", t->lexeme);
    return t;
}

TAC* create_tac(void) {
    TAC* tac = malloc(sizeof(TAC));
    tac->op = 0;
    //tac->args= NULL;
    tac->src1 = NULL;
    tac->src2 = NULL;
    tac->dst = NULL;
    tac->next = NULL;
    return tac;
}


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

#if 0
void add_bb(BB* bb) {
    if(bb_head == NULL) {
        bb_head = bb;
        return;
    }

    BB* current = bb_head;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = bb;
    return;
}
#endif

void print_token(TOKEN* tok) {
    if(tok->type == 't' || tok->type == 'a') {
        print_reg(tok);
    } else if(tok->type == CONSTANT) {
        printf("%d", tok->value);
    } else if(tok->type == IDENTIFIER) {
        printf("%s", tok->lexeme);
    } else if(tok->type == LABEL) {
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
    if(tac->op == FUNC) {
        printf("%s %s %d\n", named_ops(tac->op), tac->args.proc.name->lexeme, tac->args.proc.arity);
        return;
    }

    if(tac->op == IF_ENUM) {
        printf("%s ", named_ops(tac->op));

        printf("%s ", named_ops(tac->args.test->op));
        if(tac->args.test->src1) {
            print_token(tac->args.test->src1);
            printf(" ");
        }

        if(tac->args.test->src2) {
            print_token(tac->args.test->src2);
            printf(" ");
        }

        print_token(tac->src2);
        puts(" ");
        return;
    }

    if(tac->dst) {
        print_token(tac->dst);
        printf(" = ");
    }
    printf("%s ", named_ops(tac->op));
    if(tac->src1) {
        print_token(tac->src1);
        printf(" ");
    }
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

    VALUE* bound = frame_check_reverse(t, frame);
    if(bound != NULL) {
        t = (TOKEN*) bound;
    }

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

TAC* gen_tac_if(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    //handle first part of if
    tac->op = IF_ENUM;
    TAC* test = gen_tac(term->left, frame);

    // Remove test TAC from list (its been "saved" into the if TAC)
    TAC* current = head;
    while(current->next->next) {
        current = current->next;
    }

    current->next = NULL;

    tac->args.test = test;
    tac->src1 = test->dst;
    tac->src2 = get_label();

    //add condition to TAC list
    add_tac(tac);

    //add TAC for truthy condition
    TAC* truthy;
    if(term->right->type == ELSE) {
        truthy = gen_tac(term->right->left, frame);
    } else {
        truthy = gen_tac(term->right, frame);
    }

    //add GOTO to skip falsey body
    TOKEN* label_end_falsey = get_label();
    TAC* go_tac = create_tac();
    go_tac->op = GOTO;
    go_tac->src2 = label_end_falsey;

    add_tac(go_tac);

    //add label for falsey body
    TAC* label_falsey = create_tac();
    label_falsey->op = LABEL;
    label_falsey->src2 = tac->src2;
    add_tac(label_falsey);

    //add falsey body
    TAC* falsey = NULL;
    if(term->right->type == ELSE) {
        falsey = gen_tac(term->right->right, frame);
    }

    //add label for end of falsey body
    TAC* end_falsey = create_tac();
    end_falsey->op = LABEL;
    end_falsey->src2 = label_end_falsey;
    add_tac(end_falsey);

    return tac;
}

TAC* gen_tac_assign(NODE* term, FRAME* frame) {
    TAC* tac = create_tac();
    tac->op = STORE;
    TAC* left = gen_tac(term->left, frame);
    TAC* right = gen_tac(term->right, frame);
    tac->dst = left->dst ? left->dst : left->src1;
    //tac->src1 = right->dst ? right->dst : right->src1;
    if(right->dst) {
        //if the assignment expression has a register, use it
        tac->src1 = right->dst;
    } else {
        //otherwise manually load into register
        right->dst = get_reg('t');
        TAC* load = create_tac();
        load->op = STORE;
        load->src1 = right->src1;
        load->dst= right->dst;
        add_tac(load);
        tac->src1 = right->dst;
    }

    VALUE* bound = frame_check(tac->src1, frame);
    if(bound != NULL) {
        frame_assign(tac->src1, frame, (VALUE*) tac->dst);
    } else {
        frame_declaration(tac->src1, frame);
        frame_assign(tac->src1, frame, (VALUE*) tac->dst);
    }

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
            gen_tac(term->left, frame);
            return gen_tac(term->right, frame);
        case ';':
            gen_tac(term->left, frame);
            return gen_tac(term->right, frame);
        default:
            return create_tac();
    }
}

TAC* gen_tac_declaration(NODE* term, FRAME* frame) {
    NODE* d = term->left;
    NODE* code = term->right;

    NODE* type = d->left;
    NODE* F = d->right;

    TOKEN* t = (TOKEN*) F->left->left;
    NODE* formals = F->right;
    //printf("Found function: %s\t%p\n", t->lexeme, t);

#if 0
    if(strcmp(t->lexeme, "main") == 0) {
        //puts("Found main, starting program");
        return gen_tac(code, frame);
    }
#endif

    VALUE* bound = frame_check(t, frame);
    if(bound != NULL) {
        printf("ERROR: FUNCTION ALREADY DEFINED\n");
        exit(-1);
    } else {
        VALUE* val = create_value();
        CLOSURE* f = malloc(sizeof(CLOSURE));

        f->name = t;
        f->formals = formals;
        f->frame = frame;
        f->code = code;
        val->v.f = f;
        frame_declaration(t, frame);
        frame_assign(t, frame, val);

        int n_formals = 0;
        NODE* current = formals;
        while(current != NULL) {
            n_formals++;
            current = current->right;
        }

        TAC* tac = create_tac();
        tac->op = FUNC;
        tac->args.proc.name = t;
        tac->args.proc.arity = n_formals;
        add_tac(tac);

        //this used to end here with return gen_tac(code, frame);
        gen_tac(code, frame);

        TAC* tac2 = create_tac();
        tac2->op = END_FUNC;
        tac2->args.proc.name = t;
        add_tac(tac2);
    }

    return NULL;
}

TAC* gen_tac_call(TOKEN* token, NODE* args, FRAME* frame) {
    TAC* tac = create_tac();
    tac->args.call.name = token;

    VALUE* val = frame_check(token, frame);

    CLOSURE* f = val->v.f;
    FRAME* new_frame = frame_extend(frame, f->formals, args);
    new_frame->next = f->frame;

    NODE* iter_args = args;
    int n_args = 0;
    while(iter_args != NULL) {
        n_args++;
        iter_args = iter_args->right;
    }
    tac->args.call.arity = n_args;
    tac->op = CALL_ENUM;
    tac->src1 = token;
    tac->dst = get_reg('t');
    add_tac(tac);
    return tac;
    //return gen_tac(f->code, new_frame);
}

TAC* gen_tac(NODE* term, FRAME* frame) {
    if(term == NULL) return NULL;
    //printf("%s\n", named(term->type));
    switch(term->type) {
        case LEAF:
            return leaf_to_token(term, frame);
        case RETURN:
            return gen_tac_return(term, frame);
        case APPLY:
            return gen_tac_call((TOKEN*) term->left->left, term->right, frame);
        case IF:
            return gen_tac_if(term, frame);
        default:
            if(ispunct(term->type)) {
                return (TAC*) gen_tac_punct(term, frame);
            } else {
                //printf("Cannot generate TAC from %s yet!\n", named(term->type));
                if(term->type == 'D') return gen_tac_declaration(term, frame);
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

BB* get_basic_blocks(TAC* head, FRAME* frame) {
    TAC* current = head;
    while(current != NULL) {
        current;
    }
}

void cleanup_gen_tac(TAC* head) {
    TAC* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        //free(tmp);
    }
}
