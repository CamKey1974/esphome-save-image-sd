#include "esphome.h"
#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"

namespace esphome {
namespace save_image_sd {

class SaveImageSD : public Component {
 public:
  void setup() override {
    if (!SD_MMC.begin("/sdcard", true)) {
      ESP_LOGE("SD", "SD-Karte konnte nicht initialisiert werden!");
    } else {
      ESP_LOGI("SD", "SD-Karte bereit.");
    }
  }

  void capture_to_sd() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      ESP_LOGE("CAM", "Kamera-Frame konnte nicht gelesen werden!");
      return;
    }

    char filename[64];
    time_t now;
    time(&now);
    strftime(filename, sizeof(filename), "/sdcard/foto_%Y%m%d_%H%M%S.jpg", localtime(&now));

    File file = SD_MMC.open(filename, FILE_WRITE);
    if (!file) {
      ESP_LOGE("SD", "Datei konnte nicht geöffnet werden: %s", filename);
    } else {
      file.write(fb->buf, fb->len);
      file.close();
      ESP_LOGI("SD", "Foto gespeichert: %s", filename);
    }

    esp_camera_fb_return(fb);
  }
};

}  // namespace save_image_sd
}  // namespace esphome

