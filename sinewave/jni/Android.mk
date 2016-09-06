LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libportaudio
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libportaudio.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
TARGET_PLATFORM := android-9
TARGET_ARCH_ABI := armeabi-v7
LOCAL_MODULE := sinetest
LOCAL_STATIC_LIBRARIES := portaudio
LOCAL_SRC_FILES := sinetest.c
LOCAL_CFLAGS := -g
LOCAL_C_INCLUDES := $(ANDROID_NDK_ROOT)/platforms/$(TARGET_PLATFORM)/arch-arm/usr/include/
LOCAL_LDLIBS := -llog -lOpenSLES -L$(ANDROID_NDK_ROOT)/platforms/$(TARGET_PLATFORM)/arch-arm/usr/lib

include $(BUILD_EXECUTABLE)
