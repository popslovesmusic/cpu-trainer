#include "surrogate/persist.hpp"

<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
<<<<<<< ours
namespace sur {

void save_model(const Model&, const std::filesystem::path&) {}

void load_model(Model&, const std::filesystem::path&) {}
=======
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
=======
>>>>>>> theirs
#include <bit>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "surrogate/layers.hpp"
#include "surrogate/model.hpp"
#include "surrogate/tensor.hpp"

namespace sur {
namespace {

constexpr char kMagic[4] = {'S', 'U', 'R', '0'};
constexpr std::uint32_t kVersion = 1;

struct FileHeader {
  char magic[4];
  std::uint32_t version;
  std::uint8_t little_endian;
  std::uint8_t reserved[3];
  std::uint32_t layer_count;
};

struct LayerHeader {
  std::uint32_t param_count;
};

struct ParamHeader {
  std::int32_t dims[4];
  std::uint64_t size;
};

bool is_little_endian() {
  return std::endian::native == std::endian::little;
}

template <class T>
void write_binary(std::ofstream& out, const T& value) {
  out.write(reinterpret_cast<const char*>(&value), sizeof(T));
  if (!out) {
    throw std::runtime_error("Failed to write persistence data");
  }
}

template <class T>
void read_binary(std::ifstream& in, T& value) {
  in.read(reinterpret_cast<char*>(&value), sizeof(T));
  if (!in) {
    throw std::runtime_error("Failed to read persistence data");
  }
}

std::string build_open_error(const std::filesystem::path& path, const char* action) {
  return std::string("Failed to ") + action + " model file: " + path.string();
}

}  // namespace

void save(const Model& model, const std::filesystem::path& path) {
  const auto& layers = detail::access_layers(model);
  std::vector<std::vector<const Tensor<float>*>> layer_params;
  layer_params.reserve(layers.size());
  for (const auto& layer : layers) {
    std::vector<const Tensor<float>*> params;
    auto span = layer->params();
    params.reserve(span.size());
    for (Tensor<float>* tensor : span) {
      if (!tensor) {
        throw std::runtime_error("Encountered null parameter tensor while accessing model");
      }
      if (!tensor->is_contiguous()) {
        throw std::runtime_error("Model parameters must be contiguous for persistence");
      }
      params.push_back(tensor);
    }
    layer_params.push_back(std::move(params));
  }

  std::ofstream out(path, std::ios::binary | std::ios::trunc);
  if (!out) {
    throw std::runtime_error(build_open_error(path, "open for writing"));
  }

  FileHeader header{};
  std::memcpy(header.magic, kMagic, sizeof(kMagic));
  header.version = kVersion;
  header.little_endian = static_cast<std::uint8_t>(is_little_endian() ? 1 : 0);
  header.reserved[0] = header.reserved[1] = header.reserved[2] = 0;
  header.layer_count = static_cast<std::uint32_t>(layer_params.size());
  write_binary(out, header);

  for (const auto& params : layer_params) {
    LayerHeader layer_header{};
    layer_header.param_count = static_cast<std::uint32_t>(params.size());
    write_binary(out, layer_header);
    for (const Tensor<float>* tensor : params) {
      ParamHeader param_header{};
      const auto& shape = tensor->shape();
      for (std::size_t i = 0; i < 4; ++i) {
        param_header.dims[i] = static_cast<std::int32_t>(shape[i]);
      }
      param_header.size = static_cast<std::uint64_t>(tensor->size());
      write_binary(out, param_header);
    }
  }

  for (const auto& params : layer_params) {
    for (const Tensor<float>* tensor : params) {
      const auto element_count = tensor->size();
      if (element_count == 0) {
        continue;
      }
      const auto byte_count = static_cast<std::streamsize>(element_count * sizeof(float));
      out.write(reinterpret_cast<const char*>(tensor->data()), byte_count);
      if (!out) {
        throw std::runtime_error("Failed to write model parameter data");
      }
    }
  }
}

void load(Model& model, const std::filesystem::path& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    throw std::runtime_error(build_open_error(path, "open for reading"));
  }

  FileHeader header{};
  read_binary(in, header);
  if (std::memcmp(header.magic, kMagic, sizeof(kMagic)) != 0) {
    throw std::runtime_error("Invalid surrogate model file (magic mismatch)");
  }
  if (header.version != kVersion) {
    throw std::runtime_error("Unsupported surrogate model version");
  }
  const bool file_little_endian = header.little_endian != 0;
  if (file_little_endian != is_little_endian()) {
    throw std::runtime_error("Endianness mismatch when loading surrogate model");
  }

  auto& layers = detail::access_layers(model);
  std::vector<std::vector<Tensor<float>*>> layer_params;
  layer_params.reserve(layers.size());
  for (auto& layer : layers) {
    std::vector<Tensor<float>*> params;
    auto span = layer->params();
    params.reserve(span.size());
    for (Tensor<float>* tensor : span) {
      if (!tensor) {
        throw std::runtime_error("Encountered null parameter tensor while accessing model");
      }
      if (!tensor->is_contiguous()) {
        throw std::runtime_error("Model parameters must be contiguous for persistence");
      }
      params.push_back(tensor);
    }
    layer_params.push_back(std::move(params));
  }

  if (header.layer_count != static_cast<std::uint32_t>(layer_params.size())) {
    throw std::runtime_error("Persisted layer count does not match model configuration");
  }

  std::vector<std::vector<std::uint64_t>> stored_sizes(layer_params.size());
  for (std::size_t layer_idx = 0; layer_idx < layer_params.size(); ++layer_idx) {
    LayerHeader layer_header{};
    read_binary(in, layer_header);
    if (layer_header.param_count != static_cast<std::uint32_t>(layer_params[layer_idx].size())) {
      throw std::runtime_error("Persisted parameter count mismatch for layer");
    }
    auto& sizes = stored_sizes[layer_idx];
    sizes.resize(layer_header.param_count);
    for (std::size_t param_idx = 0; param_idx < layer_params[layer_idx].size(); ++param_idx) {
      ParamHeader param_header{};
      read_binary(in, param_header);
      const Tensor<float>* tensor = layer_params[layer_idx][param_idx];
      const auto& shape = tensor->shape();
      for (std::size_t i = 0; i < 4; ++i) {
        if (param_header.dims[i] != shape[i]) {
          throw std::runtime_error("Persisted parameter shape mismatch");
        }
      }
      if (param_header.size != tensor->size()) {
        throw std::runtime_error("Persisted parameter size mismatch");
      }
      sizes[param_idx] = param_header.size;
    }
  }

  for (std::size_t layer_idx = 0; layer_idx < layer_params.size(); ++layer_idx) {
    for (std::size_t param_idx = 0; param_idx < layer_params[layer_idx].size(); ++param_idx) {
      Tensor<float>* tensor = layer_params[layer_idx][param_idx];
      const auto elements = stored_sizes[layer_idx][param_idx];
      if (elements == 0) {
        continue;
      }
      const auto byte_count = static_cast<std::streamsize>(elements * sizeof(float));
      in.read(reinterpret_cast<char*>(tensor->data()), byte_count);
      if (!in) {
        throw std::runtime_error("Failed to read model parameter data");
      }
    }
  }
}
<<<<<<< ours
<<<<<<< ours
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
=======
>>>>>>> theirs
=======
>>>>>>> theirs

}  // namespace sur

