

find_path(LibGtest_INCLUDE_DIR gtest/gtest.h
        PATHS /usr /usr/local/
        )

find_library(LibGtest_LIBRARY
        NAMES gtest
        PATHS /usr /usr/local/ )


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibGtest  DEFAULT_MSG
        LibGtest_LIBRARY LibGtest_INCLUDE_DIR)

