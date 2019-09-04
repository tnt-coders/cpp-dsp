find_package(Doxygen)

if (${DOXYGEN_FOUND})
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_IMAGE_PATH ${CMAKE_CURRENT_LIST_DIR}/images)

    doxygen_add_docs(openfft_doxygen
        ${PROJECT_SOURCE_DIR}/docs
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
      COMMENT "Generating HTML documentation..."
    )
endif()