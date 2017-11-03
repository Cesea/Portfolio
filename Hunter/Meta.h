#ifndef META_H
#define META_H

#include "Typedefs.h"

#include <string>
#include <map>

class RefVariant;

typedef void(*SerializerFunction)(std::ostream &, RefVariant);


#define PASETE(_, __) _##__
#define GENERATE_LINE( _ ) PASTE( GENERATED_TOKEN_, _)
#define GENERATE_FILE( _ ) PASETE( __FILE__, _ )
#define NAME_GENERATOR( ) GENERATE_FILE( __LINE__ )

#define PTR_ADD(PTR, OFFSET)   ((void *)(((char *)(PTR)) + (OFFSET))) 


#define META_TYPE(TYPE) (MetaCreator<RemQual<TYPE>::type>::Get())


#define META(OBJECT) (MetaCreator<RemQual<decltype(OBJECT)>::type>::Get())



#define DEFINE_META(TYPE) \
	MetaCreator<RemQual<TYPE>::type> NAME_GENERATOR( )( #TYPE, sizeof( TYPE ) ); \
  RemQual<TYPE>::type *TYPE::NullCast( void ) { return reinterpret_cast<RemQual<TYPE>::type *>(NULL); } \
  void TYPE::AddMember(const std::string &name, uint32 offset, const MetaData *data ) { return MetaCreator<RemQual<TYPE>::type>::AddMember( name, offset, data ); } \
  void MetaCreator<RemQual<TYPE>::type>::RegisterMetaData( void ) { TYPE::RegisterMetaData( ); } \
  void TYPE::RegisterMetaData( void )

#define DEFINE_META_POD(TYPE) \
	MetaCreator<RemQual<TYPE>::type> NAME_GENERATOR() (#TYPE, sizeof(TYPE)); \
	void MetaCreator<RemQual<TYPE>::type>::RegisterMetaData() \
	{\
		MetaCreator<RemQual<TYPE>::type>::SetSerializerFunction(TextSerializePrim<RemQual<TYPE>::type>); \
	}

#define DECLARE_META(TYPE)  \
	static void AddMember(const std::string &name, uint32 offset, const MetaData *meta); \
	static RemQual<TYPE>::type *NullCast(); \
	static void RegisterMetaData(void)


#define ADD_MEMBER( MEMBER ) \
	AddMember(#MEMBER, (uint32)(&(NullCast()->MEMBER)), META(NullCast()->MEMBER))


//Class Forward Declaration
class Member;

// Class Meta //////////////////////////////////////////////////////
class MetaData
{
public :
	MetaData()
		:_name(), _size(), _members(nullptr), _lastMember(nullptr), _serializer(nullptr)
	{
	}
	MetaData(const std::string &name, uint32 size)
		:_name(name), _size(size), _members(nullptr), _lastMember(nullptr), _serializer(nullptr)
	{
	}

	const std::string &Name() const
	{
		return _name;
	}
	const uint32 Size() const
	{
		return _size;
	}

	void Copy(void *dest, const void *src) const;
	void Delete(void *data) const;
	void *NewCopy(const void *src) const;
	void *New();

	void AddMember(const Member *member);
	bool32 HasMembers() const;

	const Member *Members() const;
	void PrintMembers(std::ostream &os) const;

	void SetSerializer(SerializerFunction func = nullptr);
	void Serialize(std::ostream &os, RefVariant ref) const;

	//TODO : Implement this;
	uint32 TotalNumElement() const;

private :
	std::string _name;
	SerializerFunction _serializer;

	Member *_members;
	Member *_lastMember;
	uint32 _size;
};
/////////////////////////////////////////////////////////////////////////

// Class Member //////////////////////////////////////////////////////
class Member
{
public :
	Member(const std::string &name, uint32 offset, const MetaData *meta)
		:_name(name), _offset(offset), _meta(meta), _next(nullptr)
	{
	}

	const std::string Name() const { return _name; }
	uint32 Offset() const { return _offset; }
	const MetaData *Meta() const { return _meta; }

	Member *&Next();
	Member *const &Next() const;

private :
	std::string _name;
	uint32 _offset;
	const MetaData *_meta;
	Member *_next;
};
/////////////////////////////////////////////////////////////////////////

// Class Meta Creator //////////////////////////////////////////////////////
template <typename T>
class MetaCreator
{
public :
	MetaCreator(const std::string &name, uint32 size)
	{
		Init(name, size);
		//*Get() = MetaData(name, size);
	}

	static void Init(const std::string &name, uint32 size)
	{
		*Get() = MetaData(name, size);
		RegisterMetaData();
	}

	static T *NullCast()
	{
		return reinterpret_cast<T *>(nullptr);
	}

	static void AddMember(const std::string &name, uint32 offset, const MetaData *meta)
	{
		Get()->AddMember(new Member(name, offset, meta));
	}

	static void SetSerializerFunction(SerializerFunction fn)
	{
		Get()->SetSerializer(fn);
	}

	static void RegisterMetaData();

	static MetaData *Get()
	{
		static MetaData metaData;
		return &metaData;
	}
};
/////////////////////////////////////////////////////////////////////////

#endif
