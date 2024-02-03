cmake_minimum_required(VERSION 3.16)
project(Krokiet)

add_executable(Krokiet WIN32
    Source/Main.cpp
    Source/KrokietParser.h
    Source/KrokietParser.cpp
    Source/KrokietRenderer.h
    Source/KrokietRenderer.cpp
    Source/KrokietWindow.h
    Source/KrokietWindow.cpp)

target_link_libraries(Krokiet PUBLIC
    dxgi
    d3d11
    d2d1)

target_compile_features(Krokiet PUBLIC cxx_std_20)
