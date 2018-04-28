#include <drivers/qei.h>

Qei::Qei(Qei::Channel channel)
{
    this->channel = channel;
}

uint16_t Qei::GetStepsCount()
{
	return 0;
}
