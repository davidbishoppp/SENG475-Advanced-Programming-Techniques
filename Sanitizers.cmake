# define sanatizer options
option(ENABLE_ASAN "Enable Address Sanitizer (ASan). Default: false" false)
option(ENABLE_UBSAN "Enable Undefined Behavior Sanitizer (UBSan). Default: false" false)

# modify variables accordingly
if(${ENABLE_ASAN})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

if(${ENABLE_UBSAN})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()