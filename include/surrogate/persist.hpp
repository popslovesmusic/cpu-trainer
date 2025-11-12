#pragma once

#include <filesystem>

namespace sur {

class Model;

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
void save_model(const Model& model, const std::filesystem::path& path);
void load_model(Model& model, const std::filesystem::path& path);
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
void save(const Model& model, const std::filesystem::path& path);
void load(Model& model, const std::filesystem::path& path);

// Backward-compatible aliases for earlier API drafts.
inline void save_model(const Model& model, const std::filesystem::path& path) {
  save(model, path);
}

inline void load_model(Model& model, const std::filesystem::path& path) {
  load(model, path);
}
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs

}  // namespace sur

