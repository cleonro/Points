project(Points)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS})

set(SRCS
    main.cpp
    vector.h vector.cpp
    pointscontroller.h pointscontroller.cpp
)

add_executable(Points ${SRCS})
target_link_libraries(Points ${VTK_LIBRARIES})

