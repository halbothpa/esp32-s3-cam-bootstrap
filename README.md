# ESP32-S3-CAM ESPHome Bootstrap

Public bootstrap and reusable ESPHome source for the GOOUUU/generic ESP32-S3-CAM N16R8 setup.

This repository intentionally contains **no secret values**. It now hosts:

- `adopt.yaml` for ESPHome dashboard adoption
- `packages/camera.yaml` with the camera, flash LED, runtime controls and motion input
- `components/sd_snapshot/` for microSD JPEG snapshots

The package and external component are public, so ESPHome no longer needs a GitHub username or Personal Access Token to compile the adopted configuration.

## Required local ESPHome secrets

Put only the real values in Home Assistant / ESPHome `secrets.yaml`:

- `wifi_ssid`
- `wifi_password`
- `api_encryption_key`
- `ota_password`
- `fallback_ap_password`
- `web_username`
- `web_password`

No `esp32_s3_cam_github_username` or `esp32_s3_cam_github_token` is required.

## Installation

Use the WebSerial installer:

https://halbothpa.github.io/esp32-s3-cam-esphome/

The factory image contains no personal credentials. Wi-Fi can be provisioned through Improv Serial after flashing. ESPHome then imports `adopt.yaml` and resolves the secret values only from the local Home Assistant ESPHome instance.

Security reports: [`SECURITY.md`](SECURITY.md).
