#include "rms.h"
#include <cmath>
#include <iostream>

Rms::Rms(int bufferSize)
{
				this->bufferSize = bufferSize;
				std::cout << "initialising RMS" << std::endl;
}

Rms::~Rms()
{
}

float Rms::trackSignal(float incomingSignal)
{
				//do rms without keeping a real buffer. just track the buffersum and the index of the sample 
				bufferPosition++;

				bufferSum += incomingSignal * incomingSignal;
				RMSSignal = std::sqrt(bufferSum / static_cast<float>(bufferPosition));

				if(bufferPosition > bufferSize){resetRmsSize();}
				return RMSSignal;
}

float Rms::resetRmsSize()
{
				bufferSum = 0;
				RMSSignal = 0;
				bufferPosition = 0;
				return RMSSignal;
}
