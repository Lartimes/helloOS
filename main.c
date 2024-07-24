#include <string.h>
#include "stdio.h"
int main() {
    char a[40];
    int result = sprintf(a , "aa = %d" , 5);
    printf("%d" ,result);
    printf("%s" ,a);
    sprintf(a)

    return 0;
}
