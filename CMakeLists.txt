cmake_minimum_required(VERSION 3.13...3.19)
project(DContainers
        VERSION 0.1
        LANGUAGES CXX
        DESCRIPTION "Multi-dimensional containers for C++"
        HOMEPAGE_URL "https://github.com/nR3D/DContainers")

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

add_library(DContainers
        INTERFACE
        include/DContainers/DArray.hpp
        include/DContainers/DVector.hpp
        include/DContainers/Span/Spanning.hpp
        include/DContainers/Span/DSpanning.hpp
        include/DContainers/Span.hpp)

# Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(DContainers::DContainers ALIAS DContainers)

#Set target properties
target_include_directories(DContainers
        INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )

target_compile_features(DContainers INTERFACE cxx_std_20)


# --- Installation instructions ---

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/DContainers)

install(TARGETS DContainers
        EXPORT DContainers-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT DContainers-targets
        FILE
        DContainersTargets.cmake
        NAMESPACE
        DContainers::
        DESTINATION
        ${INSTALL_CONFIGDIR}
        )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/DContainersConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/DContainersConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/DContainersConfig.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
        )

# Install configuration files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/DContainersConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/DContainersConfigVersion.cmake
        DESTINATION ${INSTALL_CONFIGDIR}
        )


# --- Export ---

export(EXPORT DContainers-targets
        FILE ${CMAKE_CURRENT_BINARY_DIR}/DContainersTargets.cmake
        NAMESPACE DContainers::)

# Register package in the User Package Registry
export(PACKAGE DContainers)


# --- Enable Testing ---

enable_testing()
add_subdirectory(test)
