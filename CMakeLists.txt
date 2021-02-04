cmake_minimum_required(VERSION 3.14)

project(wakeonlan VERSION 1.0)

file(GLOB WAKEUP_SOURCES "*.c")
file(GLOB WAKEUP_HEADERS "*.h")

add_executable(wakeup ${WAKEUP_SOURCES} ${WAKEUP_HEADERS})
