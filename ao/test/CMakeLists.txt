set(SRCS main.cpp
    api.cpp
    bounds.cpp
    cache.cpp
    contours.cpp
    dual.cpp
    eval_affine.cpp
    eval_interval.cpp
    eval_jacobian.cpp
    eval_array.cpp
    eval_deriv.cpp
    eval_deriv_array.cpp
    eval_feature.cpp
    eval_point.cpp
    feature.cpp
    heightmap.cpp
    marching.cpp
    mesh.cpp
    region.cpp
    solver.cpp
    tape.cpp
    template.cpp
    tree.cpp
    voxels.cpp
    xtree.cpp
    util/shapes.cpp)
set(LIBS ao-kernel)

if (GUILE_FOUND)
    set(SRCS ${SRCS} guile.cpp)
    set(LIBS ${LIBS} guile-ao)
endif(GUILE_FOUND)

add_executable(ao-test ${SRCS})
target_link_libraries(ao-test ${LIBS})
