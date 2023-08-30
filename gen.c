#include "happy.h"

static struct gen
{
    int error;
    int data;
    void* stack;
    __ctx_type ctx;
    struct gen* caller;
    void* eh_list;
    void (*f)(int);
} __gen_0;
struct gen* __this_gen = &__gen_0;

int yield(int value)
{
    if (__this_gen->caller == NULL)
        throw(ERR_GENNIL);
    __this_gen->data = value;


    // __ctx_type yctx;
    // __ctx_record(&yctx);
    __this_gen->caller->data=value;
    int e=__ctx_record(&__this_gen->ctx);
    if(e==0)
        __ctx_recover(&__this_gen->caller->ctx,__this_gen->data);

    // __eh_push(&yctx);
    // __this_gen->eh_list
    // TODO

    return __this_gen->data;
}

int send(struct gen* gen, int value)
{
    if (gen == NULL)
        throw(ERR_GENNIL);
    if (gen->error)
        throw(gen->error);
    gen->data = value;

    // __ctx_type sctx;
    // __ctx_record(&sctx);
    gen->caller = __this_gen;
    int e=__ctx_record(&__this_gen->ctx);
    if(e==0){
        __this_gen = gen;
        __ctx_recover(&gen->ctx,gen->data);
    }
    else
         __this_gen = __this_gen->caller;
    // __ctx_type* ctx_g=__eh_pop();
    // __eh_push(&sctx);
    // __ctx_recover(ctx_g,value);
    // TODO

    if (gen->error)
        throw(gen->error);
    return gen->data;
}

struct gen* generator(void (*f)(int), int arg)
{
    struct gen* g = malloc(sizeof(*g));
    memset(g, 0, sizeof(*g));
    g->f = f;
    g->stack = malloc(8192);


    // __ctx_type genctx;
    // genctx.val[0]=(int)f;
    // genctx.val[2]=(int)g->stack+16;
    // __eh_record(&genctx);
    // __eh_push(&genctx);
    // g->eh_list=__this_gen->eh_list;

    // g->error=0;
    // g->ctx.val[0]=f;
    // g->ctx.val[2]=NULL;
    // g->ctx.rbp
    // g->ctx.retaddress=f;
    // g->ctx.rsp=g->stack+8184; 

    g->ctx.rsp=g->stack+8176;
    g->ctx.retaddress=f; 
    g->ctx.arg_=arg;
    long long* p_returnaddress =g->stack+8184;
    *p_returnaddress = (void(*)(int))throw; 

    // TODO

    return g;
}

void genfree(struct gen** g)
{
    if (*g == NULL)
        throw(ERR_GENNIL);
    free((*g)->stack);
    free(*g);
    *g = NULL;
}

void __eh_push(void* ctx)
{
    assert(ctx != NULL);
    __ctx_type* newele;
    newele=(__ctx_type*)ctx;
    // newele->ctx_val=ctx;
    newele->next=(__ctx_type*)__this_gen->eh_list;
    __this_gen->eh_list=newele;
}

void* __eh_pop()
{
    assert(__this_gen->eh_list != NULL);
    //__this_gen->eh_list=__this_gen->eh_list->next;
    __ctx_type* head=(__ctx_type*)__this_gen->eh_list;
    __this_gen->eh_list=head->next;
    return head;
    // TODO

}

void* __eh_head()
{
    return __this_gen->eh_list;
}

void throw(int error)
{
    if(__this_gen->eh_list==NULL){
        __this_gen->error=ERR_GENEND;
        // __this_gen=__this_gen->caller
        __ctx_recover(&__this_gen->caller->ctx,ERR_GENEND);
    }

    if (error == 0)
        throw(ERR_THROW0);
    __ctx_type* ctx_p=__eh_pop();
    // __ctx_type* ctx_p=(__ctx_type*)__eh_head();
    // __eh_pop();
    __ctx_recover(ctx_p,error);

    assert(0); // shouldn't run here
}
