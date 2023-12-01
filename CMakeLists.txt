cmake_minimum_required(VERSION 3.20)
project(stardust)

set(STARDUST_HEADERS
    include/stardust/abilities.hpp
    include/stardust/character.hpp
    include/stardust/types.hpp
)

set(STARDUST_SOURCES
    src/abilities.cpp
    src/character.cpp
)

add_library(stardust
    ${STARDUST_HEADERS}
    ${STARDUST_SOURCES}
)
add_library(stardust::stardust ALIAS stardust)

target_include_directories(stardust PUBLIC include)
