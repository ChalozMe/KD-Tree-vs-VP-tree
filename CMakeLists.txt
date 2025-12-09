
cmake_minimum_required(VERSION 3.10)
project(KNNComparison)

set(CMAKE_CXX_STANDARD 17)

add_executable(knn_test
    src/main.cpp
    src/kd_tree.cpp
    src/vp_tree.cpp
)
