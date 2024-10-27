#include "../h/syscall_c.h"

using size_t = decltype(sizeof(0));

void *operator new(size_t n)
{
    return mem_alloc(n);
}

void *operator new[](size_t n)//ovo ne smem da koristim u Thread klasi zato sto se unutar mem_alloc-a poziva sistemski poziv
{//ugnezdavanje sistemskih poziva ne sme
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
}

void operator delete[](void *p) noexcept
{
    mem_free(p);
}