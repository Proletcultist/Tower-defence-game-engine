#include "libgamerepo/entity.h"

Image::Image(uint32_t id, std::string name, std::filesystem::path imagepath)
    : Entity(id, EntityType::Image, name) {

  filetype = nullptr;
  data = nullptr;
  data_size = 0;
}
