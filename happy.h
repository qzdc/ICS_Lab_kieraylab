#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <string.h>

//char __ctx_type[256]; // TODO?
typedef struct _ctx{
    //char ctx_val[128];
    //void val[3];
    void* retaddress;
    void* rbp;
    void* rsp;
    long long arg_;
    struct _ctx* next;
}__ctx_type;
struct gen;

#define ERR_THROW0 -1
#define ERR_GENEND -2
#define ERR_GENNIL -3

int __ctx_record(__ctx_type*);
void __ctx_recover(__ctx_type*, int);

void __eh_push(void*);
void* __eh_pop();
void throw(int);
static inline void __eh_check_cleanup(int* error) {
    int e=*error;
   if(e == 0)
    __eh_pop();// TODO
}

#define try int error __attribute__((cleanup(__eh_check_cleanup))) = 0; \
    __ctx_type ctx_;\
    __eh_push(&ctx_);\
    error=__ctx_record(&ctx_);\
    if(error==0)
// TODO

#define catch else

// TODO

struct gen* generator(void (*)(int), int);
void genfree(struct gen**);
int yield(int);
int send(struct gen*, int);
#define next(gen) send(gen, 0)
