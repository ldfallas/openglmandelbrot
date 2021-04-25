# find_path(
#     GLFW_INSTALL_DIR
#     NAME include 
#     PATHS "C:/software/lib/glfw64/"
# )
# message(Warning ">>>> xx${GLFW_INSTALL_DIR} ${PROJECT_BINARY_DIR}")

if (GLFW_LOCATION MATCHES OFF)
  message("The GLFW_LOCATION option must be specified with the full directory name of the GLFW installation")
endif()

find_library(
    GLFW_LIBRARY
    NAMES glfw3
    # HINTS C:/software/lib/glfw64/lib-vc2019/
    # PATHS C:/software/lib/glfw64/lib-vc2019/

    # PATHS C:/software/lib/glfw64/lib-vc2019/
    HINTS ${GLFW_LOCATION}
    PATH_SUFFIXES lib-vc2019/
    )

add_library(glfw3 STATIC IMPORTED)

# find_path(
#     GLFW_INCLUDE_DIR
#     NAMES glfw3.h
#     HINTS C:/software/lib/glfw64/include
# )
set(GLFW_INCLUDE_DIR "${GLFW_LOCATION}/include")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
   glfw3
   DEFAULT_MSG
   GLFW_LIBRARY
   GLFW_INCLUDE_DIR
)

message(">>${GLFW_INCLUDE_DIR}")
set_target_properties(
    glfw3
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
      IMPORTED_LOCATION ${GLFW_LIBRARY}
)