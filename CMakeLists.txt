cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Укажите путь к вашему исходному коду
set(SOURCE_FILES main.cpp)

# Создайте исполняемый файл
add_executable(MyProject ${SOURCE_FILES})