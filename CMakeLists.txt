cmake_minimum_required(VERSION 3.20)

project(surrogate_cpu_trainer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

add_library(surrogate STATIC
    src/aligned_alloc.cpp
    src/mathkernels_ref.cpp
    src/mathkernels_avx2.cpp
    src/layers_dense.cpp
    src/layers_activation.cpp
    src/loss.cpp
    src/optimizer_sgd.cpp
    src/optimizer_adam.cpp
    src/model.cpp
    src/trainer.cpp
    src/dataloader.cpp
    src/logging.cpp
    src/persist.cpp
)

target_include_directories(surrogate
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(surrogate PUBLIC cxx_std_20)

target_compile_options(surrogate PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
)

add_executable(surrogate_tests
    tests/test_tensor.cpp
    tests/test_mathkernels.cpp
    tests/test_layers.cpp
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
    tests/test_loss.cpp
>>>>>>> theirs
=======
    tests/test_loss.cpp
    tests/test_optimizer.cpp
>>>>>>> theirs
=======
    tests/test_loss.cpp
    tests/test_optimizer.cpp
    tests/test_model.cpp
>>>>>>> theirs
=======
    tests/test_loss.cpp
    tests/test_optimizer.cpp
    tests/test_model.cpp
>>>>>>> theirs
=======
=======
>>>>>>> theirs
    tests/test_loss.cpp
    tests/test_optimizer.cpp
    tests/test_model.cpp
    tests/test_dataloader.cpp
<<<<<<< ours
>>>>>>> theirs
=======
    tests/test_persist.cpp
>>>>>>> theirs
    tests/test_gradcheck.cpp
    tests/test_trainer.cpp
)

target_link_libraries(surrogate_tests PRIVATE surrogate)

add_executable(surrogate_bench
    bench/bench_gemm.cpp
    bench/bench_elemwise.cpp
)

target_link_libraries(surrogate_bench PRIVATE surrogate)

add_executable(example_mlp examples/example_mlp.cpp)

target_link_libraries(example_mlp PRIVATE surrogate)

install(TARGETS surrogate
    EXPORT surrogateTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CTest)
if(BUILD_TESTING)
    add_test(NAME surrogate_tests COMMAND surrogate_tests)
endif()

