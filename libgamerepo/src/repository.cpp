#include <fstream>
#include <filesystem>
#include <expected>
#include <system_error>
#include <unordered_map>
#include <cerrno>

#include "bsoncxx/builder/basic/array.hpp"
#include "bsoncxx/builder/basic/document.hpp"
#include "bsoncxx/types.hpp"
#include "bsoncxx/document/value.hpp"
#include "libgamerepo/repository.h"
#include "libgamerepo/repository_error.h"

std::expected<GameRepository, GameRepositoryCreationError> GameRepository::create(std::filesystem::path root_path) {
    std::error_code err;

    if (!std::filesystem::exists(root_path, err)) {
        if (err) {
            return std::unexpected(FilesystemError(err));
        } else {
            return std::unexpected(DirectoryDoesntExist()); 
        }
    } else if (!std::filesystem::is_directory(root_path, err)) {
        if (err) {
            return std::unexpected(FilesystemError(err));
        } else {
            return std::unexpected(NotADirectory()); 
        }
    } else if (!std::filesystem::is_empty(root_path)) {
        if (err) {
            return std::unexpected(FilesystemError(err));
        } else {
            return std::unexpected(NonEmptyProjectDirectory());
        }
    }

    GameRepository rep = GameRepository(std::unordered_map<uint32_t, bsoncxx::document::value>(), 0, root_path / REPOSITORY_FILENAME);

    // Flush the empty repository, so the file with it is created
    auto flush_res = rep.flush();
    if (!flush_res) {
        return std::unexpected(flush_res.error());
    }

    return std::move(rep);
}

std::expected<void, FilesystemError> GameRepository::flush() const {
    using bsoncxx::builder::basic::kvp;
    
    bsoncxx::types::b_int32 next_id{std::bit_cast<int32_t>(this->next_id)};

    bsoncxx::builder::basic::array entities_builder;
    for (std::pair<uint32_t, bsoncxx::document::value> entity : buffer) {
        entities_builder.append(entity.second);
    }
    bsoncxx::array::value entities = entities_builder.extract();

    bsoncxx::document::value bson = bsoncxx::builder::basic::make_document(kvp("next_id", next_id), kvp("entities", entities));

    std::ofstream file;
    file.exceptions(std::ios::failbit | std::ios::badbit);
    try {
        file.open(root_path / REPOSITORY_FILENAME, std::ios::binary);
        file.write(reinterpret_cast<const char*>(bson.data()), static_cast<std::streamsize>(bson.size()));
        file.close();
    } catch (const std::ios_base::failure& e) {
        return std::unexpected(FilesystemError(e.code()));
    }

    return {};
}

