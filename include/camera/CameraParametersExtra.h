/*
 * Copyright (C) 2014 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define CAMERA_PARAMETERS_EXTRA_C \
const char CameraParameters::AUTO_EXPOSURE_CENTER_WEIGHTED[] = "center-weighted"; \
const char CameraParameters::AUTO_EXPOSURE_FRAME_AVG[] = "frame-average"; \
const char CameraParameters::AUTO_EXPOSURE_SPOT_METERING[] = "spot-metering"; \
const char CameraParameters::FOCUS_MODE_FACEDETECT[] = "facedetect"; \
const char CameraParameters::FOCUS_MODE_TOUCHAF[] = "touchaf"; \
const char CameraParameters::ISO_50[] = "ISO50"; \
const char CameraParameters::KEY_ANTI_SHAKE_MODE[] = "antishake"; \
const char CameraParameters::KEY_AUTO_CONTRAST[] = "auto-contrast"; \
const char CameraParameters::KEY_AUTO_EXPOSURE[] = "metering"; \
const char CameraParameters::KEY_BEAUTY_MODE[] = "beauty"; \
const char CameraParameters::KEY_BLUR_MODE[] = "blur"; \
const char CameraParameters::KEY_MAX_SHARPNESS[] = "max-sharpness"; \
const char CameraParameters::KEY_MAX_CONTRAST[] = "max-contrast"; \
const char CameraParameters::KEY_MAX_SATURATION[] = "max-saturation"; \
const char CameraParameters::KEY_VINTAGE_MODE[] = "vintagemode"; \
const char CameraParameters::KEY_WDR_MODE[] = "wdr"; \
const char CameraParameters::VINTAGE_MODE_BNW[] = "bnw"; \
const char CameraParameters::VINTAGE_MODE_COOL[] = "cool"; \
const char CameraParameters::VINTAGE_MODE_NORMAL[] = "normal"; \
const char CameraParameters::VINTAGE_MODE_OFF[] = "off"; \
const char CameraParameters::VINTAGE_MODE_WARM[] = "warm"; \
const char CameraParameters::SCENE_MODE_BACKLIGHT[] = "back-light"; \
const char CameraParameters::SCENE_MODE_DAWN[] = "dusk-dawn"; \
const char CameraParameters::SCENE_MODE_DUSKDAWN[] = "dusk-dawn"; \
const char CameraParameters::SCENE_MODE_FALL[] = "fall-color"; \
const char CameraParameters::SCENE_MODE_FALL_COLOR[] = "fall-color"; \
const char CameraParameters::SCENE_MODE_TEXT[] = "text"; \
\
const char CameraParameters::KEY_SUPPORTED_ISO_MODES[] = "iso-values"; \
const char CameraParameters::KEY_FACE_DETECTION[] = "face-detection"; \
const char CameraParameters::KEY_SUPPORTED_FACE_DETECTION[] = "face-detection-values"; \
const char CameraParameters::FACE_DETECTION_OFF[] = "off"; \
const char CameraParameters::FACE_DETECTION_ON[] = "on"; \
const char CameraParameters::KEY_ISO_MODE[] = "iso"; \
const char CameraParameters::KEY_SELECTABLE_ZONE_AF[] = "selectable-zone-af"; \
const char CameraParameters::KEY_SUPPORTED_SELECTABLE_ZONE_AF[] = "selectable-zone-af-values"; \
const char CameraParameters::SELECTABLE_ZONE_AF_AUTO[] = "auto"; \
const char CameraParameters::SELECTABLE_ZONE_AF_SPOT_METERING[] = "spot-metering"; \
const char CameraParameters::SELECTABLE_ZONE_AF_CENTER_WEIGHTED[] = "center-weighted"; \
const char CameraParameters::SELECTABLE_ZONE_AF_FRAME_AVERAGE[] = "frame-average"; \
const char CameraParameters::KEY_PREVIEW_FRAME_RATE_MODE[] = "preview-frame-rate-mode"; \
const char CameraParameters::KEY_SUPPORTED_PREVIEW_FRAME_RATE_MODES[] = "preview-frame-rate-modes"; \
const char CameraParameters::KEY_PREVIEW_FRAME_RATE_AUTO_MODE[] = "frame-rate-auto"; \
const char CameraParameters::KEY_PREVIEW_FRAME_RATE_FIXED_MODE[] = "frame-rate-fixed"; \
const char CameraParameters::KEY_SHARPNESS[] = "sharpness"; \
const char CameraParameters::KEY_SATURATION[] = "saturation"; \
const char CameraParameters::KEY_CONTRAST[] = "contrast"; \
const char CameraParameters::KEY_SCENE_DETECT[] = "scene-detect"; \
const char CameraParameters::KEY_SUPPORTED_SCENE_DETECT[] = "scene-detect-values"; \
const char CameraParameters::SCENE_DETECT_OFF[] = "off"; \
const char CameraParameters::SCENE_DETECT_ON[] = "on"; \
const char CameraParameters::KEY_TOUCH_AF_AEC[] = "touch-af-aec"; \
const char CameraParameters::KEY_SUPPORTED_TOUCH_AF_AEC[] = "touch-af-aec-values"; \
const char CameraParameters::TOUCH_AF_AEC_OFF[] = "touch-off"; \
const char CameraParameters::TOUCH_AF_AEC_ON[] = "touch-on"; \
const char CameraParameters::KEY_LENSSHADE[] = "lensshade"; \
const char CameraParameters::KEY_GPS_LATITUDE_REF[] = "gps-latitude-ref"; \
const char CameraParameters::KEY_GPS_LONGITUDE_REF[] = "gps-longitude-ref"; \
const char CameraParameters::KEY_GPS_ALTITUDE_REF[] = "gps-altitude-ref"; \
const char CameraParameters::KEY_GPS_STATUS[] = "gps-status"; \
const char CameraParameters::KEY_EXIF_DATETIME[] = "exif-datetime"; \
const char CameraParameters::KEY_SUPPORTED_AUTO_EXPOSURE[] = "auto-exposure-values"; \
const char CameraParameters::KEY_SUPPORTED_LENSSHADE_MODES[] = "lensshade-values"; \
const char CameraParameters::LENSSHADE_ENABLE[] = "enable"; \
const char CameraParameters::LENSSHADE_DISABLE[] = "disable"; \
const char CameraParameters::ISO_AUTO[] = "auto"; \
const char CameraParameters::ISO_HJR[] = "ISO_HJR"; \
const char CameraParameters::ISO_100[] = "ISO100"; \
const char CameraParameters::ISO_200[] = "ISO200"; \
const char CameraParameters::ISO_400[] = "ISO400"; \
const char CameraParameters::ISO_800[] = "ISO800"; \
const char CameraParameters::ISO_1600[] = "ISO1600"; \
const char CameraParameters::KEY_HISTOGRAM[] = "histogram"; \
const char CameraParameters::KEY_SUPPORTED_HISTOGRAM_MODES[] = "histogram-values"; \
const char CameraParameters::HISTOGRAM_ENABLE[] = "enable"; \
const char CameraParameters::HISTOGRAM_DISABLE[] = "disable"; \
const char CameraParameters::SKIN_TONE_ENHANCEMENT_ENABLE[] = "enable"; \
const char CameraParameters::SKIN_TONE_ENHANCEMENT_DISABLE[] = "disable"; \
const char CameraParameters::KEY_SKIN_TONE_ENHANCEMENT[] = "skinToneEnhancement"; \
const char CameraParameters::KEY_SUPPORTED_SKIN_TONE_ENHANCEMENT_MODES[] = "skinToneEnhancement-values"; \
const char CameraParameters::PIXEL_FORMAT_YUV420SP_ADRENO[] = "yuv420sp-adreno"; \
const char CameraParameters::PIXEL_FORMAT_RAW[] = "raw"; \
int CameraParameters::getInt64(const char *key) const { return -1; }; \
void CameraParameters::setPreviewFrameRateMode(const char *mode) { set(KEY_PREVIEW_FRAME_RATE_MODE, mode); }; \
const char *CameraParameters::getPreviewFrameRateMode() const { return get(KEY_PREVIEW_FRAME_RATE_MODE); }; \
void CameraParameters::setTouchIndexAec(int x, int y) { }; \
void CameraParameters::getTouchIndexAec(int *x, int *y) const { }; \
void CameraParameters::setTouchIndexAf(int x, int y) { }; \
void CameraParameters::getTouchIndexAf(int *x, int *y) const { }; \
void CameraParameters::setPreviewFpsRange(int minFPS, int maxFPS) { };

#define CAMERA_PARAMETERS_EXTRA_H \
    static const char AUTO_EXPOSURE_CENTER_WEIGHTED[]; \
    static const char AUTO_EXPOSURE_FRAME_AVG[]; \
    static const char AUTO_EXPOSURE_SPOT_METERING[]; \
    static const char FOCUS_MODE_FACEDETECT[]; \
    static const char FOCUS_MODE_TOUCHAF[]; \
    static const char ISO_50[]; \
    static const char KEY_ANTI_SHAKE_MODE[]; \
    static const char KEY_AUTO_CONTRAST[]; \
    static const char KEY_AUTO_EXPOSURE[]; \
    static const char KEY_BEAUTY_MODE[]; \
    static const char KEY_BLUR_MODE[]; \
    static const char KEY_MAX_SHARPNESS[]; \
    static const char KEY_MAX_CONTRAST[]; \
    static const char KEY_MAX_SATURATION[]; \
    static const char KEY_VINTAGE_MODE[]; \
    static const char KEY_WDR_MODE[]; \
    static const char VINTAGE_MODE_BNW[]; \
    static const char VINTAGE_MODE_COOL[]; \
    static const char VINTAGE_MODE_NORMAL[]; \
    static const char VINTAGE_MODE_OFF[]; \
    static const char VINTAGE_MODE_WARM[]; \
    static const char SCENE_MODE_BACKLIGHT[]; \
    static const char SCENE_MODE_DAWN[]; \
    static const char SCENE_MODE_DUSKDAWN[]; \
    static const char SCENE_MODE_FALL[]; \
    static const char SCENE_MODE_FALL_COLOR[]; \
    static const char SCENE_MODE_TEXT[]; \
\
    static const char KEY_SUPPORTED_ISO_MODES[]; \
    static const char KEY_FACE_DETECTION[]; \
    static const char KEY_SUPPORTED_FACE_DETECTION[]; \
    static const char FACE_DETECTION_OFF[]; \
    static const char FACE_DETECTION_ON[]; \
    static const char KEY_ISO_MODE[]; \
    static const char KEY_SELECTABLE_ZONE_AF[]; \
    static const char KEY_SUPPORTED_SELECTABLE_ZONE_AF[]; \
    static const char SELECTABLE_ZONE_AF_AUTO[]; \
    static const char SELECTABLE_ZONE_AF_SPOT_METERING[]; \
    static const char SELECTABLE_ZONE_AF_CENTER_WEIGHTED[]; \
    static const char SELECTABLE_ZONE_AF_FRAME_AVERAGE[]; \
    static const char KEY_PREVIEW_FRAME_RATE_MODE[]; \
    static const char KEY_SUPPORTED_PREVIEW_FRAME_RATE_MODES[]; \
    static const char KEY_PREVIEW_FRAME_RATE_AUTO_MODE[]; \
    static const char KEY_PREVIEW_FRAME_RATE_FIXED_MODE[]; \
    static const char KEY_SHARPNESS[]; \
    static const char KEY_SATURATION[]; \
    static const char KEY_CONTRAST[]; \
    static const char KEY_SCENE_DETECT[]; \
    static const char KEY_SUPPORTED_SCENE_DETECT[]; \
    static const char SCENE_DETECT_OFF[]; \
    static const char SCENE_DETECT_ON[]; \
    static const char KEY_TOUCH_AF_AEC[]; \
    static const char KEY_SUPPORTED_TOUCH_AF_AEC[]; \
    static const char TOUCH_AF_AEC_OFF[]; \
    static const char TOUCH_AF_AEC_ON[]; \
    static const char KEY_LENSSHADE[]; \
    static const char KEY_GPS_LATITUDE_REF[]; \
    static const char KEY_GPS_LONGITUDE_REF[]; \
    static const char KEY_GPS_ALTITUDE_REF[]; \
    static const char KEY_GPS_STATUS[]; \
    static const char KEY_EXIF_DATETIME[]; \
    static const char KEY_SUPPORTED_AUTO_EXPOSURE[]; \
    static const char KEY_SUPPORTED_LENSSHADE_MODES[]; \
    static const char LENSSHADE_ENABLE[]; \
    static const char LENSSHADE_DISABLE[]; \
    static const char ISO_AUTO[]; \
    static const char ISO_HJR[]; \
    static const char ISO_100[]; \
    static const char ISO_200[]; \
    static const char ISO_400[]; \
    static const char ISO_800[]; \
    static const char ISO_1600[]; \
    static const char KEY_HISTOGRAM[]; \
    static const char KEY_SUPPORTED_HISTOGRAM_MODES[]; \
    static const char HISTOGRAM_ENABLE[]; \
    static const char HISTOGRAM_DISABLE[]; \
    static const char SKIN_TONE_ENHANCEMENT_ENABLE[]; \
    static const char SKIN_TONE_ENHANCEMENT_DISABLE[]; \
    static const char KEY_SKIN_TONE_ENHANCEMENT[]; \
    static const char KEY_SUPPORTED_SKIN_TONE_ENHANCEMENT_MODES[]; \
    static const char PIXEL_FORMAT_YUV420SP_ADRENO[]; \
    static const char PIXEL_FORMAT_RAW[]; \
    int getInt64(const char *key) const; \
    void setPreviewFrameRateMode(const char *mode); \
    const char *getPreviewFrameRateMode() const; \
    void setTouchIndexAec(int x, int y); \
    void getTouchIndexAec(int *x, int *y) const; \
    void setTouchIndexAf(int x, int y); \
    void getTouchIndexAf(int *x, int *y) const; \
    void setPreviewFpsRange(int minFPS, int maxFPS);
