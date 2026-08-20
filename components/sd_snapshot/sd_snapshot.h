#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SD_MMC.h>

#include <cstddef>
#include <cstdint>
#include <string>

#include "esphome/core/component.h"

namespace esphome::sd_snapshot {

class SdSnapshotComponent : public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;

  bool mount();
  void unmount();
  bool save_jpeg(const uint8_t *data, size_t length, const char *path);

  uint64_t card_size_mb();
  uint64_t free_mb();
  uint32_t saved_images() const { return this->saved_images_; }
  const std::string &last_path() const { return this->last_path_; }
  const std::string &last_error() const { return this->last_error_; }
  bool last_write_ok() const { return this->last_write_ok_; }
  uint64_t last_write_bytes() const { return this->last_write_bytes_; }

 protected:
  void set_error_(const std::string &message);

  bool mounted_{false};
  bool last_write_ok_{false};
  uint32_t saved_images_{0};
  uint64_t last_write_bytes_{0};
  std::string last_path_{};
  std::string last_error_{"not initialized"};
};

}  // namespace esphome::sd_snapshot
