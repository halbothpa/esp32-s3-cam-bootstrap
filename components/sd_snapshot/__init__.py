import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.esp32 import require_fatfs, require_vfs_dir
from esphome.const import CONF_ID

DEPENDENCIES = ["esp32"]

sd_snapshot_ns = cg.esphome_ns.namespace("sd_snapshot")
SdSnapshotComponent = sd_snapshot_ns.class_("SdSnapshotComponent", cg.Component)


def _require_storage_features(config):
    require_vfs_dir()
    require_fatfs()
    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SdSnapshotComponent),
        }
    ).extend(cv.COMPONENT_SCHEMA),
    _require_storage_features,
)


async def to_code(config):
    cg.add_library("FS", None)
    cg.add_library("SD_MMC", None)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
