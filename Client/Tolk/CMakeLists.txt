project(Tolk)

include(ExternalProject)

##################################################
# TOLK
##################################################

# Pass toolchain file through to ExternalProject (needed for MinGW cross-compile)
if (CMAKE_TOOLCHAIN_FILE)
  set (TOLK_TOOLCHAIN_ARG -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
endif()

ExternalProject_Add(tolk-src
  GIT_REPOSITORY    https://github.com/bear101/tolk
  GIT_TAG           1e538e2
  UPDATE_COMMAND    ""
  PATCH_COMMAND     git reset --hard
  COMMAND           sed -i "s/set (TOLK_LINK_FLAGS User32 Ole32 OleAut32)/set (TOLK_LINK_FLAGS user32 ole32 oleaut32 sapi)/" <SOURCE_DIR>/CMakeLists.txt
  CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
                    -DTOLK_WITH_JAVAJNI=OFF
                    -DTOLK_MULTITHREADED=ON
                    -DCMAKE_CXX_FLAGS=-include\ windows.h
                    ${TOLK_TOOLCHAIN_ARG}
  BUILD_COMMAND     ${CMAKE_COMMAND} --build . --config Release
  INSTALL_DIR       ${CMAKE_CURRENT_BINARY_DIR}/Tolk
  INSTALL_COMMAND   ${CMAKE_COMMAND} --build . --target install --config Release
  BUILD_BYPRODUCTS  <INSTALL_DIR>/lib/Tolk.lib
                    <INSTALL_DIR>/lib/libTolk.dll.a
  )
ExternalProject_Get_Property(tolk-src INSTALL_DIR)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(Tolk STATIC IMPORTED GLOBAL)
add_dependencies(Tolk tolk-src)
target_include_directories (Tolk INTERFACE ${INSTALL_DIR}/include)

if (MSVC)
  set_target_properties(Tolk PROPERTIES
    IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/lib/Tolk.lib
    IMPORTED_LOCATION       ${INSTALL_DIR}/lib/Tolk.lib)
else()
  # MinGW: import library is libTolk.dll.a, DLL goes to bin/
  set_target_properties(Tolk PROPERTIES
    IMPORTED_LOCATION ${INSTALL_DIR}/lib/libTolk.dll.a)
endif()

if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  if (MSVC)
    set (TOLK_DLL_FILES ${INSTALL_DIR}/lib/nvdaControllerClient64.dll ${INSTALL_DIR}/lib/SAAPI64.dll ${INSTALL_DIR}/lib/Tolk.dll PARENT_SCOPE)
  else()
    set (TOLK_DLL_FILES ${INSTALL_DIR}/bin/Tolk.dll ${INSTALL_DIR}/lib/nvdaControllerClient64.dll ${INSTALL_DIR}/lib/SAAPI64.dll PARENT_SCOPE)
  endif()
else()
  if (MSVC)
    set (TOLK_DLL_FILES ${INSTALL_DIR}/lib/dolapi32.dll ${INSTALL_DIR}/lib/nvdaControllerClient32.dll ${INSTALL_DIR}/lib/SAAPI32.dll ${INSTALL_DIR}/lib/Tolk.dll PARENT_SCOPE)
  else()
    set (TOLK_DLL_FILES ${INSTALL_DIR}/bin/Tolk.dll ${INSTALL_DIR}/lib/dolapi32.dll ${INSTALL_DIR}/lib/nvdaControllerClient32.dll ${INSTALL_DIR}/lib/SAAPI32.dll PARENT_SCOPE)
  endif()
endif()
