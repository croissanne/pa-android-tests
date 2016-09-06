#include "include/portaudio.h"
#include <android/log.h>
#include <math.h>

#define APPNAME "portaudio_test_blocking_sine"
#define NUM_SECONDS (5)
#define SAMPLE_RATE (48000)
#define FRAMES_PER_BUFFER (512)

#ifndef M_PI
#define M_PI (3.14159265)
#endif

#define TABLE_SIZE (512)

int main()
{
    PaStreamParameters outputParameters;
    PaStreamParameters inputParameters;
    PaStream *stream;
    PaError err;
    // record 200 buffers, play 200 buffers
    short buffer[200][FRAMES_PER_BUFFER][2];
    short sine[TABLE_SIZE];
    int left_phase = 0;
    int right_phase = 0;
    int left_inc = 1;
    int right_inc = 3;
    int i, j, k, x;
    int bufferCount;

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

    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "no default inputdevice");
    }
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = paInt16;
    inputParameters.suggestedLatency =
        Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream, &inputParameters, &outputParameters,
                        SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);

    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error opening stream %d", err);
    }

    for (k = 0; k < 1; ++k) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "starting playback");
        err = Pa_StartStream(stream);
        if (err != paNoError) {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                "error starting stream %s",
                                Pa_GetLastHostErrorInfo()->errorText);
        }

        bufferCount = 1000;

        for (i = 0; i < bufferCount; i++) {
            for (j = 0; j < 200; j++)
                err = Pa_ReadStream(stream, buffer[j], 512);
            for (j = 0; j < 200; j++)
                err = Pa_WriteStream(stream, buffer[j], 512);
        }

        //        for (i = 0; i < bufferCount; ++i)

        err = Pa_StopStream(stream);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "stopped stream, err %d", err);
    }
    err = Pa_CloseStream(stream);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "closed stream, err %d",
                        err);
    Pa_Terminate();
}
