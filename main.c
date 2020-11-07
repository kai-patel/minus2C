#include <stdio.h>
#include <ctype.h>
#include "nodes.h"
#include "C.tab.h"
#include <string.h>

#include <stdlib.h>
#include <stdbool.h>

#include "interpret.h"
#include "frame.h"
#include "gentac.h"

char *named(int t) {

    static char b[100];
    if (isgraph(t) || t==' ') {
      sprintf(b, "%c", t);
      return b;
    }
    switch (t) {
      default: return "???";
    case IDENTIFIER:
      return "id";
    case CONSTANT:
      return "constant";
    case STRING_LITERAL:
      return "string";
    case LE_OP:
      return "<=";
    case GE_OP:
      return ">=";
    case EQ_OP:
      return "==";
    case NE_OP:
      return "!=";
    case EXTERN:
      return "extern";
    case AUTO:
      return "auto";
    case INT:
      return "int";
    case VOID:
      return "void";
    case APPLY:
      return "apply";
    case LEAF:
      return "leaf";
    case IF:
      return "if";
    case ELSE:
      return "else";
    case WHILE:
      return "while";
    case CONTINUE:
      return "continue";
    case BREAK:
      return "break";
    case RETURN:
      return "return";
    }
}

void print_leaf(NODE *tree, int level) {

    TOKEN *t = (TOKEN *)tree;
    int i;
    for (i=0; i<level; i++) putchar(' ');
    if (t->type == CONSTANT) printf("%d\n", t->value);
    else if (t->type == STRING_LITERAL) printf("\"%s\"\n", t->lexeme);
    else if (t) puts(t->lexeme);
}

void print_tree0(NODE *tree, int level) {

    int i;
    if (tree==NULL) return;
    if (tree->type==LEAF) {
      print_leaf(tree->left, level);
    }
    else {
      for(i=0; i<level; i++) putchar(' ');
      printf("%s\n", named(tree->type));
/*       if (tree->type=='~') { */
/*         for(i=0; i<level+2; i++) putchar(' '); */
/*         printf("%p\n", tree->left); */
/*       } */
/*       else */
        print_tree0(tree->left, level+2);
      print_tree0(tree->right, level+2);
    }
}

void print_tree(NODE *tree) {
    print_tree0(tree, 0);
}

//interpret.h externs
extern VALUE* interpret(NODE*, FRAME*);

//frame.h externs
extern VALUE* frame_check(TOKEN* x, FRAME* frame);
extern VALUE* frame_assign(TOKEN* x, FRAME* frame, VALUE* value);
extern VALUE* frame_declaration(TOKEN* x, FRAME* frame);
extern FRAME* frame_create();

//value.h externs
extern void print_value(VALUE*);

//gentac.h externs
extern TAC* run_gen_tac(NODE*, FRAME*);
extern void print_tac_program(TAC*);

//other externs
extern int yydebug;
extern int yyparse(void);
extern NODE* ans;
extern void init_symbtable(void);
extern TOKEN* lookup_token(char*);

void call_interpreter(NODE* tree) {
    FRAME* frame = frame_create();
    //frame = NULL;
    VALUE* result = interpret(tree, frame);
    print_value(result);
    puts(" ");
    return;
}

void call_gentac(NODE* tree) {
    FRAME* frame = frame_create();
    TAC* result = run_gen_tac(tree, frame);
    print_tac_program(result);
    return;
}

void call_compiler(NODE* tree) {
    return;
}

int main(int argc, char** argv) {

    NODE* tree;
    if (argc>1 && strcmp(argv[1],"-d")==0) yydebug = 1;
    init_symbtable();
    //printf("--C COMPILER\n");
    yyparse();
    tree = ans;
    //printf("parse finished with %p\n", tree);
    //print_tree(tree);

    if(argc < 2) {
        print_tree(tree);
        call_interpreter(tree);
    } else if(strcmp(argv[1], "-i") == 0) {
        call_interpreter(tree);
    } else if(strcmp(argv[1], "-t") == 0) {
        call_gentac(tree);
    } else if(strcmp(argv[1], "-c") == 0) {
        call_compiler(tree);
    }

    return 0;
}
