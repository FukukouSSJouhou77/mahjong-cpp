file(GLOB_RECURSE SRC_FILES ../mahjong/*.cpp)
file(GLOB ENTRY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/pythonXlib_*.cpp)
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1)
FetchContent_MakeAvailable(Catch2)
add_subdirectory(pybind11-2.10.3)
pybind11_add_module(mahjong_pythonlib_mod pythonlib_module.cpp ${SRC_FILES})
target_link_libraries(mahjong_pythonlib_mod PRIVATE Catch2::Catch2 ${Boost_LIBRARIES} spdlog ${CMAKE_DL_LIBS})