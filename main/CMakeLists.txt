set(projectName "electrode_brain_sim")

add_executable(${projectName} main.cpp)
target_include_directories(${projectName} PUBLIC
  ${CADMIUM_ROOT}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_background_noise tests/test_background_noise.cpp)
target_include_directories(test_background_noise PUBLIC
  ${CADMIUM_ROOT}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_neuron tests/test_neuron.cpp)
target_include_directories(test_neuron PUBLIC
  ${CADMIUM_ROOT}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_adder tests/test_adder.cpp)
target_include_directories(test_adder PUBLIC
  ${CADMIUM_ROOT}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_electrode tests/test_electrode.cpp)
target_include_directories(test_electrode PUBLIC
  ${CADMIUM_ROOT}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_top tests/test_top.cpp)
target_include_directories(test_top PUBLIC
  ${CADMIUM_ROOT}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
