#pragma once

#include <filesystem>

namespace sur {

class Model;

void save_model(const Model& model, const std::filesystem::path& path);
void load_model(Model& model, const std::filesystem::path& path);

}  // namespace sur

