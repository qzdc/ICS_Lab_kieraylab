#include "happy.h"

void* __eh_head();

void test1(int x)
{
    static const char* str = "test1 PASS\n";
    static const int len = 11;
    static __ctx_type ctx[12];
    static int p = 0;
    while (p < len)
    {
        if (x > len)
        {
            p++;
            __ctx_recover(&ctx[p], p);
        }
        int t = __ctx_record(&ctx[x]);
        if (t == 0)
        {
            test1(x + 1);
        }
        else
        {
            assert(t == x);
            printf("%c", str[x - 1]);
        }
    }
}

void test2(__ctx_type* last, int depth)
{
    static int cnt;
    __ctx_type ctx;
    while (++cnt)
    {
        if ((((rand() & 15) > 6) || depth < 20) && depth < 100)
        {
            int t = __ctx_record(&ctx);
            if (t == 0)
            {
                test2(&ctx, depth + 1);
            }
            else
            {
                assert(depth < t && t <= 100);
                if (t == 100)
                {
                    assert(cnt > 200 && cnt < 2000);
                    if (last)
                        __ctx_recover(last, t);
                    else
                        printf("test2 PASS\n");
                    break;
                }
            }
        }
        else
        {
            __ctx_recover(last, depth);
            assert(0);
        }
    }
}

void test3(int x)
{
    static const char* str = "test3 PASS\n";
    static const int len = 11;
    if (x == len)
    {
        throw(str[0]);
        assert(0);
    }
    try
    {
        test3(x + 1);
    }
    catch
    {
        printf("%c", error);
        if (x > 0)
        {
            throw(str[len - x]);
            assert(0);
        }
        else
        {
            assert(__eh_head() == NULL);
        }
    }
}

void test4(int x)
{
    static int cnt;
    while (++cnt < 10000)
    {
        if (rand() % 100 >= x)
        {
            try
            {
                test4(x + 1);
                if (cnt < 10000)
                    continue;
            }
            catch
            {
                assert(error == x);
            }
            assert(cnt <= 10000);
            if (cnt == 10000)
                break;
        }
        else if (rand() & 1)
            throw(x - 1);
        else
            break;
    }
    if (x == 0)
    {
        assert(__eh_head() == NULL);
        printf("test4 PASS\n");
    }
}

void test5(int x)
{
    static const char* str = "test5 PASS\n";
    static const int len = 11;
    if (x == 0)
    {
        struct gen* g = generator(test5, 1);
        while (1)
        {
            try
            {
                printf("%c", send(g, 233));
            }
            catch
            {
                assert(error == ERR_GENEND);
                break;
            }
        }
        genfree(&g);
    }
    else
    {
        assert(x == 1);
        for (int i = 0; i < len; i++)
            assert(yield(str[i]) == 233);
    }
    assert(__eh_head() == NULL);
}

void test6(int x)
{
    static int cnt;
    struct gen* g = NULL;
    while (1)
    {
        if (rand() % 100 >= x)
        {
            try
            {
                assert(cnt == send(g, x + 1));
                assert(cnt <= 10000);
                if (cnt == 10000)
                {
                    throw(ERR_GENEND);
                    assert(0);
                }
                continue;
            }
            catch
            {
                if (error == ERR_GENNIL)
                {
                    g = generator(test6, x + 1);
                }
                else if (error == ERR_GENEND)
                {
                    break;
                }
                else
                {
                    assert(error == x * 2 + 1);
                    genfree(&g);
                }
            }
        }
        else if (rand() & 3)
        {
            assert(yield(++cnt) == x);
        }
        else
        {
            throw(x * 2 - 1);
            assert(0);
        }
    }
    genfree(&g);
    assert(__eh_head() == NULL);
    if (x == 0)
        printf("test6 PASS\n");
}

static void cat()
{
    printf(".................................................\n.***********************************`.......*****\n.*******************************`][.,/@[*********\n.*******************************`./@@/,**********\n.*****************=*********\\`[`=@@@*************\n.^\\=**************=*********=`,@@`=/*************\n.o=^********^*o*`***o]`*,/`]]@@/ o,/**=**********\n.o*^********^*o*****[`]]/@@@@@^ *`*o**=**********\n.^**********,[`*`]/@@@@@@@@@@@ =*****************\n.^***********,,/@@@@@@@@@@@@@^.^*****************\n.^******,/`*\\/@@@@@@@@@@@@@@@@@******************\n.^******`/./@@@@/     \\@@@@@@@/.*****************\n.*******\\//*,@@@@ .. . ,@@@@@@^******************\n.^\\=****`,OOOOo]`[\\.... @@@@@@^,*****************\n.^o=o***OOOOOOo`*...\\`..@@@@@@^=*****************\n.^o==^=OOOOOOOo`*.... ,\\,@@@@@*=oooooooooooooooo=\n.^o==`O@@@@@@OO``......    \\@@,`*oooo^/oooooooooo\n.^o=`O@@@@@@@OOoo*...        ,\\*oooooooooooooooo\n.^o*=@@@@@@@@@OO]`...     ,/   =^o`oooooooooooooo\n.^o*@@@@@@@@@OOOO[..   ,/`      \\oooooooooooooooo\n=`/=@@@@@O@@OOo\\*....//         ,\\ooooooOOOOooooo\n=^^=@@@@OOOOO/`*.. =@^           =OOOOOOooooooooO\n.^^/@@@@OOO^^,`...O@^             =^OOOOOOOOooooo\n.`^O@@@OOO\\==^.../@@               \\OOOOOOOOooooo\n.^=@O@OOO\\O^....@@@^                \\OOOOOOOOOOOO\n");
}

int main()
{
    srand(time(NULL));
    test1(0);
    test2(NULL, 0);
    test3(0);
    test4(0);
    test5(0);
    test6(0);
    try throw(printf("Congratulations!\n"));
    catch cat();
    return 0;
}