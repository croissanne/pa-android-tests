#include "include/portaudio.h"
#include "include/pa_opensles.h"
#include <android/log.h>
#include <math.h>

#define APPNAME "portaudio_test_blocking_sine"
#define NUM_SECONDS (1)
#define SAMPLE_RATE (48000)
#define FRAMES_PER_BUFFER (512)

#ifndef M_PI
#define M_PI (3.14159265)
#endif

#define TABLE_SIZE (512)

int main()
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    short buffer[FRAMES_PER_BUFFER][2];
    short sine[TABLE_SIZE];
    int left_phase = 0;
    int right_phase = 0;
    int left_inc = 1;
    int right_inc = 3;
    int i, j, k;
    int bufferCount;

    PaOpenSLES_SetNumberOfBuffers(2);
    PaOpenSLES_SetNativeBufferSize(512);
    for (i = 0; i < TABLE_SIZE; i++) {
        sine[i] =
            (short)(((float)sin(((double)i / (double)TABLE_SIZE) * M_PI * 2.)) *
                    SHRT_MAX);
    }
    err = Pa_Initialize();
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error during initialization %d", err);
    }

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "no default output device");
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency =
        Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream, NULL, &outputParameters, SAMPLE_RATE,
                        FRAMES_PER_BUFFER, paClipOff, NULL, NULL);

    for (k = 0; k < 4; ++k) {
        bufferCount = ((NUM_SECONDS * SAMPLE_RATE) / FRAMES_PER_BUFFER);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "starting playback");
        err = Pa_StartStream(stream);
        if (err != paNoError)
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                "error starting stream %d", err);

        for (i = 0; i < bufferCount; i++) {
            for (j = 0; j < 512; ++j) {
                buffer[j][0] = sine[left_phase];
                buffer[j][1] = sine[right_phase];
                left_phase += 4;
                if (left_phase >= 512)
                    left_phase -= 512;
                right_phase += 8;
                if (right_phase >= 512)
                    right_phase -= 512;
            }

            err = Pa_WriteStream(stream, buffer, 512);
        }
        err = Pa_StopStream(stream);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "stopped stream, err %d", err);
        Pa_Sleep(200);
        ++left_inc;
        ++right_inc;
    }
    err = Pa_CloseStream(stream);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "closed stream, err %d",
                        err);
    Pa_Terminate();
}
