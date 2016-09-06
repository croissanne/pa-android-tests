ANDROID_NDK_ROOT=/home/android/android_standalone # this is made with the standalone toolchain script (https://developer.android.com/ndk/guides/standalone_toolchain.html)
ANDROID_TARGET_ARCH=armeabi-v7a
ANDROID_NDK_TOOLS_PREFIX=arm-linux-androideabi
ANDROID_NDK_TOOLCHAIN_PREFIX=arm-linux-androideabi
ANDROID_ARCHITECTURE=arm
ANDROID_NDK_TOOLCHAIN_VERSION=4.8
ANDROID_NDK_HOST=linux-x86_64
SYSROOT=$ANDROID_NDK_ROOT/sysroot


export ANDROID_NDK_ROOT ANDROID_TARGET_ARCH ANDROID_NDK_TOOLS_PREFIX ANDROID_NDK_TOOLCHAIN_PREFIX ANDROID_ARCHITECTURE ANDROID_NDK_TOOLCHAIN_VERSION

ANDROID_NDK_TOOLS_FULLPREFIX=$ANDROID_NDK_ROOT/bin/$ANDROID_NDK_TOOLS_PREFIX

CC="$ANDROID_NDK_TOOLS_FULLPREFIX-gcc"
CXX="$ANDROID_NDK_TOOLS_FULLPREFIX-g++"
CPP="$ANDROID_NDK_TOOLS_FULLPREFIX-cpp"
AR="$ANDROID_NDK_TOOLS_FULLPREFIX-ar"
STRIP="$ANDROID_NDK_TOOLS_FULLPREFIX-strip"
RANLIB="$ANDROID_NDK_TOOLS_FULLPREFIX-ranlib"
LD="$ANDROID_NDK_TOOLS_FULLPREFIX-ld"
AS="$ANDROID_NDK_TOOLS_FULLPREFIX-as"

case "$ANDROID_TARGET_ARCH" in
    armeabi-v7a)
        CFLAGS="-g -O2 -ffunction-sections -Wall -funwind-tables -fstack-protector -fno-short-enums -DANDROID -DLITTLE_ENDIAN -Wno-psabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -Wa,--noexecstack -nostdlib --sysroot=$SYSROOT"
        ;;
    armeabi)
        CFLAGS="-g -O2 -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -DANDROID -DLITTLE_ENDIAN -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -Wa,--noexecstack -nostdlib"
        ;;
esac

LIBGCC_PATH_FULL=$($CC -mthumb-interwork -print-libgcc-file-name)
LIBGCC_PATH=$(dirname "$LIBGCC_PATH_FULL")

LIBS="-lc -lgcc -L$SYSROOT/usr/lib -I$ANDROID_NDK_ROOT/include/c++/$ANDROID_NDK_TOOLCHAIN_VERSION -L$ANDROID_NDK_ROOT/$ANDROID_NDK_TOOLCHAIN_PREFIX/lib/armv7-a"
CPPFLAGS="--sysroot=$SYSROOT"
CXXFLAGS="--sysroot=$SYSROOT"
LDFLAGS="-lc -lgnustl_shared -lOpenSLES"

export CPPFLAGS LIBS CC CXX CPP AR STRIP RANLIB LD AS LDFLAGS LIBS CFLAGS

# remove the versioning of the shared objects, the ndk linker only links against shared objects without version numbers
sed -i 's/\(library_names_spec=.\|soname_spec=.\).{libname}.{.*\(.\)/\1${libname}${shared_ext}\2/' configure
mkdir -p android_libs
./configure --prefix=$(readlink -f android_libs) --host=arm-linux-androideabi --enable-debug-output --with-opensles
make -j4 install
