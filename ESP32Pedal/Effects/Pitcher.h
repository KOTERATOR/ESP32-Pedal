#pragma once

#include "../EffectsUnit.h"
#include "../Views/PotentiometerView.h"
#include "pitchshifterOLD.h"

#define PITCH_BUFFERSIZE 1024

namespace PitchShifterCode
{

}; // namespace PitchShifterCode
class Pitcher : public EffectsUnit
{
private:
    //int16_t outBuf[1024] = {};
    float gInFIFO[PITCH_BUFFERSIZE];
    float gOutFIFO[PITCH_BUFFERSIZE];
    float gFFTworksp[2 * PITCH_BUFFERSIZE];
    float gLastPhase[PITCH_BUFFERSIZE / 2 + 1];
    float gSumPhase[PITCH_BUFFERSIZE / 2 + 1];
    float gOutputAccum[2 * PITCH_BUFFERSIZE];
    float gAnaFreq[PITCH_BUFFERSIZE];
    float gAnaMagn[PITCH_BUFFERSIZE];
    float gSynFreq[PITCH_BUFFERSIZE];
    float gSynMagn[PITCH_BUFFERSIZE];

    long gRover = false, gInit = false;

    float inBuf[PITCH_BUFFERSIZE] = {}, psBuf[PITCH_BUFFERSIZE] = {}, ps2Buf[PITCH_BUFFERSIZE] = {};
    float pitchShift = 0.0;
    bool firstBuf = false;

    bool finished = false;
    int counter = 0;
    int16_t value = 0;
    PotentiometerView pitch = PotentiometerView(this, "PITCH", 2047);

    void analysis(long k, float pitchShift, float expct, float freqPerBin, long osamp)
    {
        long qpd;
        float magn, phase, tmp, real, imag;
        /* de-interlace FFT buffer */
        real = gFFTworksp[2 * k];
        imag = gFFTworksp[2 * k + 1];

        /* compute magnitude and phase */
        magn = 2. * sqrt(real * real + imag * imag);
        phase = atan2_fast(imag, real);

        /* compute phase difference */
        tmp = phase - gLastPhase[k];
        gLastPhase[k] = phase;

        /* subtract expected phase difference */
        tmp -= (float)k * expct;

        /* map delta phase into +/- Pi interval */
        qpd = tmp / M_PI;
        if (qpd >= 0)
            qpd += qpd & 1;
        else
            qpd -= qpd & 1;
        tmp -= M_PI * (float)qpd;

        /* get deviation from bin frequency from the +/- Pi interval */
        tmp = osamp * tmp / (2. * M_PI);

        /* compute the k-th partials' true frequency */
        tmp = (float)k * freqPerBin + tmp * freqPerBin;

        /* store magnitude and true frequency in analysis arrays */
        gAnaMagn[k] = magn;
        gAnaFreq[k] = tmp;
    }

    void pitcher(float pitchShift, long numSampsToProcess, long osamp, float sampleRate, float *indata, float *outdata, long step)
    {

        float magn, phase, tmp, window, real, imag;
        float freqPerBin, expct;
        long i, k, qpd, index, inFifoLatency, stepSize, fftFrameSize2, fftFrameSize = numSampsToProcess;

        /* set up some handy variables */
        fftFrameSize2 = fftFrameSize / 2;
        stepSize = fftFrameSize / osamp;
        freqPerBin = sampleRate / (float)fftFrameSize;
        expct = 2. * M_PI * (float)stepSize / (float)fftFrameSize;

        inFifoLatency = fftFrameSize - stepSize;
        if (gRover == false)
            gRover = inFifoLatency;

        /* initialize our static arrays */
        if (step == 0)
        {
            memset(gInFIFO, 0, PITCH_BUFFERSIZE * sizeof(float));
            memset(gOutFIFO, 0, PITCH_BUFFERSIZE * sizeof(float));
            memset(gFFTworksp, 0, 2 * PITCH_BUFFERSIZE * sizeof(float));
            memset(gLastPhase, 0, (PITCH_BUFFERSIZE / 2 + 1) * sizeof(float));
            memset(gSumPhase, 0, (PITCH_BUFFERSIZE / 2 + 1) * sizeof(float));
            memset(gOutputAccum, 0, 2 * PITCH_BUFFERSIZE * sizeof(float));
            memset(gAnaFreq, 0, PITCH_BUFFERSIZE * sizeof(float));
            memset(gAnaMagn, 0, PITCH_BUFFERSIZE * sizeof(float));

            /* main processing loop */
            /* do windowing and re,im interleave */
            for (k = 0; k < fftFrameSize; k++)
            {
                //window = -.5*cos(2.*M_PI*(float)k/(float)fftFrameSize)+.5;
                gFFTworksp[2 * k] = indata[k] * 1.0;
                gFFTworksp[2 * k + 1] = 0.;
            }

            /* ***************** ANALYSIS ******************* */
            /* do transform */
            smbFft(gFFTworksp, fftFrameSize, -1);

            memset(gSynMagn, 0, fftFrameSize * sizeof(float));
            memset(gSynFreq, 0, fftFrameSize * sizeof(float));
        }

        /* this is the analysis step */
        if (step <= fftFrameSize2)
        {
            analysis(step, pitchShift, expct, freqPerBin, osamp);
        }
        else
        {
            /* ***************** PROCESSING ******************* */
            /* this does the actual pitch shifting */
            for (k = 0; k <= fftFrameSize2; k++)
            {
                index = k * pitchShift;
                if (index <= fftFrameSize2)
                {
                    gSynMagn[index] += gAnaMagn[k];
                    gSynFreq[index] = gAnaFreq[k] * pitchShift;
                }
            }

            long realStep = step - fftFrameSize2;

            /* ***************** SYNTHESIS ******************* */
            /* this is the synthesis step */
            k = realStep;
            {

                /* get magnitude and true frequency from synthesis arrays */
                magn = gSynMagn[k];
                tmp = gSynFreq[k];

                /* subtract bin mid frequency */
                tmp -= (float)k * freqPerBin;

                /* get bin deviation from freq deviation */
                tmp /= freqPerBin;

                /* take osamp into account */
                tmp = 2. * M_PI * tmp / osamp;

                /* add the overlap phase advance back in */
                tmp += (float)k * expct;

                /* accumulate delta phase to get bin phase */
                gSumPhase[k] += tmp;
                phase = gSumPhase[k];

                /* get real and imag part and re-interleave */
                gFFTworksp[2 * k] = magn * Math::get_cos(phase);
                gFFTworksp[2 * k + 1] = magn * Math::get_sin(phase);
            }

            if (step == fftFrameSize - 1)
            {
                /* zero negative frequencies */
                for (k = fftFrameSize + 2; k < 2 * fftFrameSize; k++)
                    gFFTworksp[k] = 0.;

                /* do inverse transform */
                smbFft(gFFTworksp, fftFrameSize, 1);

                /* do windowing and add to output accumulator */
                for (k = 0; k < fftFrameSize; k++)
                {
                    //window = -.5*cos(2.*M_PI*(float)k/(float)fftFrameSize)+.5;
                    //gOutputAccum[k]
                    outdata[k] += 2. * 1.0 * gFFTworksp[2 * k] / (fftFrameSize2 * osamp);
                }
            }
        }

        /*for (k = 0; k < stepSize; k++)
            gOutFIFO[k] = gOutputAccum[k];*/

        /* shift accumulator */
        //memmove(gOutputAccum, gOutputAccum + stepSize, fftFrameSize * sizeof(float));

        /* move input FIFO */
        /*for (k = 0; k < inFifoLatency; k++)
            gInFIFO[k] = gInFIFO[k + stepSize];*/
    }

public:
    Pitcher() : EffectsUnit("PITCHER")
    {
        for (int i = 0; i < PITCH_BUFFERSIZE; i++)
        {
            inBuf[i] = 0;
            psBuf[i] = 0;
            ps2Buf[i] = 0;
            //outBuf[i] = 0;
        }
    }

    void processing(int16_t *in1, int16_t *in2)
    {
        inBuf[counter] = (*in1) - (*in2);
        psBuf[counter] = ps2Buf[counter] = inBuf[counter];

        counter++;
        if (counter >= PITCH_BUFFERSIZE)
        {
            counter = 0;
            //PitchShifter::PitchShift(pitch.getRangedValue(), 25000.0, inBuf, 1024);

            //smbFft(inBuf, BUFFERSIZE, -1);
            //unsigned long time = millis();
            //pitchShift = powf(2., ((pitch.getRangedValue() * 2.0f - 1.0) * 12) / 12.);

            //xTaskCreatePinnedToCore(PitchShifterCode::TaskPitchShift, /* Function to implement the task */ "TaskPitch",   /* Name of the task */ 20000,     /* Stack size in words */ NULL,      /* Task input parameter */ 0,         /* Priority of the task */ &PitchShifterCode::TaskPitch,    /* Task handle. */ 0);        /* Core where the task should run */
            //Serial.printf("PITCH SHIFT: %d\n", millis() - time);
            /* for(int i = 0; i < BUFFERSIZE; i++)
            {
                psBuf[i] = inBuf[i/2];
                
            }*/
        }
        
        //pitcher(pitchShift, PITCH_BUFFERSIZE, 4, 25000, inBuf, firstBuf ? ps2Buf : psBuf, counter);
        int16_t outVal = (firstBuf ? psBuf : ps2Buf)[counter];

        if (outVal >= 0)
        {
            /*(*in1) = outVal;
            (*in2) = 0;*/
        }
        else
        {
            /*(*in1) = 0;
            (*in2) = -outVal;*/
        }
    }
};