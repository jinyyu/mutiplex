#include <net4cxx/Logger.h>

int main(int argc, char *argv[])
{
    using namespace net4cxx;
    //set_log_destination("/tmp/test.txt");
    set_log_level(Logger::WARNING);
    LOG_INFO("AAA");

    LOG_WARNING("int %d", 999);

    LOG_ERROR("int %d", 999);
}