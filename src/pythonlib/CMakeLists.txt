file(GLOB_RECURSE SRC_FILES ../mahjong/*.cpp)
file(GLOB ENTRY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/pythonlib_*.cpp)

add_library(mahjong_pythonlib ${SRC_FILES} ${ENTRY_FILE})
target_link_libraries(mahjong_pythonlib Catch2::Catch2 ${Boost_LIBRARIES} spdlog ${CMAKE_DL_LIBS})