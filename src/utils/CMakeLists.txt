add_library(utils OBJECT
    byte_allocator.cpp
)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(utils PRIVATE compiler_flags)