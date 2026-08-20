# ESP32-S3-CAM ESPHome Bootstrap

This archived public repository is intentionally tiny. It exists only because
ESPHome `dashboard_import` must read a public YAML file.

It contains **no camera implementation, firmware, workflow or credential**.
[`adopt.yaml`](adopt.yaml) references the private implementation through nine
local `!secret` keys.

## Before adoption

Add these keys to the ESPHome `secrets.yaml` on Home Assistant:

- `wifi_ssid`
- `wifi_password`
- `api_encryption_key`
- `ota_password`
- `fallback_ap_password`
- `web_username`
- `web_password`
- `esp32_s3_cam_github_username`
- `esp32_s3_cam_github_token`

The GitHub token must be a Fine-grained PAT restricted to the private
`halbothpa/esp32-s3-cam-esphome` repository with only `Contents: Read-only`.
Never put the token value into this repository or a GitHub Page.

The installation interface is the
[WebSerial Page](https://halbothpa.github.io/esp32-s3-cam-esphome/).
Security reports: [`SECURITY.md`](SECURITY.md).
