# pa-android-tests
These are some simple console applications for android which you can use to test portaudio with the opensles backend.

An archive version of portaudio is, for now, included in the jni/armeabi folder. You can replace this with a newer version, the examples link statically against the archive.

## building portaudio

If you'd like to replace the included archives with your own you can crosscompile https://github.com/Gundersanne/portaudio_opensles.

I've included a shell script which you can use to crosscompile portaudio. You need an NDK and execute the standalone toolchain script. After that you can use something very similar to the included script.

Simply edit the ANDROID_NDK_ROOT variable, make it point to a standalone instance of the android ndk. More information on how to make a standalone toolchain here: https://developer.android.com/ndk/guides/standalone_toolchain.html under Advanced Method.

I've tested this with standalone toolchain platform android-14, arm architecture. Once the variable is set, you can execute the script and your files should be installed in the android_libs folder.

## building portaudio using cmake

An easier method to crosscompile https://github.com/Gundersanne/portaudio_opensles using cmake, all
you need is the android ndk. Example:

```
# create and change to build dir
mkdir -p build && cd build

# location of the ANDROID_TOOLCHAIN_FILE relative to the unzipped ndk: android-ndk-r23/build/cmake/android.toolchain.cmake
cmake \
    -DANDROID_PLATFORM=android-15 \
    -DANDROID_ABI=armeabi-v7a \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_TOOLCHAIN_FILE \
    ..

# build
make
```

## building the examples

You need an android android NDK, this was tested with the r9b version (http://dl.google.com/android/ndk/android-ndk-r9b-linux-x86_64.tar.bz2). Newer versions or versions for other operating systems are available here https://developer.android.com/ndk/downloads/index.html. To build the examples you do not need a standalone toolchain, just the ndk (containing ndk-build) will do.

To build simply go to the jni folder (the folder containing the Android.mk) and run:

`export ANDROID_NDK_ROOT=/<path-to-your>/android-ndk-<version> && $ANDROID_NDK_ROOT/ndk-build`

## install and run

Now in the newly created libs folder there should be an executable which you can push to your device. For instance:

`adb push sinewave/libs/armeabi/sinetest /data/temp/`

You may need to give it permission to execute using chmod in the adb shell. After that you can just call it from the adb shell. Check logs for some output.

## building and running the java example (Portaudioexample)

Simply import the gradle project into android studio (made with 2.3) and it should work.

Or here is a link to the armv7 apk:
https://github.com/Gundersanne/pa-android-tests/blob/master/docs/armeabi-v7a/app-debug.apk
