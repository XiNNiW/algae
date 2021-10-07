cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)

project(algae)
include(CMakeDependentOption)
include(GNUInstallDirs)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )

set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

#GTEST
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)


find_library(
        FFTW_DOUBLE_LIB
        NAMES "fftw3"
        PATHS ${PKG_FFTW_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
        )
find_path(FFTW_INCLUDE_DIRS
    NAMES "fftw3.h"
    PATHS ${PKG_FFTW_INCLUDE_DIRS} ${INCLUDE_INSTALL_DIR}
)

add_library(FFTW::Double INTERFACE IMPORTED)
set_target_properties(FFTW::Double
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FFTW_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${FFTW_DOUBLE_LIB}"
    )

#Algae (DSP)
add_library(algae STATIC
             ${PROJECT_SOURCE_DIR}/src/algae.cpp )
set_target_properties(algae PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/src/algae.h)
target_include_directories(algae PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(algae
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:include>"
)
target_link_libraries(algae FFTW::Double)

set(cmake_package_name algae)
# set(targets_export_name ${cmake_package_name}Targets CACHE INTERNAL "")
set(targets_export_name ${cmake_package_name} CACHE INTERNAL "")
install(TARGETS algae EXPORT algaeTargets
    # NAMESPACE ${cmake_package_name}::
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
install(TARGETS algae
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${PROJECT_SOURCE_DIR}/src/algae.h DESTINATION include)

# install(EXPORT algae
#         FILE algae.cmake
#         NAMESPACE algae::
#         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${cmake_package_name}
# )


configure_file(algae.pc.in algae.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/algae.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

file(GLOB_RECURSE TEST_FILES
  "${TEST_DIR}/**/*_test.cpp",
  "${TEST_DIR}/*_test.cpp"

)

#Tests
add_executable( AlgaeTests 
${TEST_FILES}
    # ${TEST_DIR}/math_functions_test.cpp
    # ${TEST_DIR}/oscillator_test.cpp
    # ${TEST_DIR}/dc_blocker_test.cpp
    # ${TEST_DIR}/audio_block_test.cpp
    # ${TEST_DIR}/control_test.cpp
    # ${TEST_DIR}/units_test.cpp
)
# target_include_directories(AlgaeTests PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(AlgaeTests algae)
target_link_libraries(AlgaeTests GTest::GTest GTest::Main)
gtest_discover_tests(AlgaeTests)
