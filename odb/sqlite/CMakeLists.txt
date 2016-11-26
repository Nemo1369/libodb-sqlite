set(CURRENT_TARGET odb-sqlite)

list(APPEND ${CURRENT_TARGET}_INCLUDES
    ${PROJECT_SOURCE_DIR}
    )

if (ODB_LIBODB_FOUND)
    list(APPEND ${CURRENT_TARGET}_INCLUDES
        ${ODB_LIBODB_INCLUDE_DIRS}
        )

    list(APPEND ${CURRENT_TARGET}_LIBRARIES
        ${ODB_LIBODB_LIBRARIES}
        )
else()
    list(APPEND ${CURRENT_TARGET}_INCLUDES
        ${ODB_LIBODB_SOURCES_DIR}
        )

    list(APPEND ${CURRENT_TARGET}_LIBRARIES odb)
endif()

list(APPEND ${CURRENT_TARGET}_LIBRARIES sqlite3)

list(APPEND ${CURRENT_TARGET}_UNGROUPED_HEADERS
    auto-handle.hxx
    binding.hxx
    connection-factory.hxx
    connection.hxx
    connection.ixx
    container-statements.hxx
    container-statements.txx
    database.hxx
    database.ixx
    error.hxx
    exceptions.hxx
    forward.hxx
    no-id-object-result.hxx
    no-id-object-result.txx
    no-id-object-statements.hxx
    no-id-object-statements.txx
    polymorphic-object-result.hxx
    polymorphic-object-result.txx
    polymorphic-object-statements.hxx
    polymorphic-object-statements.txx
    prepared-query.hxx
    query-dynamic.hxx
    query-dynamic.ixx
    query-dynamic.txx
    query.hxx
    query.ixx
    query.txx
    section-statements.hxx
    section-statements.txx
    simple-object-result.hxx
    simple-object-result.txx
    simple-object-statements.hxx
    simple-object-statements.ixx
    simple-object-statements.txx
    sqlite-types.hxx
    statement-cache.hxx
    statement-cache.txx
    statement.hxx
    statements-base.hxx
    tracer.hxx
    traits-calls.hxx
    traits.hxx
    transaction-impl.hxx
    transaction-impl.ixx
    transaction.hxx
    transaction.ixx
    version.hxx
    view-result.hxx
    view-result.txx
    view-statements.hxx
    view-statements.txx
    )

list(APPEND ${CURRENT_TARGET}_DETAILS_HEADERS
    details/config.hxx
    details/conversion.hxx
    details/export.hxx
    details/options.hxx
    details/options.ixx
    )

list(APPEND ${CURRENT_TARGET}_DETAILS_BUILD2_HEADERS
    details/build2/config.h
    details/build2/config-vc.h
    )

list(APPEND ${CURRENT_TARGET}_UNGROUPED_SOURCES
    connection.cxx
    connection-factory.cxx
    database.cxx
    error.cxx
    exceptions.cxx
    prepared-query.cxx
    query.cxx
    query-dynamic.cxx
    query-const-expr.cxx
    simple-object-statements.cxx
    statement.cxx
    statements-base.cxx
    statement-cache.cxx
    tracer.cxx
    traits.cxx
    transaction.cxx
    transaction-impl.cxx
    )

list(APPEND ${CURRENT_TARGET}_DETAILS_SOURCES
    details/options.cxx
    )

list(APPEND ${CURRENT_TARGET}_HEADERS
    ${${CURRENT_TARGET}_UNGROUPED_HEADERS}
    ${${CURRENT_TARGET}_DETAILS_HEADERS}
    )

list(APPEND ${CURRENT_TARGET}_SOURCES
    ${${CURRENT_TARGET}_UNGROUPED_SOURCES}
    ${${CURRENT_TARGET}_DETAILS_SOURCES}
    )

if(UNIX)
    add_definitions(-DODB_THREADS_POSIX=1)
elseif(WIN32)
    add_definitions(-DODB_THREADS_WIN32=1)
endif()

if(BUILD_SHARED_LIBRARIES)
    add_definitions(-DLIBODB_SQLITE_DYNAMIC_LIB)
    add_library(${CURRENT_TARGET} SHARED
        ${${CURRENT_TARGET}_HEADERS}
        ${${CURRENT_TARGET}_SOURCES}
        )
else()
    add_definitions(-DLIBODB_SQLITE_STATIC_LIB)
    add_library(${CURRENT_TARGET} STATIC
        ${${CURRENT_TARGET}_HEADERS}
        ${${CURRENT_TARGET}_SOURCES}
        )
endif()

target_include_directories(${CURRENT_TARGET} PUBLIC
    "$<BUILD_INTERFACE:${${CURRENT_TARGET}_INCLUDES}>"
    )

target_link_libraries(${CURRENT_TARGET} ${${CURRENT_TARGET}_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)

    target_compile_options(${CURRENT_TARGET} PUBLIC $<$<COMPILE_LANGUAGE:C>:-Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Werror-implicit-function-declaration -Wno-missing-braces -Wno-missing-field-initializers -std=gnu99 -fno-strict-aliasing>)

    if(USE_LIBC++)
        target_compile_options(${CURRENT_TARGET} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Werror-implicit-function-declaration -Wno-missing-field-initializers -fno-strict-aliasing -std=gnu++14 -stdlib=libc++>)
    else()
        target_compile_options(${CURRENT_TARGET} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Werror-implicit-function-declaration -Wno-missing-field-initializers -fno-strict-aliasing -std=gnu++14>)
    endif()

    target_compile_options(${CURRENT_TARGET} PUBLIC $<$<CONFIG:DEBUG>:-g>)
endif()

set_target_properties(${CURRENT_TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(${CURRENT_TARGET} PROPERTIES BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(${CURRENT_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(APPLE AND NOT ${CMAKE_TARGET_ARCHITECTURE} STREQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR})
    set_target_properties(${CURRENT_TARGET} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${APPLE_SIGN_IDENTITY})
endif()
