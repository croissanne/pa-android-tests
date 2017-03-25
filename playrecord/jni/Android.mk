LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libportaudio
TARGET_ARCH_ABI := armeabi-v7a
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libportaudio.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
TARGET_PLATFORM := android-19
TARGET_ARCH_ABI := armeabi-v7
LOCAL_MODULE := playrecord
LOCAL_STATIC_LIBRARIES := portaudio
LOCAL_SRC_FILES := playrecord.c
LOCAL_CFLAGS := -g
LOCAL_C_INCLUDES := $(ANDROID_NDK_ROOT)/platforms/$(TARGET_PLATFORM)/arch-arm/usr/include/
LOCAL_LDLIBS := -llog -lOpenSLES -L$(ANDROID_NDK_ROOT)/platforms/$(TARGET_PLATFORM)/arch-arm/usr/lib

include $(BUILD_EXECUTABLE)
