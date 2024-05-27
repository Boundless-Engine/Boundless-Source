#include "status_helpers.h"

#include <stdexcept>
#include <format>

void Boundless::operator +(const BReturn& r)
{
	if (r < SUCCESS || r > SUCCESS)
		throw std::runtime_error(std::format("the operation has failed... %i", (int)r));

}
