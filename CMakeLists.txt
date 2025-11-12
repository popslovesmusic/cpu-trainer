cmake_minimum_required(VERSION 3.20)

project(surrogate_cpu_trainer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
include(CheckCXXSourceCompiles)

option(SUR_ENABLE_AVX2 "Enable AVX2 kernels" ON)
option(SUR_ENABLE_OPENMP "Enable OpenMP parallelism" ON)

set(SUR_COMPILER_HAS_AVX2 OFF)
if(SUR_ENABLE_AVX2)
  message(STATUS "Checking whether the compiler supports AVX2 intrinsics")
  set(_sur_saved_cxx_flags "${CMAKE_CXX_FLAGS}")
  set(_sur_saved_required_flags "${CMAKE_REQUIRED_FLAGS}")
  set(_sur_saved_try_compile_keep_files "${CMAKE_TRY_COMPILE_KEEP_FILES}")
  set(CMAKE_TRY_COMPILE_KEEP_FILES TRUE)
  if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} /arch:AVX2")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mfma")
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -mavx2 -mfma")
  endif()

  message(STATUS "Using CXX flags '${CMAKE_CXX_FLAGS}' for AVX2 probe")
  message(STATUS "Using required flags '${CMAKE_REQUIRED_FLAGS}' for AVX2 probe")

  check_cxx_source_compiles(
    "#ifndef __AVX2__\n#error \"AVX2 not supported\"\n#endif\nint main(){return 0;}"
    SUR_COMPILER_HAS_AVX2)

  if(NOT SUR_COMPILER_HAS_AVX2)
    set(_sur_avx2_probe "${CMAKE_BINARY_DIR}/CMakeFiles/sur_check_avx2.cpp")
    file(WRITE "${_sur_avx2_probe}" "#include <immintrin.h>\nint main(){\n  __m256 a = _mm256_set1_ps(1.0f);\n  __m256 b = _mm256_set1_ps(2.0f);\n  (void)_mm256_add_ps(a, b);\n  return 0;\n}\n")
    set(_sur_avx2_command "${CMAKE_CXX_COMPILER}")
    set(_sur_avx2_args)
    if(CMAKE_CXX_COMPILER_ARG1)
      list(APPEND _sur_avx2_args "${CMAKE_CXX_COMPILER_ARG1}")
    endif()
    list(APPEND _sur_avx2_args "-std=c++20")
    if(CMAKE_CXX_FLAGS)
      separate_arguments(_sur_base_flags NATIVE_COMMAND "${CMAKE_CXX_FLAGS}")
      list(APPEND _sur_avx2_args ${_sur_base_flags})
    endif()
    list(APPEND _sur_avx2_args "-c" "${_sur_avx2_probe}" "-o" "${CMAKE_BINARY_DIR}/CMakeFiles/sur_check_avx2.o")
    if(MSVC)
      list(APPEND _sur_avx2_args "/arch:AVX2")
    else()
      list(APPEND _sur_avx2_args "-mavx2" "-mfma")
    endif()
    execute_process(COMMAND ${_sur_avx2_command} ${_sur_avx2_args}
                    RESULT_VARIABLE _sur_avx2_result
                    OUTPUT_QUIET ERROR_QUIET)
    if(_sur_avx2_result EQUAL 0)
      set(SUR_COMPILER_HAS_AVX2 TRUE)
      message(STATUS "AVX2 fallback probe succeeded")
    else()
      message(STATUS "AVX2 fallback probe failed with code ${_sur_avx2_result}")
    endif()
  endif()

  set(CMAKE_CXX_FLAGS "${_sur_saved_cxx_flags}")
  set(CMAKE_REQUIRED_FLAGS "${_sur_saved_required_flags}")
  set(CMAKE_TRY_COMPILE_KEEP_FILES "${_sur_saved_try_compile_keep_files}")
  if(SUR_COMPILER_HAS_AVX2)
    message(STATUS "AVX2 intrinsics supported")
  else()
    message(STATUS "AVX2 intrinsics not supported")
  endif()
endif()

if(SUR_ENABLE_OPENMP)
  find_package(OpenMP)
endif()
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs

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

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
if(SUR_ENABLE_OPENMP AND OpenMP_CXX_FOUND)
  target_link_libraries(surrogate PUBLIC OpenMP::OpenMP_CXX)
  target_compile_definitions(surrogate PUBLIC SUR_HAS_OPENMP=1)
elseif(SUR_ENABLE_OPENMP)
  message(WARNING "OpenMP requested but not found; building without parallel support.")
endif()

if(SUR_COMPILER_HAS_AVX2)
  target_compile_definitions(surrogate PUBLIC SUR_HAS_AVX2=1)
  if(MSVC)
    target_compile_options(surrogate PRIVATE /arch:AVX2)
  else()
    target_compile_options(surrogate PRIVATE -mavx2 -mfma)
  endif()
elseif(SUR_ENABLE_AVX2)
  message(WARNING "AVX2 requested but the compiler does not support it; falling back to portable kernels.")
endif()

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
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
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
    tests/test_loss.cpp
    tests/test_optimizer.cpp
    tests/test_model.cpp
    tests/test_dataloader.cpp
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
=======
    tests/test_persist.cpp
>>>>>>> theirs
=======
    tests/test_persist.cpp
>>>>>>> theirs
=======
    tests/test_persist.cpp
>>>>>>> theirs
=======
    tests/test_persist.cpp
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
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
set(SURROGATE_TEST_SOURCES
    test_tensor
    test_mathkernels
    test_layers
    test_loss
    test_optimizer
    test_model
    test_dataloader
    test_persist
    test_gradcheck
    test_trainer
)

set(SURROGATE_TEST_TARGETS)
foreach(test_name IN LISTS SURROGATE_TEST_SOURCES)
    add_executable(${test_name} tests/${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE surrogate)
    list(APPEND SURROGATE_TEST_TARGETS ${test_name})
endforeach()

add_custom_target(surrogate_tests)
add_dependencies(surrogate_tests ${SURROGATE_TEST_TARGETS})

add_executable(surrogate_bench_gemm bench/bench_gemm.cpp)
target_link_libraries(surrogate_bench_gemm PRIVATE surrogate)

add_executable(surrogate_bench_elemwise bench/bench_elemwise.cpp)
target_link_libraries(surrogate_bench_elemwise PRIVATE surrogate)

add_custom_target(surrogate_bench)
add_dependencies(surrogate_bench
    surrogate_bench_gemm
    surrogate_bench_elemwise
)
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs

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
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
    add_test(NAME surrogate_tests COMMAND surrogate_tests)
=======
    foreach(test_target IN LISTS SURROGATE_TEST_TARGETS)
        add_test(NAME ${test_target} COMMAND ${test_target})
    endforeach()
>>>>>>> theirs
=======
    foreach(test_target IN LISTS SURROGATE_TEST_TARGETS)
        add_test(NAME ${test_target} COMMAND ${test_target})
    endforeach()
>>>>>>> theirs
=======
    foreach(test_target IN LISTS SURROGATE_TEST_TARGETS)
        add_test(NAME ${test_target} COMMAND ${test_target})
    endforeach()
>>>>>>> theirs
=======
    foreach(test_target IN LISTS SURROGATE_TEST_TARGETS)
        add_test(NAME ${test_target} COMMAND ${test_target})
    endforeach()
>>>>>>> theirs
=======
    foreach(test_target IN LISTS SURROGATE_TEST_TARGETS)
        add_test(NAME ${test_target} COMMAND ${test_target})
    endforeach()
>>>>>>> theirs
=======
    foreach(test_target IN LISTS SURROGATE_TEST_TARGETS)
        add_test(NAME ${test_target} COMMAND ${test_target})
    endforeach()
>>>>>>> theirs
endif()

