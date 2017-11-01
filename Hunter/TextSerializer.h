#ifndef TEXT_SERIALIZER_H
#define TEXT_SERIALIZER_H

void Padding(std::ostream &os);

void TextSerialize(std::ostream &os, RefVariant var);

template <typename T>
void TextSerializePrim(std::ostream &os, RefVariant prim)
{
	Padding(os);
	os << prim.GetValue<RemQual<T>::type>() << std::endl;
}


#endif