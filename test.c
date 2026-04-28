
#include <stdio.h>
#include "map.h"

DEFINE_CSTR_MAP(int, IntMap);

int main()
{
    IntMap mymap = IntMap_new(5);
    IntMap_set(mymap, "dude", 56);
    IntMap_set(mymap, "diddyfoiddds", 999);
    printf("%d\n", IntMap_get(mymap, "dude"));
    printf("%d\n", IntMap_get(mymap, "diddyfoiddds"));


    return 0;
}