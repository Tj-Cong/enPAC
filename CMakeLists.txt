cmake_minimum_required(VERSION 3.13)
project(enPAC_2020_3_0)

set(CMAKE_CXX_STANDARD 14)
set(LINK_DIR /usr/local/lib)

link_directories(${LINK_DIR})
link_libraries(tcmalloc)

include_directories(.)

#if (CMAKE_BUILD_TYPE MATCHES Debug)
#    add_definitions(-DDEBUG=1)
#endif ()

add_executable(enPAC_2020_3_0
        product.cpp product.h
        Data_Structures.cpp
        Data_Structures.h
        Lexer.cpp
        Lexer.h
        Petri_Net.cpp
        Petri_Net.h
        RG.cpp
        RG.h
        SBA.cpp
        SBA.h
        Syntax_Tree.cpp
        Syntax_Tree.h
        TBA.cpp
        TBA.h
        TGBA.cpp
        TGBA.h
        tinystr.cpp
        tinystr.h
        tinyxml.cpp
        tinyxml.h
        tinyxmlerror.cpp
        tinyxmlparser.cpp
        xml2ltl.cpp
        xml2ltl.h
        main.cpp)


target_link_libraries(enPAC_2020_3_0 tcmalloc)