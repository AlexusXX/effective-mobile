add_executable(first_helloworld
        helloworld/main.cpp
        helloworld/hello.cpp
)
add_executable(first_iterations
        iterations/library.h
        iterations/main.cpp
)
add_executable(first_sorting
        sorting/main.cpp
)
find_package(benchmark REQUIRED)
add_executable(first_average
        average/main.cpp
)
target_link_libraries(first_average benchmark::benchmark)
add_executable(first_copying
        copying/main.cpp
)

add_executable(first_command_processor
        command-processor/main.cpp
)