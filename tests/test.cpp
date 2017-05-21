

#include <stdio.h>
#include <inttypes.h>
#include <utils.h>

using namespace net;

int main(int argc, char* argv[]) {

  uint32_t i = 0x80000000;

  for(int i = 0; i < 34; ++i) {
    printf("i = %d, v= %d\n",i, roundup_pow_of_two(i));
  }





}

