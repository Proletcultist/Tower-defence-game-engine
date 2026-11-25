
add_library(libgamerepo STATIC)
target_link_libraries(libgamerepo
    PRIVATE bsoncxx_static
)
target_sources(libgamerepo
    PRIVATE
        src/entity.cpp
        src/repository.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES include/libgamerepo/entity.h
              include/libgamerepo/repository.h
              include/libgamerepo/deserialization_error.h
              include/libgamerepo/repository_error.h
)
