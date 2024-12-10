cmake_minimum_required(VERSION 3.26)
project(ProjetC C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

# SDL2 Configuration
set(SDL2_DIR "C:/SDL2-2.30.9/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED PATHS ${SDL2_DIR})
include_directories(${SDL2_INCLUDE_DIRS})

# SDL2_ttf Configuration
set(SDL2_TTF_DIR "C:/SDL2_ttf-2.20.2/x86_64-w64-mingw32")
include_directories("${SDL2_TTF_DIR}/include/SDL2")
link_directories("${SDL2_TTF_DIR}/lib")

# SDL2_Mixer Configuration
set(SDL2_MIXER_DIR "C:/SDL2_mixer-2.8.0/x86_64-w64-mingw32")
include_directories("${SDL2_MIXER_DIR}/include")
link_directories("${SDL2_MIXER_DIR}/lib")

# Add source files
add_executable(ProjetC main.c
    text_field.h
    text_field.c
    graphics.h
    graphics.c
    audio.h
    audio.c
    question.c
    random_audio.c)

# Link libraries
target_link_libraries(ProjetC ${SDL2_LIBRARIES} SDL2_ttf SDL2_mixer)