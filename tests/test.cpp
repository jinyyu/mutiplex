#include <stdio.h>
#include <inttypes.h>
#include <libreactor/utils.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <cstdlib>

using namespace reactor;

int main(int argc, char* argv[])
{

    char* p = (char*) malloc(6 * 1024 * 1024);
    if (!p) {
        printf("error");
    }
    else {
        printf("success");
    }

}

