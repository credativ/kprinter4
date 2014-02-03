project(kprinter4)

cmake_minimum_required(VERSION 2.8)

set(KPRINTER4_VERSION "6")
set(LIBSPECTRE_MINIMUM_VERSION "0.2")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
configure_file(config.h.cmake ${CMAKE_BINARY_DIR}/config.h)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)

macro_optional_find_package(LibSpectre)
macro_log_feature(LIBSPECTRE_FOUND "libspectre" "A PostScript rendering library" "http://libspectre.freedesktop.org/wiki/" FALSE "${LIBSPECTRE_MINIMUM_VERSION}" "Support for PS files in okular.")

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${KDE4_INCLUDE_DIR}
  ${QT_INCLUDES}
  ${LIBSPECTRE_INCLUDE_DIR}
)

set(kprinter4_SRCS
  main.cpp
  fileprinter.cpp
  psdocument.cpp
  widgets/printscalingoptionswidget.cpp
)

kde4_add_ui_files(kprinter4_SRCS
  widgets/printscalingoptionswidgetUI.ui
)

kde4_add_executable(kprinter4 ${kprinter4_SRCS})
target_link_libraries(kprinter4
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KUTILS_LIBS}
  ${KDE4_KPRINTUTILS_LIBS}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${LIBSPECTRE_LIBRARY}
)

install(TARGETS kprinter4 DESTINATION ${BIN_INSTALL_DIR})

add_subdirectory(po)

macro_display_feature_log()
