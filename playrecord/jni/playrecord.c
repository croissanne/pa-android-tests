#include "include/pa_opensles.h"
#include "include/portaudio.h"
#include <android/log.h>
#include <math.h>

#define NUM_SECONDS (10)
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)

#ifndef M_PI
#define M_PI (3.14159265)
#endif

#define TABLE_SIZE (256)
#define APPNAME "PLAYRECORD"

short buffer[200][FRAMES_PER_BUFFER][2];
int left_phase;
int right_phase;
char message[20];
int count = 0;
int goup = 1;
int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo,
                     PaStreamCallbackFlags statusFlags)
{
    short *out = (short *)outputBuffer;
    short *in = (short *)inputBuffer;
    unsigned long i;
    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;

    for (i = 0; i < framesPerBuffer; ++i) {
        buffer[0][i][0] = *in++;
        buffer[0][i][1] = *in++;
    }
    for (i = 0; i < framesPerBuffer; ++i) {
        *out++ = buffer[0][i][0];
        *out++ = buffer[0][i][1];
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
    PaStreamParameters outputParameters, inputParameters;
    int i;

    SetNativeBufferSize(
        1024); /* before Pa_initialize the ideal buffer size is set */
    err = Pa_Initialize();
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "error during initialization %d", err);
    }

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "no output device found");
        return;
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency =
        Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = 0;

    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency =
        Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = 0;

    if (inputParameters.device == paNoDevice) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "no inputdevice found");
        return;
    }
    err =
        Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE,
                      FRAMES_PER_BUFFER, paClipOff, &paCallback, 0);
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "couldn't open stream with err %d", err);
        return;
    }
    err = Pa_SetStreamFinishedCallback(stream, &paStreamFinished);
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "couldn't set finished callback with err %d", err);
        Pa_CloseStream(stream);
        Pa_Terminate();
        return;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "starting playback");
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "couldn't start stream with err %d", err);
        Pa_CloseStream(stream);
        Pa_Terminate();
        return;
    }

    Pa_Sleep(1000 * NUM_SECONDS);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "stopping playback");
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "couldn't stop stream with err %d", err);
        Pa_CloseStream(stream);
        Pa_Terminate();
        return;
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "closing playback");
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                            "couldn't close stream with err %d", err);
        Pa_Terminate();
        return;
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME,
                        "closed playback, terminating");
    Pa_Terminate();
}
