FIND_PACKAGE(ITK)
LIST(APPEND ALL_INCLUDE_DIRECTORIES ${ITK_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES ${ITK_LIBRARIES})
link_directories(${ITK_LIBRARY_DIRS})

 
