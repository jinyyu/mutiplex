#include "evcpp/utils.h"

namespace ev
{

bool is_pow_of_two(uint32_t v)
{
    return (v != 0) && ((v & (v - 1)) == 0);
}

uint32_t roundup_pow_of_two(uint32_t v)
{
    if (v < 2) {
        return 2;
    }
    uint32_t ret = 0x00000001;
    for (uint32_t i = 1; i < 32; ++i) {
        if (ret >= v) {
            return ret;
        }
        else {
            ret <<= 1;
        }
    }
}

}