add_library(board STATIC src/board.cpp)
target_include_directories(board PUBLIC include)
target_link_libraries(board PUBLIC STM32_Drivers)