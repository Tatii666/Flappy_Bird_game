cmake_minimum_required(VERSION 3.20.2)
project (Game) # Имя проекта

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static") # Для того, чтобы скомпилированная программа работала из под винды

# Часть для SFML
set(SFML_DIR "c:/SFML/lib/cmake/SFML") # Путь до папки библиотеки
set(SFML_STATIC_LIBRARIES TRUE)        # Нужно для того, чтобы не копировать dll-файлы в рабочую папку проекта
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED) # Устанавливаем зависимые компоненты

add_executable(Game main.cpp) # добавляем компилируемые файлы
target_link_libraries(Game sfml-graphics sfml-audio)
