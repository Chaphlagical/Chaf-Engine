#include "ColorEditing.h"

namespace Chaf
{
	ColorEditing::ColorEditing(const Ref<Image>& source, const Ref<Image>& target) :
		m_source(source),
		m_target(target)
	{
	}
}