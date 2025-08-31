find_package(benchmark REQUIRED)
add_executable(second_sorting
        sorting/main.cpp
)
target_link_libraries(second_sorting benchmark::benchmark)
add_executable(second_serializer
        serializer/ISerializer.h
        serializer/main.cpp        serializer/Serializers.h
        serializer/IVehicle.h
)
find_package(nlohmann_json REQUIRED)
target_link_libraries(second_serializer PRIVATE ${nlohmann_json_LIBRARIES})
