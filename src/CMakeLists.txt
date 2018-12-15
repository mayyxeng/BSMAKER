set(CMAKE_BUILD_TYPE Debug)




#This is really bad now, I should change the library handling

set(SOURCES
    main.cpp
    Parser.cpp
    Config.cpp
    Units.cpp
    Overlay.cpp
   )
add_executable(BSMaker ${SOURCES})

# Find the libraries that correspond to the LLVM components
# that we wish to use
#llvm_map_components_to_libnames(llvm_libs support core irreader)
target_compile_options(BSMaker PUBLIC -O0 -std=c++14 -pedantic -Wall -fPIC)
# Link against LLVM libraries
