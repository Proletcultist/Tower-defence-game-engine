
add_library(libgamerepo STATIC)
target_sources(libgamerepo
    PRIVATE
        src/entity.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES include/libgamerepo/entity.h
              include/libgamerepo/repository.h
)
