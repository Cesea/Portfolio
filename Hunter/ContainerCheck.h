#ifndef CONTAINER_CHECK_H
#define CONTAINER_CHECK_H

template <class TContainer>
void EnsureCapacity(TContainer& container, typename TContainer::size_type index)
{
	// if we need to resize
	if (container.size() <= index)
	{
		// then we shall resize!
		container.resize(index + 1);
	}
}

#endif