#include "sd_snapshot.h"

#include <utility>

#include "esphome/core/log.h"

namespace esphome::sd_snapshot {

static const char *const TAG = "sd_snapshot";
static constexpr int SD_CLK_PIN = 39;
static constexpr int SD_CMD_PIN = 38;
static constexpr int SD_D0_PIN = 40;
static constexpr uint64_t BYTES_PER_MEBIBYTE = 1024ULL * 1024ULL;

void SdSnapshotComponent::setup() {
  if (this->mount()) {
    ESP_LOGI(TAG, "microSD mounted, size=%llu MB", this->card_size_mb());
  } else {
    ESP_LOGW(TAG, "microSD unavailable: %s", this->last_error_.c_str());
  }
}

void SdSnapshotComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "SD Snapshot:");
  ESP_LOGCONFIG(TAG, "  Pins: CLK=%d, CMD=%d, D0=%d (1-bit SDMMC)", SD_CLK_PIN, SD_CMD_PIN, SD_D0_PIN);
  ESP_LOGCONFIG(TAG, "  Mounted: %s", YESNO(this->mounted_));
  ESP_LOGCONFIG(TAG, "  Saved images: %u", this->saved_images_);
  ESP_LOGCONFIG(TAG, "  Last status: %s", this->last_error_.c_str());
}

float SdSnapshotComponent::get_setup_priority() const { return setup_priority::LATE; }

void SdSnapshotComponent::set_error_(const std::string &message) {
  this->last_error_ = message;
  ESP_LOGW(TAG, "%s", message.c_str());
}

bool SdSnapshotComponent::mount() {
  if (this->mounted_) return true;

  if (!SD_MMC.setPins(SD_CLK_PIN, SD_CMD_PIN, SD_D0_PIN)) {
    this->set_error_("SD_MMC.setPins(39,38,40) failed");
    return false;
  }

  if (!SD_MMC.begin("/sdcard", true, false, SDMMC_FREQ_DEFAULT)) {
    this->set_error_("SD_MMC.begin failed or no readable card");
    return false;
  }

  if (SD_MMC.cardType() == CARD_NONE) {
    SD_MMC.end();
    this->set_error_("no SD card");
    return false;
  }

  if (!SD_MMC.exists("/camera") && !SD_MMC.mkdir("/camera")) {
    SD_MMC.end();
    this->set_error_("cannot create /camera");
    return false;
  }

  this->mounted_ = true;
  this->last_error_ = "OK";
  return true;
}

void SdSnapshotComponent::unmount() {
  if (this->mounted_) SD_MMC.end();
  this->mounted_ = false;
}

bool SdSnapshotComponent::save_jpeg(const uint8_t *data, size_t length, const char *path) {
  this->last_write_ok_ = false;
  this->last_write_bytes_ = 0;

  if (data == nullptr || length == 0) {
    this->set_error_("empty JPEG buffer");
    return false;
  }
  if (path == nullptr || path[0] != '/') {
    this->set_error_("snapshot path must be absolute");
    return false;
  }
  if (!this->mount()) return false;

  File file = SD_MMC.open(path, FILE_WRITE);
  if (!file) {
    this->set_error_(std::string("open failed: ") + path);
    return false;
  }

  const size_t written = file.write(data, length);
  file.flush();
  file.close();

  this->last_write_bytes_ = written;
  this->last_path_ = path;
  this->last_write_ok_ = written == length;
  if (this->last_write_ok_) {
    this->saved_images_++;
    this->last_error_ = "OK";
    ESP_LOGI(TAG, "Saved JPEG %s (%u bytes)", path, static_cast<unsigned>(length));
    return true;
  }

  this->set_error_("short write: " + std::to_string(written) + "/" + std::to_string(length));
  return false;
}

uint64_t SdSnapshotComponent::card_size_mb() {
  if (!this->mount()) return 0;
  return SD_MMC.cardSize() / BYTES_PER_MEBIBYTE;
}

uint64_t SdSnapshotComponent::free_mb() {
  if (!this->mount()) return 0;
  const uint64_t total = SD_MMC.totalBytes();
  const uint64_t used = SD_MMC.usedBytes();
  return total > used ? (total - used) / BYTES_PER_MEBIBYTE : 0;
}

}  // namespace esphome::sd_snapshot
