/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2015 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/gpu/shader.h"

#include <cinttypes>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "xenia/base/filesystem.h"
#include "xenia/base/math.h"
#include "xenia/base/memory.h"
#include "xenia/base/string.h"
#include "xenia/gpu/ucode.h"

namespace xe {
namespace gpu {
using namespace ucode;

static std::string MakeShaderDumpPath(const std::string& base_path,
                                      const char* path_prefix,
                                      uint64_t data_hash) {
  std::ostringstream stream;
  stream << base_path << "/shader_" << path_prefix << '_' << std::uppercase
         << std::setfill('0') << std::setw(16) << std::hex << data_hash;
  return stream.str();
}

Shader::Shader(ShaderType shader_type, uint64_t ucode_data_hash,
               const uint32_t* ucode_dwords, size_t ucode_dword_count)
    : shader_type_(shader_type), ucode_data_hash_(ucode_data_hash) {
  // We keep ucode data in host native format so it's easier to work with.
  ucode_data_.resize(ucode_dword_count);
  xe::copy_and_swap(ucode_data_.data(), ucode_dwords, ucode_dword_count);
}

Shader::~Shader() = default;

std::string Shader::GetTranslatedBinaryString() const {
  std::string result;
  result.resize(translated_binary_.size());
  std::memcpy(const_cast<char*>(result.data()), translated_binary_.data(),
              translated_binary_.size());
  return result;
}

std::pair<std::string, std::string> Shader::Dump(const std::string& base_path,
                                                 const char* path_prefix) {
  // Ensure target path exists.
  auto target_path = xe::to_wstring(base_path);
  if (!target_path.empty()) {
    target_path = xe::to_absolute_path(target_path);
    xe::filesystem::CreateFolder(target_path);
  }

  const std::string file_name_no_extension =
      MakeShaderDumpPath(base_path, path_prefix, ucode_data_hash_);

  std::string txt_file_name, bin_file_name;

  if (shader_type_ == ShaderType::kVertex) {
    txt_file_name = file_name_no_extension + ".vert";
    bin_file_name = file_name_no_extension + ".bin.vert";
  } else {
    txt_file_name = file_name_no_extension + ".frag";
    bin_file_name = file_name_no_extension + ".bin.frag";
  }

  FILE* f = fopen(txt_file_name.c_str(), "wb");
  if (f) {
    fwrite(translated_binary_.data(), 1, translated_binary_.size(), f);
    fprintf(f, "\n\n");
    auto ucode_disasm_ptr = ucode_disassembly().c_str();
    while (*ucode_disasm_ptr) {
      auto line_end = std::strchr(ucode_disasm_ptr, '\n');
      fprintf(f, "// ");
      fwrite(ucode_disasm_ptr, 1, line_end - ucode_disasm_ptr + 1, f);
      ucode_disasm_ptr = line_end + 1;
    }
    fprintf(f, "\n\n");
    if (!host_disassembly_.empty()) {
      fprintf(f, "\n\n/*\n%s\n*/\n", host_disassembly_.c_str());
    }
    fclose(f);
  }

  f = fopen(bin_file_name.c_str(), "wb");
  if (f) {
    fwrite(ucode_data_.data(), 4, ucode_data_.size(), f);
    fclose(f);
  }

  return {std::move(txt_file_name), std::move(bin_file_name)};
}

}  //  namespace gpu
}  //  namespace xe
