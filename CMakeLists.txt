cmake_minimum_required(VERSION 3.16)
project(dense_index_db)

set(CMAKE_CXX_STANDARD 20)

add_executable(dense_index_db main.cpp file-classes/DenseIndexFile.cpp file-classes/DenseIndexFile.h file-classes/DataSegment.cpp file-classes/DataSegment.h file-classes/IndexSegment.cpp file-classes/IndexSegment.h)