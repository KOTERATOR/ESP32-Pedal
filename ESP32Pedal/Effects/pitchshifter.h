#pragma once

#define INP_BUFFER_SIZE 8192
#define datanum 1024
///////////////////////////Globals////////////
float dat[datanum];
double data, Odata;
float c[datanum], d[datanum], e[datanum], f[datanum];
float OrigMag[datanum], OrigFreq[datanum], NewMag[datanum], NewFreq[datanum];
float LastPhase[datanum], SumPhase[datanum];
int offset, NewOffset, osamp, mask;
int peakNum, peak[datanum];
float omega, in_omega, real, img, mag, phase, tempR, tempI, phaseDif, sgn;
float phaseR, phaseI, phaseStep;
int halfsize, fftStep, stepSize, CurStep;
short int Acur, Astep, index;
float FreqFactor;
////////////////////////////////////
float Sine[datanum], Cosine[datanum], Hann[datanum], ReOrder[datanum];
///////////////////////////////allocating memory/////////////////////
int *pSaveBuffer = (int *)GlobalAlloc(GMEM_FIXED, 8 * INP_BUFFER_SIZE);
//////////////////////////////////////////////

void FillBufferPitchShift(int *pBuffer, int initz = 1, int opt = 0)
{
	int i, j, k, l, m, piNum;
	static int *p;
	static int index;

	if (initz == 1)
	{
		ZeroMemory(pSaveBuffer, 8 * INP_BUFFER_SIZE);
		ZeroMemory(LastPhase, 4 * datanum);
		ZeroMemory(SumPhase, 4 * datanum);

		for (i = 0; i < datanum; i++)
		{
			Sine[i] = sin(2 * PI * i / (datanum - 1));
			Cosine[i] = cos(2 * PI * i / (datanum - 1));
			Hann[i] = (.5 - .5 * cos(2 * PI * i / (datanum - 1)));
		}
		///////////Preparing the array to be used for reordering///////////

		for (i = 0; i < datanum; i++)
			ReOrder[i] = 0;
		for (i = 0; i < datanum; i++)
		{
			mask = 1;
			int multiplier = datanum / 2;
			for (j = 0; j < 10; j++)
			{
				if (i & mask)
					ReOrder[i] += multiplier;
				mask *= 2;
				multiplier /= 2;
			}
		}
		//////////////////////////////////////////////////////////

		initz = 0;
	}

	offset = dwPlayLength;
	FreqFactor = pow(2.0, ((Key - 12) * 100 + Cents) / 1200.0); //////////////musical scale
	if (opt == 1)
		FreqFactor = 1 / tempoFac;

	omega = 2 * PI / osamp; ///  expected phase gain per step per bin
	in_omega = 1 / omega;
	stepSize = (datanum / osamp);
	phaseStep = 2 * PI * stepSize;

	p = (int *)pBuffer;

	CurStep = 0; // initializing shift
	for (j = 0; j < Rep * osamp; j++)
	{

		NewOffset = offset + CurStep;

		for (i = 0; i < datanum; i++)
		{

			dat[i] = Volume * playBuff[NewOffset + i] * Hann[i]; // the samples are stored in playBuff  buffer
		}

		/////////////////FFT////////////////////////////////     		;
		for (i = 0; i < datanum; i++)
		{
			c[i] = dat[ReOrder[i]];
			d[i] = 0;
		}

		for (halfsize = 1; halfsize < datanum; halfsize *= 2)
		{
			Acur = 0;
			Astep = datanum / (2 * halfsize);
			phaseR = 1.0, phaseI = 0;

			for (fftStep = 0; fftStep < halfsize; fftStep++)
			{
				for (i = fftStep; i < datanum; i += 2 * halfsize)
				{
					tempR = phaseR * c[i + halfsize] - phaseI * d[i + halfsize];
					tempI = phaseR * d[i + halfsize] + phaseI * c[i + halfsize];

					c[i + halfsize] = c[i] - tempR;
					d[i + halfsize] = d[i] - tempI;

					c[i] = c[i] + tempR;
					d[i] = d[i] + tempI;
				}

				Acur = (Acur + Astep) % datanum;
				phaseR = Cosine[Acur];
				phaseI = -Sine[Acur];
			}

		} ////////////////////

		////////////////////////////////////
		for (k = 0; k < datanum; k++)
		{
			e[k] = 0;
			f[k] = 0;
		}
		///////////////////////////////////

		//////////////////
		// compute phase difference
		for (k = 1; k < datanum / 2; k++)
		{
			real = c[k];
			img = d[k];
			mag = 2 * sqrt(real * real + img * img);

			if (img > 0.)
				sgn = 1.0;
			else
				sgn = -1.0;

			if (img == 0.)
				phase = 0;
			else if (real == 0.)
				phase = (sgn * PI / 2);
			else
				phase = atan2(img, real);

			phaseDif = phase - LastPhase[k]; // change in phase
			LastPhase[k] = phase;						 // the new phase becomes the last phase

			//// subtracting the expected phase difference
			phaseDif -= k * omega;

			// bringing the value in the interval of -PI and +PI
			piNum = (int)(phaseDif / PI);

			if (piNum >= 0)
				piNum = ((piNum + 1) / 2) * 2;
			else
				piNum = ((piNum - 1) / 2) * 2;
			phaseDif -= PI * piNum;
			//////////////The Result////////////////////////

			OrigMag[k] = mag;
			OrigFreq[k] = (k + phaseDif * in_omega); ///// new_k = (k + phaseDiff*osamp/2*PI)
		}
		////////////////////////////////////////////////////////////
		//// Finding the peak values for large changes in frequencies
		/////////////////////////////////////////////////////////////
		peakNum = 0;
		for (k = 1; k < datanum / 2 - 1; k++)
		{
			if (OrigMag[k] > OrigMag[k - 1] && OrigMag[k] > OrigMag[k + 1])
			{
				peak[peakNum] = k;
				peakNum++;
			}
		}

		// ***************** PROCESSING *******************
		// this does the actual pitch shifting
		ZeroMemory(NewMag, 4 * datanum);
		ZeroMemory(NewFreq, 4 * datanum);
		///////////////////////////////////////
		if (FreqFactor >= 2.0)
			for (k = 0; k < peakNum; k++) // using only peaks
			{
				m = peak[k];
				index = (m * FreqFactor);
				if (index < datanum / 2)
				{
					NewFreq[index] = (OrigFreq[m] * FreqFactor);
					NewMag[index] += OrigMag[m];
				}
			}
		else // use all the bins
			for (k = 0; k < datanum / 2; k++)
			{
				index = (k * FreqFactor);
				if (index < datanum / 2)
				{
					NewFreq[index] = (OrigFreq[k] * FreqFactor);
					NewMag[index] += OrigMag[k];
				}
			}

		// ***************** SYNTHESIS *******************

		for (k = 0; k < datanum / 2; k++)
		{
			///the phase change in moving hop size is 2*PI*k/osamp
			/// we are using the true value of k calculated earlier
			phase = SumPhase[k] + (NewFreq[k] * omega);

			piNum = phase / PI;
			////Bringing in the proper range
			if (piNum >= 0)
				piNum = ((piNum + 1) / 2) * 2;
			else
				piNum = ((piNum - 1) / 2) * 2;

			phase -= PI * piNum;

			SumPhase[k] = phase; // initial value for the next frame

			e[k] = mag * cos(phase);
			f[k] = mag * sin(phase);
		}

		///////////////////////////////////////////////
		/////////   ReOrder and IFFT
		///////////////////////////////////////////////
		for (i = 0; i < datanum; i++)
		{
			c[i] = e[ReOrder[i]];
			d[i] = f[ReOrder[i]];
		}
		///////////////////////////////////
		for (halfsize = 1; halfsize < datanum; halfsize *= 2)
		{

			Acur = 0;
			Astep = datanum / (2 * halfsize);
			phaseR = 1.0, phaseI = 0;

			for (fftStep = 0; fftStep < halfsize; fftStep++)
			{
				for (i = fftStep; i < datanum; i += 2 * halfsize)
				{
					tempR = phaseR * c[i + halfsize] - phaseI * d[i + halfsize];
					tempI = phaseR * d[i + halfsize] + phaseI * c[i + halfsize];

					c[i + halfsize] = c[i] - tempR;
					d[i + halfsize] = d[i] - tempI;

					c[i] = c[i] + tempR;
					d[i] = d[i] + tempI;
				}

				Acur = (Acur + Astep) % datanum;
				phaseR = Cosine[Acur];
				phaseI = Sine[Acur];
			}
		}

		///////////////////////////////////////////////////////
		//////////////////////////////////////////
		for (i = 0; i < datanum; i++)
			pSaveBuffer[CurStep + i] += 2.82 * c[i] * Hann[i] / (osamp * datanum); // add with rescaling

		CurStep += stepSize;
	}
	//////////////////////////
	//////////////////////////
	for (i = 0; i < INP_BUFFER_SIZE; i++)
	{

		data = pSaveBuffer[i];
		*p = data;
		p++;
	}

	dwPlayLength += INP_BUFFER_SIZE;
	/*
    The size of the pSaveBuffer in bytes is (8*INP_BUFFER_SIZE) in order to store 2*INP_BUFFER_SIZE integers.
   This is the most important part. In the last overlap of the loop the data will enter the second
   half of the buffer by (datanum - datanum/osamp) samples. They have to be preserved else the result
   will be poor . So the second half is copied to the first half and second half is cleared.
  */
	CopyMemory(pSaveBuffer, pSaveBuffer + INP_BUFFER_SIZE, 4 * INP_BUFFER_SIZE);
	ZeroMemory(pSaveBuffer + INP_BUFFER_SIZE, 4 * INP_BUFFER_SIZE);
}