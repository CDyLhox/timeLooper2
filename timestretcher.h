#pragma once

#include "effect.h"
#include "rms.h"
#include "leveler.h"

class Timestretcher : public Effect {
		public:
				Timestretcher();
				~Timestretcher() override;

				// appies the delay effect as a timestretcher
				void applyEffect(const float& input, float& output) override;

				// set the amount of time the inputSig has to cross the 0 before the circbuffer repeats
				void setMaxNumZeroCrossings(float amountOfZeroCrossings);
				void setThreshold(float threshold);

				// use the numZeroCrossings to fill buffer
				void prepare();
				void trackBufferSize(const float& input, int& m_zeroCrossingTimer);
				void fillBuffer(const float& input);
				float getRmsSignal();
				int getNumZeroCrossings();


                LevelTracker m_levelTracker{512}; // TODO: MAke PRIVATE
		protected:
		private:
                bool reverseBuffer = false; //TODO: interesting parameter
				int zeroCrossingsValues;

				
				float m_threshold = 0.3;
				bool effectTriggered = false;

				int m_NumZeroCrossings = 0;
				int m_maxNumZeroCrossings = 128; // FIXME interesting parameter, maxnumzerocrossings
				int m_zeroCrossingTimer = 0;
				float prevSample = 0;
				float sample = 0;
				float m_rmsSignal = 0;
				uint bufferSize = 44100;

				// ------- CIRCBUFFER STUFF ------
				void circBuffer(int bufferSize);
				float readHead();
				void writeHead(float currentSample);

				void writeLoopHead(float currentSample);
				float readLoopHead();

				// Buffer Stuffer
				float* buffer;
				float* m_loopBuffer;
				void allocateBuffer(int size);
				void releaseBuffer();

				void setDelayTime(int numSamplesDelay);



				float currentSample;
				int numSamplesDelay;

				int readHeadPosition = 0;
				int writeHeadPosition = 0;

				uint m_readLoopHeadPosition = 0;
				uint m_writeLoopHeadPosition = 0;

				uint m_loopSize = bufferSize;

				//Duplicate code but im leaving it this way because the incrementLoophead has different logistics
				inline void incrementWriteHead()
				{
								writeHeadPosition++;
								wrapHeads(writeHeadPosition);
				}
				inline void incrementReadHead()
				{
								readHeadPosition++;

								wrapHeads(readHeadPosition);
				}
				inline void wrapHeads(int& head)
				{

								while (head >= bufferSize) {
												head -= bufferSize;
								}
								while (head < 0) {
												head += bufferSize;
								}
								return;
				}

				inline void incrementLoopWriteHead()
				{
								m_writeLoopHeadPosition++;
								wrapLoopHeads(m_writeLoopHeadPosition);
				}
				inline void incrementLoopReadHead()
				{
								m_readLoopHeadPosition++;

								wrapLoopHeads(m_readLoopHeadPosition);
				}
				inline void wrapLoopHeads(uint& head)
				{

								if (head >= m_loopSize) { 
												head -= m_loopSize;
												
								} else if (head >= bufferSize) {
												head -= bufferSize;
								}
				}
};
