#include "stdio.h"
#include "stdint.h"

int aa(uint32_t v)
{
    // v-1 => 最低位&最低位 10 -> 01
    //
    return v ^ (v & (v - 1));
}

int main()
{
    // 0b 0000 0000 0000 0000
    // 0b 1101 0010 100
    printf("%d", aa(0b11010010000));
    return 0;
}