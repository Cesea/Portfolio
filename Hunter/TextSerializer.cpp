#include "stdafx.h"
#include "TextSerializer.h"

static uint32 paddingLevel = 0;

void Padding(std::ostream & os)
{
	for (uint32 i = 0; i < paddingLevel; ++i)
	{
		os << "	";
	}
}

void TextSerialize(std::ostream & os, RefVariant var)
{
	const MetaData *meta = var.Meta();
	void *data = var.Data();

	assert(meta->HasMembers());

	os << meta->Name() << std::endl;

	const Member *member = meta->Members();
	Padding(os);
	os << "{" << std::endl;
	while (member)
	{
		++paddingLevel;
		void *offsetData = PTR_ADD(var.Data(), member->Offset());
		member->Meta()->Serialize(os, RefVariant(member->Meta(), offsetData));
		member = member->Next();
		--paddingLevel;
	}
	Padding(os);
	os << "}" << std::endl;
}