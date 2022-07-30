#include "esp_spiffs.h"
#include "wdt.h"
#include "src/Logging.h"

bool spiffs_format(const char* partition_label) {
    disable_core0_WDT();
    esp_err_t err = esp_spiffs_format(partition_label);
    enable_core0_WDT();
    if (err) {
        log_info("SPIFFS format failed - " << esp_err_to_name(err));
        return true;
    }
    return false;
}
bool spiffs_mount() {
    esp_vfs_spiffs_conf_t conf = { .base_path = "/spiffs", .partition_label = "spiffs", .max_files = 2, .format_if_mount_failed = false };

    esp_err_t err = esp_vfs_spiffs_register(&conf);
    if (err) {
        log_debug("Spiffs mount failed " << esp_err_to_name(err));
        return true;
    }
    return false;
}
void spiffs_unmount() {
    esp_vfs_spiffs_unregister("spiffs");
}
