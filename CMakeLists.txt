cmake_minimum_required (VERSION 3.15)

project(poker_helper)

add_executable(poker_helper)

target_compile_features(poker_helper PRIVATE cxx_std_20)

target_sources(poker_helper PRIVATE "main.cpp")

install(TARGETS poker_helper)