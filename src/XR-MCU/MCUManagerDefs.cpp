#include "MCUManagerDefs.h"
#include <thread>

namespace mcu
{

size_t getNumCpus()
{
	static size_t s_numCpus = std::thread::hardware_concurrency();

	return s_numCpus;
}

const size_t k_numCpus = std::thread::hardware_concurrency();

size_t cpusForDecoding()
{
	return static_cast<size_t>(0.30f * k_numCpus);
}
size_t cpusForEncoding()
{
	return static_cast<size_t>(0.65f * k_numCpus);
}
size_t cpusForFusion()
{
	return static_cast<size_t>(0.05f * k_numCpus);
}

const size_t k_numCpusForDecoding = cpusForDecoding();
const size_t k_numCpusForEncoding = cpusForEncoding();
const size_t k_numCpuForFusion = cpusForFusion();

}