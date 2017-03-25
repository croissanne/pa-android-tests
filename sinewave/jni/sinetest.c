#include "include/pa_opensles.h"
#include "include/portaudio.h"
#include <android/log.h>
#include <math.h>

#define NUM_SECONDS (1)
#define SAMPLE_RATE (48000)
#define FRAMES_PER_BUFFER (512)

#ifndef M_PI
#define M_PI (3.14159265)
#endif

#define TABLE_SIZE (256)
#define APPNAME "sinewave test"

float sine[TABLE_SIZE];
int left_phase;
int right_phase;
char message[20];
int count;

int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo,
                     PaStreamCallbackFlags statusFlags)
{
    float *out = (float *)outputBuffer;
    unsigned long i;
    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;

    for (i = 0; i < framesPerBuffer; i++) {
        *out++ = sine[left_phase];  /* left */
        *out++ = sine[right_phase]; /* right */
        left_phase += 2;
        if (left_phase >= TABLE_SIZE)
            left_phase -= TABLE_SIZE;
        right_phase +=
            4; /* higher pitch so we2 can distinguish left and right. */
        if (right_phase >= TABLE_SIZE)
            right_phase -= TABLE_SIZE;
    }
    return paContinue;
}

static int paCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo *timeInfo,
                      PaStreamCallbackFlags statusFlags, void *userData)
{
    return paCallbackMethod(inputBuffer, outputBuffer, framesPerBuffer,
                            timeInfo, statusFlags);
}

void paStreamFinishedMethod()
{
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "streamfinishedcallback says: stream is finished!");
}

static void paStreamFinished(void *userData)
{
    return paStreamFinishedMethod();
}

int main()
{
    PaStream *stream;
    PaError err = paNoError;
    PaStreamParameters outputParameters;
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
        sine[i] = ((float)sin(((double)i / (double)TABLE_SIZE) * M_PI * 2.));
    }
    PaOpenSLES_SetNativeBufferSize(240);
    /* PaOpenSLES_SetNumberOfBuffers(2); */
    err = Pa_Initialize();
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error during initialization %d", err);
    }

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "no output device found");
        return 0;
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency =
        Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = 0;

    err =
        Pa_OpenStream(&stream, 0, &outputParameters, SAMPLE_RATE,
                      paFramesPerBufferUnspecified, paClipOff, &paCallback, 0);
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "couldn't open stream with err %d", err);
        return 0;
    }
    err = Pa_SetStreamFinishedCallback(stream, &paStreamFinished);
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "couldn't set finished callback with err %d", err);
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 0;
    }
    for (i = 0; i < 3; ++i) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "starting playback");
        err = Pa_StartStream(stream);
        if (err != paNoError) {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                "couldn't start stream with err %d", err);
            Pa_CloseStream(stream);
            Pa_Terminate();
            return 0;
        }

        Pa_Sleep(1000 * NUM_SECONDS);

        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "stopping playback");
        err = Pa_StopStream(stream);
        if (err != paNoError) {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                                "couldn't stop stream with err %d", err);
            Pa_CloseStream(stream);
            Pa_Terminate();
            return 0;
        }
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "closing playback");
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "couldn't close stream with err %d", err);
        Pa_Terminate();
        return 0;
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "closed playback, terminating");
    Pa_Terminate();
    return 0;
}
