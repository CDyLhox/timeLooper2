#pragma once
#include <iostream>

// based on code by Cas Huurdeman

class Rms {
		public:
				Rms(int bufferSize);
				~Rms();

				float trackSignal(float incomingSignal);
				float resetRmsSize();

		private:
				int bufferSize = 4000;
				int bufferPosition = 0;

				float* buffer;
				float bufferSum = 0.0;
				float RMSSignal = 0;
};
