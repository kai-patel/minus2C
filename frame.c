#include "frame.h"
#include "interpret.h"
#include <stdlib.h>
#include <stdio.h>

extern VALUE* interpret(NODE*, FRAME*);

typedef struct binding {
    TOKEN* name;
    VALUE* val;
    BINDING* next;
} BINDING;

VALUE* frame_check(TOKEN* x, FRAME* frame) {
    while(frame != NULL) {
        BINDING* bindings = frame->bindings;
        while(bindings != NULL) {
            if(bindings->name == x) {
                return bindings->val;
            }
            bindings = bindings->next;
        }
        frame = frame->next;
    }
    //fprintf(stderr, "Unbound variable!\n");
    return NULL;
}

VALUE* frame_assign(TOKEN* x, FRAME* frame, VALUE* value) {
    while(frame != NULL) {
        BINDING* bindings = frame->bindings;
        while(bindings != NULL) {
            if(bindings->name == x) {
                bindings->val = value;
                return value;
            }
            bindings = bindings->next;
        }
        frame = frame->next;
    }
    fprintf(stderr, "Could not assign variable!\n");
}

VALUE* frame_declaration(TOKEN* x, FRAME* frame) {
    BINDING* bindings = frame->bindings ;
    BINDING* new = malloc(sizeof(BINDING));
        if(new != 0) { // check allocation worked
            new->name = x;
            new->val = (VALUE*) 0; // temporary
            new->next = bindings ;
            frame->bindings = new ;
            return (VALUE*) 0; // temporary
        }
    fprintf(stderr, "binding allocation failed\n");
}

FRAME* frame_extend(FRAME* frame, NODE* ids, NODE* args) {
    FRAME* new_frame = frame_create();
    BINDING* bindings = NULL;
    for(NODE* ip = ids, *ap = args; (ip !=NULL) && (ap != NULL); ip = ip->right, ap = ap->right) {
        bindings = make_binding(ip->left, interpret(ap->left, frame), bindings);
    }
    new_frame->bindings = bindings;
    return new_frame;
}

FRAME* frame_create(void) {
    FRAME* frame = malloc(sizeof(FRAME));
    frame->bindings = NULL;
    frame->next = NULL;
}

BINDING* make_binding(NODE* tok, VALUE* val, BINDING* bindings) {
    BINDING* new_binding = malloc(sizeof(BINDING));
    new_binding->name = (TOKEN*) tok;
    new_binding->val = val;
    new_binding->next = bindings;
    return new_binding;
}
