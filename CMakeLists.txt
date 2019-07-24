cmake_minimum_required(VERSION 3.13)
project(enPAC_2020_3_0)

set(CMAKE_CXX_STANDARD 14)

add_executable(enPAC_2020_3_0
        main.cpp
        Petri_Net.h
        Petri_Net.cpp
        tinystr.h
        tinystr.cpp
        tinyxml.h
        tinyxml.cpp
        tinyxmlerror.cpp
        tinyxmlparser.cpp
        xml2ltl.h
        xml2ltl.cpp RG.h RG.cpp
        Data_Structures.h
        Data_Structures.cpp
        Lexer.h
        Lexer.cpp
        TGBA.h
        TGBA.cpp
        TBA.h
        TBA.cpp
        SBA.h
        SBA.cpp
        Syntax_Tree.h
        Syntax_Tree.cpp
        product.h
        product.cpp)