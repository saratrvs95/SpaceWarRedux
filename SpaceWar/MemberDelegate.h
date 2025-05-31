#pragma once
#include <vector>
#include <functional>

#define MemberDelegate(FnType, TypeName)\
using TypeName = FnType;\
std::vector<TypeName> mDelegates_##TypeName;\
\
void Bind##TypeName(const TypeName& inDelegate)\
{\
	mDelegates_##TypeName.push_back(inDelegate);\
}\
\
void Unbind##TypeName(const TypeName& inDelegate)\
{\
	for (auto it = mDelegates_##TypeName.begin(); it != mDelegates_##TypeName.end(); ++it)\
	{\
		if (it->target<TypeName>() == inDelegate.target<TypeName>())\
		{\
			mDelegates_##TypeName.erase(it);\
			break;\
		}\
	}\
}\
\
void Broadcast##TypeName()\
{\
	for (size_t i = 0; i < mDelegates_##TypeName.size(); ++i)\
	{\
		mDelegates_##TypeName[i]();\
	}\
}\
;

// FnType is a std::function<> type
// TypeName is the 'using' directive that's created for ease of use.
// All identifiers in the macro will be named using the TypeName token
// ParamType is the parameter type for the broadcast function
#define OneParamMemberDelegate(FnType, TypeName, ParamType)\
using TypeName = FnType;\
std::vector<TypeName> mDelegates_##TypeName;\
\
void Bind##TypeName(const TypeName& inDelegate)\
{\
	mDelegates_##TypeName.push_back(inDelegate);\
}\
\
void Unbind##TypeName(const TypeName& inDelegate)\
{\
	for (auto it = mDelegates_##TypeName.begin(); it != mDelegates_##TypeName.end(); ++it)\
	{\
		if (it->target<TypeName>() == inDelegate.target<TypeName>())\
		{\
			mDelegates_##TypeName.erase(it);\
			break;\
		}\
	}\
}\
\
void Broadcast##TypeName(ParamType& in##ParamType)\
{\
	for (size_t i = 0; i < mDelegates_##TypeName.size(); ++i)\
	{\
		mDelegates_##TypeName[i](in##ParamType);\
	}\
}\
;

#define TwoParamMemberDelegate(FnType, TypeName, Param1Type, Param2Type)\
using TypeName = FnType;\
std::vector<TypeName> mDelegates_##TypeName;\
\
void Bind##TypeName(const TypeName& inDelegate)\
{\
	mDelegates_##TypeName.push_back(inDelegate);\
}\
\
void Unbind##TypeName(const TypeName& inDelegate)\
{\
	for (auto it = mDelegates_##TypeName.begin(); it != mDelegates_##TypeName.end(); ++it)\
	{\
		if (it->target<TypeName>() == inDelegate.target<TypeName>())\
		{\
			mDelegates_##TypeName.erase(it);\
			break;\
		}\
	}\
}\
\
void Broadcast##TypeName(const Param1Type& in##Param1Type, Param2Type& in##Param2Type)\
{\
	for (size_t i = 0; i < mDelegates_##TypeName.size(); ++i)\
	{\
		mDelegates_##TypeName[i](in##Param1Type, in##Param2Type);\
	}\
}\
;