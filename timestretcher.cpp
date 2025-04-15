#include "timestretcher.h"

Timestretcher::Timestretcher()
{
				std::cout << "Timestretcher::timestretcher" << std::endl;
				circBuffer(10000, 500); // 2000 samples buffersize, 500 numsamplesdelay
}
Timestretcher::~Timestretcher()
{
				std::cout << "Timestretcher::~timestretcher" << std::endl;
				std::cout << "CircBuffer::~circBuffer \n"
									<< "Elements of the Buffer were: ";
				for (int i = 0; i < 512; i++) {
								std::cout << "\033[32m" << buffer[i] << "\033[0m" << " ";
				}
				std::cout << buffer << std::endl;

				std::cout << "Elements of the loopBuffer were: ";
				for (int i = 0; i < 512; i++) {
								std::cout << "\033[34m" << m_loopBuffer[i] << "\033[0m" << " ";
				}
				std::cout << buffer << std::endl;

				std::cout << "readhead pos was: " << readHeadPosition << "\nwritehead pos was: " << writeHeadPosition << std::endl;

				std::cout << "readLoophead pos was: " << m_readLoopHeadPosition << "\nwritehead pos was: " << writeHeadPosition << std::endl;
				std::cout << "WriteLoopHead pos was: " << m_writeLoopHeadPosition << "\nwritehead pos was: " << writeHeadPosition << std::endl;

				std::cout << "the loop was " << m_loopSize << " long\n";
				// std::cout << "rmsSignal was " <<
				releaseBuffer();
}

void Timestretcher::applyEffect(const float& input, float& output)
{
				trackBufferSize(input, m_zeroCrossingTimer);
				writeHead(input);
				incrementWriteHead();

				m_rmsSignal = rms.trackSignal(input);
				output = readLoopHead();
				incrementLoopReadHead();

				prepare(input);
}
void Timestretcher::prepare(const float& input)
{
				if (m_rmsSignal > m_threshold && effectTriggered == false) { // FIXME 0.3 is sensitivity of the effect
								effectTriggered = true;

								m_writeLoopHeadPosition = 0;

								if (writeHeadPosition - m_loopSize < 0) {
												readHeadPosition = writeHeadPosition - m_loopSize + bufferSize;
								} else {
												readHeadPosition = writeHeadPosition - m_loopSize;
								}
								wrapHeads(readHeadPosition);

								// Note: copy the loop from the big buffer to the loopBuffer
								for (int i = 0; i < bufferSize; i++) {
												writeLoopHead(readHead());
												incrementLoopWriteHead();

												incrementReadHead();
								}

								readHeadPosition = 0;
								rms.resetRmsSize();
								m_rmsSignal = 0;
								return;
				}
				if (effectTriggered && m_rmsSignal < 0.05) {
								effectTriggered = false;
				}
}

void Timestretcher::setAmountZeroCrossings(int amountOfZeroCrossings)
{ // TODO: safety checks: check if the number is devisable by 2 else correct the number upwards (dc offset)
				std::cout << "Timestretcher::setAmountZeroCrossings be like\n";
				if (amountOfZeroCrossings > 12 || amountOfZeroCrossings < 256) {
								m_maxNumZeroCrossings = amountOfZeroCrossings;
				} else {
								std::cout << "value is out of range. please select a number between 256" << std::endl;
				}
}

void Timestretcher::trackBufferSize(const float& input, int& m_zeroCrossingTimer)
{
				prevSample = sample;
				sample = input;
				m_zeroCrossingTimer++;

				// if goes from positive to negative
				// XOR gate to check the zeroCrossings
				if ((prevSample >= 0) != (sample >= 0)) {
								m_NumZeroCrossings++;
				}
				// check when the zerocrossings has reached its max. update the delaytime 
				if (m_NumZeroCrossings == m_maxNumZeroCrossings && !effectTriggered) {
								std::cout << "crossed 0 : " << m_NumZeroCrossings << "amount of times" << std::endl;
								std::cout << m_zeroCrossingTimer << " time between zerocrossings" << std::endl;

								setDelayTime(m_zeroCrossingTimer);

								// Update ReadheadPosition based on how long the zerocrossingstimer says the amount of zerocrossings took
								writeHeadPosition = m_zeroCrossingTimer;
								m_loopSize = m_zeroCrossingTimer;
								m_NumZeroCrossings = 0;
								m_zeroCrossingTimer = 0;
				}
}

float Timestretcher::getRmsSignal(float rmsSignal)
{
				m_rmsSignal = rmsSignal;
				return m_rmsSignal;
}

// ______________________ CIRCBUFFER _______________________

void Timestretcher::circBuffer(int size, int numSamplesDelay)
{
				// Dynamic array
				std::cout << "numSamplesDelay: " << numSamplesDelay << std::endl;
				bufferSize = size;
				writeHeadPosition = numSamplesDelay;
				allocateBuffer(size);
}

void Timestretcher::allocateBuffer(int size)
{

				buffer = new float[size];
				for (int i = 0; i < size; i++) {
								buffer[i] = 0;
				}

				std::cout << "allocateLoopBuffer" << std::endl;

				// the size of the buffers are the same but the loopbuffer doesn't need to write constantly
				m_loopBuffer = new float[size];
				for (int i = 0; i < size; i++) {
								m_loopBuffer[i] = 0;
				}
}

void Timestretcher::releaseBuffer()
{
				delete[] buffer;
				buffer = nullptr;
				std::cout << "Timestretcher::releaseBuffer; i am releasing the buffer: " << buffer << std::endl;

				delete[] m_loopBuffer;
				buffer = nullptr;
				std::cout << "Releasing Loop Buffer" << std::endl;
}

float Timestretcher::readHead()
{
				return buffer[readHeadPosition];
}

void Timestretcher::writeHead(float currentSample)
{
				buffer[writeHeadPosition] = currentSample; // input
}

void Timestretcher::writeLoopHead(float currentSample)
{
				// std::cout << "timestretcher::writeloophead the writing of a loophead" << std::endl;
				m_loopBuffer[m_writeLoopHeadPosition] = currentSample;
}

float Timestretcher::readLoopHead()
{
				// std::cout << "Timestretcher::ReadLoopHead" << std::endl;
				return m_loopBuffer[m_readLoopHeadPosition];
}

void Timestretcher::setDelayTime(int numSamplesDelay)
{ // take current writeheadPosition and last numSamplesDelay setting.
				// old numSamplesDelay - new NumsamplesDelay += writeHeadPosition
				//
				// I DONT AGREE
				//
				// move the buffer right of where the buffer was.
				//
				// readHeadPosition = writeHeadPosition - numSamplesDelay; TODO: AFTER FIXING THE LOOP
				//
				//
				// readHeadPosition = 0;
				writeHeadPosition = numSamplesDelay;
}
