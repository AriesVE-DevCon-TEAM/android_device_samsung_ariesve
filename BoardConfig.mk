# Copyright (C) 2012 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# This file sets variables that control the way modules are built
# thorughout the system. It should not be used to conditionally
# disable makefiles (the proper mechanism to control what gets
# included in a build is to use PRODUCT_PACKAGES in a product
# definition file).
#

# WARNING: This line must come *before* including the proprietary
# variant, so that it gets overwritten by the parent (which goes
# against the traditional rules of inheritance).

# Vendor stuff
-include vendor/samsung/ariesve/BoardConfigVendor.mk

TARGET_SPECIFIC_HEADER_PATH := device/samsung/ariesve/include

# Platform
TARGET_BOOTLOADER_BOARD_NAME := ariesve
TARGET_OTA_ASSERT_DEVICE := ariesve,GT-I9001
TARGET_BOARD_PLATFORM := msm7x30
TARGET_NO_BOOTLOADER := true

# Architecture
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_VARIANT := scorpion
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_SMP := false

# Use dlmalloc instead of jemalloc for mallocs on low-ram target kernels
MALLOC_IMPL := dlmalloc

# Legacy MMAP for pre-lollipop blobs
BOARD_USES_LEGACY_MMAP := true

# Kernel
BOARD_KERNEL_BASE := 0x00400000
BOARD_KERNEL_PAGESIZE := 2048
TARGET_KERNEL_SOURCE := kernel/samsung/msm7x30/
TARGET_KERNEL_CONFIG := ariesve_defconfig

# Enable dex-preoptimization to speed up first boot sequence
WITH_DEXPREOPT := true

# WiFi
BOARD_NO_WIFI_HAL                := true
BOARD_HAVE_SAMSUNG_WIFI          := true
WIFI_BAND                        := 802_11_ABG
WPA_SUPPLICANT_VERSION           := VER_0_8_X
BOARD_WPA_SUPPLICANT_DRIVER      := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_HOSTAPD_DRIVER             := NL80211
BOARD_HOSTAPD_PRIVATE_LIB        := lib_driver_cmd_bcmdhd
BOARD_WLAN_DEVICE                := bcmdhd
WIFI_DRIVER_MODULE_PATH          := "/system/lib/modules/dhd.ko"
WIFI_DRIVER_MODULE_NAME          := "dhd"
WIFI_DRIVER_MODULE_ARG           := "firmware_path=/vendor/firmware/fw_bcmdhd.bin nvram_path=/vendor/firmware/nvram_net.txt"
WIFI_DRIVER_MODULE_AP_ARG        := "firmware_path=/vendor/firmware/fw_bcmdhd_apsta.bin nvram_path=/vendor/firmware/nvram_net.txt"
WIFI_DRIVER_FW_PATH_PARAM        := "/sys/module/dhd/parameters/firmware_path"
WIFI_DRIVER_FW_PATH_STA          := "/vendor/firmware/fw_bcmdhd.bin"
WIFI_DRIVER_FW_PATH_AP           := "/vendor/firmware/fw_bcmdhd_apsta.bin"

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/samsung/ariesve/bluetooth
BOARD_BLUEDROID_VENDOR_CONF := device/samsung/ariesve/bluetooth/vnd_ariesve.txt

# RIL
TARGET_NEEDS_NON_PIE_SUPPORT := true
BOARD_MOBILEDATA_INTERFACE_NAME = "pdp0"
BOARD_USES_LEGACY_RIL := true
BOARD_RIL_CLASS := ../../../device/samsung/ariesve/ril/

# GPS
TARGET_GPS_HAL_PATH := device/samsung/ariesve/gps
BOARD_USES_QCOM_GPS := true
BOARD_VENDOR_QCOM_GPS_LOC_API_HARDWARE := msm7x30
BOARD_VENDOR_QCOM_GPS_LOC_API_AMSS_VERSION := 50000

# Graphics
BOARD_EGL_CFG := device/samsung/ariesve/config/egl.cfg
TARGET_USES_C2D_COMPOSITION := true
TARGET_DISPLAY_INSECURE_MM_HEAP := true
USE_OPENGL_RENDERER := true

# Memory allocation
TARGET_USES_ION := true
BOARD_NEEDS_MEMORYHEAPPMEM := true

# QCOM
COMMON_GLOBAL_CFLAGS += -DBINDER_COMPAT
BOARD_USES_QCOM_HARDWARE := true
BOARD_VENDOR_QCOM_AMSS_VERSION := 6225
TARGET_USE_QCOM_BIONIC_OPTIMIZATION := true

# Power HAL
TARGET_PROVIDES_POWERHAL := true

# Lights HAL
TARGET_PROVIDES_LIBLIGHT := true

# healthd HAL
BOARD_HAL_STATIC_LIBRARIES := libhealthd.qcom

# Camera
COMMON_GLOBAL_CFLAGS += -DSAMSUNG_CAMERA_LEGACY
TARGET_RELEASE_CPPFLAGS += -DNEEDS_VECTORIMPL_SYMBOLS
USE_DEVICE_SPECIFIC_CAMERA := true
BOARD_USES_LEGACY_OVERLAY := true

# Audio
COMMON_GLOBAL_CFLAGS += -DQCOM_ENHANCED_AUDIO
BOARD_HAVE_SAMSUNG_AUDIO := true
BOARD_USES_LEGACY_ALSA_AUDIO := true

# Include an expanded selection of fonts
EXTENDED_FONT_FOOTPRINT := true

# Partitions
BOARD_BOOTIMAGE_PARTITION_SIZE := 5242880
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 7864320
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1447034880
BOARD_USERDATAIMAGE_PARTITION_SIZE := 5146411008
BOARD_CACHEIMAGE_PARTITION_SIZE := 545259520
BOARD_FLASH_BLOCK_SIZE := 131072

# Target filesystem
TARGET_USERIMAGES_USE_EXT4 := true

# Vold
BOARD_VOLD_EMMC_SHARES_DEV_MAJOR := true

# CWM Recovery
TARGET_RECOVERY_DEVICE_DIRS += device/samsung/ariesve
TARGET_RECOVERY_FSTAB := device/samsung/ariesve/ramdisk/fstab.qcom

# TWRP recovery
DEVICE_RESOLUTION := 480x800
TW_TARGET_USES_QCOM_BSP := true
TW_SCREEN_BLANK_ON_BOOT := true
TW_BRIGHTNESS_PATH := /sys/devices/platform/msm_fb.196609/leds/lcd-backlight/brightness
TW_MAX_BRIGHTNESS := 255
TW_CUSTOM_POWER_BUTTON := 107
BOARD_USE_CUSTOM_RECOVERY_FONT:= \"roboto_10x18.h\"
RECOVERY_SDCARD_ON_DATA := true
TW_EXTERNAL_STORAGE_PATH := "/external_sd"
TW_EXTERNAL_STORAGE_MOUNT_POINT := "external_sd"
TW_NO_REBOOT_BOOTLOADER := true
TW_HAS_DOWNLOAD_MODE := true
TW_USE_MODEL_HARDWARE_ID_FOR_DEVICE_ID := true
TW_INCLUDE_FB2PNG := true
TARGET_RECOVERY_PIXEL_FORMAT := "RGBX_8888"

# SELinux
BOARD_SEPOLICY_DIRS += \
    device/samsung/ariesve/sepolicy

BOARD_SEPOLICY_UNION += \
    file_contexts \
    property_contexts \
    bridge.te \
    camera.te \
    device.te \
    dhcp.te \
    domain.te \
    file.te \
    geomagneticd.te \
    healthd.te \
    init.te \
    mac_update.te \
    mediaserver.te \
    orientationd.te \
    platform_app.te \
    property.te \
    rild.te \
    rmt.te \
    surfaceflinger.te \
    system.te \
    system_app.te \
    tee.te \
    ueventd.te \
    vold.te \
    wpa_supplicant.te

# Hardware tunables
BOARD_HARDWARE_CLASS := device/samsung/ariesve/cmhw/
