#include "moxie/coroutine.h"
#include "moxlib/memory.h"
#include "string.h"

static int co_yielding(Coroutine *co) {
    if (co->state == 0) { return 0; }
    
    CoroutineYield yield = co->yield;
    switch (yield.type) {
        case YieldType_INDEFINITE: {
            return 1;
        }
        case YieldType_TIMED: {
            return yield.timeout > 0.0;
        }
        default: { return 0; }
    }
}

Coroutine *first_co = NULL;

Coroutine *StartCoroutine(CoroutineFn func, void *userdata) {
    Coroutine *new_co = AllocMem(sizeof(Coroutine));
    
    new_co->function = func;
    new_co->userdata = userdata;
    new_co->state = COROUTINE_DEAD;
    new_co->next = NULL;
    memset(&new_co->yield, 0, sizeof(CoroutineYield));
    
    if (first_co) {
        Coroutine **co = &first_co;
        while (*co) {
            co = &(*co)->next;
        }
        *co = new_co;
    } else {
        first_co = new_co;
    }
    
    return new_co;
}

void CoroutineStep(double dt) {
    Coroutine **ptr = &first_co;
    if (*ptr == NULL) return;
    
    while (*ptr) {
        Coroutine *co = *ptr;
        
        if (co->yield.type == YieldType_TIMED) {
            co->yield.timeout -= dt;
        }
        
        if (!co_yielding(co)) {
            co->yield.type = YieldType_NONE;
            enum CoroutineState state = co->function(co);
            if (state == COROUTINE_DEAD) {
                *ptr = co->next;
                FreeMem(&co);
                continue;
            }
        }
        ptr = &co->next;
    }
}
