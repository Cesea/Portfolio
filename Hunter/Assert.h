#ifndef ASSERT_H
#define ASSERT_H

#define HUNTER_ASSERT_IMPL_VAR(variable) .Variable(HUNTER_PP_TO_STRING(variable), variable)
#define HUNTER_ASSERT_IMPL_VARS(...) HUNTER_PP_EXPAND_ARGS HUNTER_PP_PASS_ARG()
#define HUNTER_ASSERT(condition, format, ...) (condition) ? (0) : (Assert(SOURCE_INFO, "Assertion failed", format, __VA_ARGS__))

class Assert
{
public :
	Assert(const SourceInfo &sourceInfo, const char *format, ...);


	Assert &Variable(const char *const name, bool var);
	Assert &Variable(const char *const name, char var);

	Assert &Variable(const char *const name, int8 var);
	Assert &Variable(const char *const name, int16 var);
	Assert &Variable(const char *const name, int32 var);
	Assert &Variable(const char *const name, int64 var);

	Assert &Variable(const char *const name, uint8 var);
	Assert &Variable(const char *const name, uint16 var);
	Assert &Variable(const char *const name, uint32 var);
	Assert &Variable(const char *const name, uint64 var);

	template <typename T>
	Assert &Variable(const char *const name, const T &value);
};

#endif