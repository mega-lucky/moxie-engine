#ifndef _COROUTINE_H
#define _COROUTINE_H


enum YieldType {
    YieldType_NONE,
    YieldType_INDEFINITE,
    YieldType_TIMED,
};

enum CoroutineState {
    COROUTINE_DEAD,
    COROUTINE_ALIVE
};

typedef struct CoroutineYield {
    enum YieldType type;
    union {
        double timeout;
    };
} CoroutineYield;

struct Coroutine;
typedef enum CoroutineState (*CoroutineFn)(struct Coroutine*);
typedef struct Coroutine {
    CoroutineFn function;
    void *userdata;
    int state;
    CoroutineYield yield;
    
    struct Coroutine *next;
} Coroutine;

Coroutine *StartCoroutine(CoroutineFn func, void *userdata);
void CoroutineStep(double dt);

#define BeginCoroutine(co) switch(co->state) {case COROUTINE_DEAD:
#define EndCoroutine(co) } return COROUTINE_DEAD
#define CoroutineResume(co) co->yield.type = YieldType_NONE
#define YieldCoroutine(co) \
    do { \
        co->yield.type = YieldType_INDEFINITE; \
        co->state = __LINE__; \
        return COROUTINE_ALIVE; \
        case __LINE__:; \
    } while (0)
#define CoroutineWait(co, time) \
    do { \
        co->yield.type = YieldType_TIMED; \
        co->yield.timeout = time; \
        co->state = __LINE__; \
        return COROUTINE_ALIVE; \
        case __LINE__:; \
    } while (0)

#endif