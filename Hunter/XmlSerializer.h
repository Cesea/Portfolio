#ifndef XML_SERIALIZER_H
#define XML_SERIALIZER_H

//void XMLSerialize(tinyxml2::XMLDocument &document, RefVariant var);

template <typename T>
void XMLSerializePrim(std::ostream &os, RefVariant prim)
{
	Padding(os);
	os << prim.GetValue<RemQual<T>::type>() << std::endl;
}

#endif