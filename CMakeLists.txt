project(kprinter4)

cmake_minimum_required(VERSION 2.8)

set(KPRINTER4_VERSION "3")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
configure_file(config.h.cmake ${CMAKE_BINARY_DIR}/config.h)

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${KDE4_INCLUDE_DIR}
  ${QT_INCLUDES}
)

set(kprinter4_SRCS
  main.cpp
  fileprinter.cpp
  psdocument.cpp
)

kde4_add_executable(kprinter4 ${kprinter4_SRCS})
target_link_libraries(kprinter4
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KUTILS_LIBS}
  ${KDE4_KPRINTUTILS_LIBS}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

install(TARGETS kprinter4 DESTINATION ${BIN_INSTALL_DIR})

add_subdirectory(po)

macro_display_feature_log()
