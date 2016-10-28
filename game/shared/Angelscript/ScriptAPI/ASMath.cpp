#include <cfloat>
#include <cstdint>
#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"

#include "mathlib.h"

#include "ASMath.h"

//Vector2D
void ConstructVector2D( Vector2D* pMemory )
{
	new ( pMemory ) Vector2D();
}

void CopyConstructVector2D( Vector2D* pMemory, const Vector2D& vec )
{
	new ( pMemory ) Vector2D( vec );
}

void Float2ConstructVector2D( Vector2D* pMemory, float x, float y )
{
	new ( pMemory ) Vector2D( x, y );
}

void DestructVector2D( Vector2D* pMemory )
{
	pMemory->~Vector2D();
}

static bool Vector2D_Equals( Vector2D* pThis, const Vector2D& vec )
{
	return ( *pThis == vec ) != 0;
}

static std::string Vector2D_ToString( const Vector2D* pThis )
{
	char szBuffer[ 512 ];

	snprintf( szBuffer, sizeof( szBuffer ), "%f, %f", pThis->x, pThis->y );

	return szBuffer;
}

static void RegisterScriptVector2D( asIScriptEngine& engine )
{
	const char* const pszObjectName = "Vector2D";

	engine.RegisterObjectType(
		pszObjectName, sizeof( Vector2D ),
		asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_ALLFLOATS );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void Vector2D()",
		asFUNCTION( ConstructVector2D ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void Vector2D(const Vector2D& in other)",
		asFUNCTION( CopyConstructVector2D ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void Vector2D(float x, float y)",
		asFUNCTION( Float2ConstructVector2D ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_DESTRUCT, "void DestructVector2D()",
		asFUNCTION( DestructVector2D ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectProperty(
		pszObjectName, "float x",
		asOFFSET( Vector2D, x ) );

	engine.RegisterObjectProperty(
		pszObjectName, "float y",
		asOFFSET( Vector2D, y ) );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D& opAssign(const Vector2D& in other)",
		asMETHODPR( Vector2D, operator=, ( const Vector2D& ), Vector2D& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D opAdd(const Vector2D& in other) const",
		asMETHODPR( Vector2D, operator+, ( const Vector2D& ) const, Vector2D ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D opSub(const Vector2D& in other) const",
		asMETHODPR( Vector2D, operator-, ( const Vector2D& ) const, Vector2D ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D opMul(float fl) const",
		asMETHODPR( Vector2D, operator*, ( float ) const, Vector2D ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D opMul_r(float fl) const",
		asMETHODPR( Vector2D, operator*, ( float ) const, Vector2D ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D opDiv(float fl) const",
		asMETHODPR( Vector2D, operator/, ( float ) const, Vector2D ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D opDiv_r(float fl) const",
		asMETHODPR( Vector2D, operator/, ( float ) const, Vector2D ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool opEquals(const Vector2D& in other) const",
		asFUNCTION( Vector2D_Equals ), asCALL_CDECL_OBJLAST );

	engine.RegisterObjectMethod(
		pszObjectName, "float Length() const",
		asMETHOD( Vector2D, Length ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D Normalize() const",
		asMETHOD( Vector2D, Normalize ), asCALL_THISCALL );

	engine.RegisterGlobalFunction(
		"float DotProduct(const Vector2D& in lhs, const Vector2D& in rhs)",
		asFUNCTIONPR( DotProduct, ( const Vector2D&, const Vector2D& ), float ), asCALL_CDECL );

	engine.RegisterObjectMethod(
		pszObjectName, "string ToString() const",
		asFUNCTION( Vector2D_ToString ), asCALL_CDECL_OBJFIRST );
}

//Vector
void ConstructVector3D( Vector* pMemory )
{
	new ( pMemory ) Vector();
}

void CopyConstructVector3D( void* pMemory, const Vector& vec )
{
	new ( pMemory ) Vector( vec );
}

void Float3ConstructVector3D( Vector* pMemory, float x, float y, float z )
{
	new ( pMemory ) Vector( x, y, z );
}

void DestructVector3D( Vector* pMemory )
{
	pMemory->~Vector();
}

static bool Vector_Equals( const Vector& vec, Vector* pThis )
{
	return ( *pThis == vec ) != 0;
}

static std::string Vector3D_ToString( const Vector* pThis )
{
	char szBuffer[ 512 ];

	snprintf( szBuffer, sizeof( szBuffer ), "%f, %f, %f", pThis->x, pThis->y, pThis->z );

	return szBuffer;
}

static float& Vector3D_opIndex( Vector* pThis, uint32_t uiIndex )
{
	return ( *pThis )[ uiIndex ];
}

static float Vector3D_opIndex_const( const Vector* pThis, uint32_t uiIndex )
{
	return ( *pThis )[ uiIndex ];
}

void RegisterScriptVector3D( asIScriptEngine& engine )
{
	const char* const pszObjectName = "Vector";

	engine.RegisterObjectType(
		pszObjectName, sizeof( Vector ),
		asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_ALLFLOATS );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void Vector()",
		asFUNCTION( ConstructVector3D ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void Vector(const Vector& in vec)",
		asFUNCTION( CopyConstructVector3D ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void Vector(float x, float y, float z)",
		asFUNCTION( Float3ConstructVector3D ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_DESTRUCT, "void DestructVector()",
		asFUNCTION( DestructVector3D ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectProperty(
		pszObjectName, "float x",
		asOFFSET( Vector, x ) );

	engine.RegisterObjectProperty(
		pszObjectName, "float y",
		asOFFSET( Vector, y ) );

	engine.RegisterObjectProperty(
		pszObjectName, "float z",
		asOFFSET( Vector, z ) );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector& opAssign(const Vector& in other)",
		asMETHODPR( Vector, operator=, ( const Vector& ), Vector& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector opNeg() const",
		asMETHODPR( Vector, operator-, ( ) const, Vector ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector opAdd(const Vector& in other) const",
		asMETHODPR( Vector, operator+, ( const Vector& ) const, Vector ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector opSub(const Vector& in other) const",
		asMETHODPR( Vector, operator-, ( const Vector& ) const, Vector ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector opMul(float fl) const",
		asMETHODPR( Vector, operator*, ( float ) const, Vector ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector opMul_r(float fl) const",
		asMETHODPR( Vector, operator*, ( float ) const, Vector ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector opDiv(float fl) const",
		asMETHODPR( Vector, operator/, ( float ) const, Vector ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector opDiv_r(float fl) const",
		asMETHODPR( Vector, operator/, ( float ) const, Vector ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float& opIndex(uint32 uiIndex)",
		asFUNCTION( Vector3D_opIndex ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "float opIndex(uint32 uiIndex) const",
		asFUNCTION( Vector3D_opIndex_const ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "bool opEquals(const Vector& in other) const",
		asFUNCTION( Vector_Equals ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "float Length() const",
		asMETHOD( Vector, Length ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float Length2D() const",
		asMETHOD( Vector, Length2D ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector Normalize() const",
		asMETHOD( Vector, Normalize ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector2D Make2D() const",
		asMETHOD( Vector, Make2D ), asCALL_THISCALL );

	engine.RegisterGlobalFunction(
		"float DotProduct(const Vector& in lhs, const Vector& in rhs)",
		asFUNCTIONPR( DotProduct, ( const Vector&, const Vector& ), float ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"Vector CrossProduct(const Vector& in, const Vector& in)",
		asFUNCTIONPR( CrossProduct, ( const Vector&, const Vector& ), Vector ), asCALL_CDECL );

	engine.RegisterGlobalProperty(
		"const Vector g_vecZero",
		const_cast<Vector*> ( &g_vecZero ) );

	engine.RegisterObjectMethod(
		pszObjectName, "string ToString() const",
		asFUNCTION( Vector3D_ToString ), asCALL_CDECL_OBJFIRST );
}

//Angelscript expects actual variables to back up exposed variables.
//Using different names to avoid the preprocessor replacing them with constants.
static const int8_t CONST_INT8_MIN		= INT8_MIN;
static const int16_t CONST_INT16_MIN	= INT16_MIN;
static const int32_t CONST_INT32_MIN	= INT32_MIN;
static const int64_t CONST_INT64_MIN	= INT64_MIN;

static const int8_t CONST_INT8_MAX		= INT8_MAX;
static const int16_t CONST_INT16_MAX	= INT16_MAX;
static const int32_t CONST_INT32_MAX	= INT32_MAX;
static const int64_t CONST_INT64_MAX	= INT64_MAX;

static const uint8_t CONST_UINT8_MAX	= UINT8_MAX;
static const uint16_t CONST_UINT16_MAX	= UINT16_MAX;
static const uint32_t CONST_UINT32_MAX	= UINT32_MAX;
static const uint64_t CONST_UINT64_MAX	= UINT64_MAX;

static const float CONST_FLOAT_MIN		= FLT_MIN;
static const double CONST_DOUBLE_MIN	= DBL_MIN;

static const float CONST_FLOAT_MAX		= FLT_MAX;
static const double CONST_DOUBLE_MAX	= DBL_MAX;

static const double CONST_PI			= M_PI;

static void RegisterScriptMathConstants( asIScriptEngine& engine )
{
	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "Math" );

	engine.RegisterGlobalProperty(
		"const int8 INT8_MIN",
		const_cast<int8_t*>( &CONST_INT8_MIN ) );

	engine.RegisterGlobalProperty(
		"const int16 INT16_MIN",
		const_cast<int16_t*>( &CONST_INT16_MIN ) );

	engine.RegisterGlobalProperty(
		"const int32 INT32_MIN",
		const_cast<int32_t*>( &CONST_INT32_MIN ) );

	engine.RegisterGlobalProperty(
		"const int64 INT64_MIN",
		const_cast<int64_t*>( &CONST_INT64_MIN ) );

	engine.RegisterGlobalProperty(
		"const int8 INT8_MAX",
		const_cast<int8_t*>( &CONST_INT8_MAX ) );

	engine.RegisterGlobalProperty(
		"const int16 INT16_MAX",
		const_cast<int16_t*>( &CONST_INT16_MAX ) );

	engine.RegisterGlobalProperty(
		"const int32 INT32_MAX",
		const_cast<int32_t*>( &CONST_INT32_MAX ) );

	engine.RegisterGlobalProperty(
		"const int64 INT64_MAX",
		const_cast<int64_t*>( &CONST_INT64_MAX ) );

	engine.RegisterGlobalProperty(
		"const uint8 UINT8_MAX",
		const_cast<uint8_t*>( &CONST_UINT8_MAX ) );

	engine.RegisterGlobalProperty(
		"const uint16 UINT16_MAX",
		const_cast<uint16_t*>( &CONST_UINT16_MAX ) );

	engine.RegisterGlobalProperty(
		"const uint32 UINT32_MAX",
		const_cast<uint32_t*>( &CONST_UINT32_MAX ) );

	engine.RegisterGlobalProperty(
		"const uint64 UINT64_MAX",
		const_cast<uint64_t*>( &CONST_UINT64_MAX ) );

	engine.RegisterGlobalProperty(
		"const float FLOAT_MIN",
		const_cast<float*>( &CONST_FLOAT_MIN ) );

	engine.RegisterGlobalProperty(
		"const double DOUBLE_MIN",
		const_cast<double*>( &CONST_DOUBLE_MIN ) );

	engine.RegisterGlobalProperty(
		"const float FLOAT_MAX",
		const_cast<float*>( &CONST_FLOAT_MAX ) );

	engine.RegisterGlobalProperty(
		"const double DOUBLE_MAX",
		const_cast<double*>( &CONST_DOUBLE_MAX ) );

	engine.RegisterGlobalProperty(
		"const double PI",
		const_cast<double*>( &CONST_PI ) );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

//Pass by value wrappers for AS.
template<typename T>
static T AS_Min( T lhs, T rhs )
{
	return min( lhs, rhs );
}

template<typename T>
static T AS_Max( T lhs, T rhs )
{
	return max( lhs, rhs );
}

template<typename T>
static T AS_Clamp( T value, T min, T max )
{
	return clamp( value, min, max );
}

//TODO: move to mathlib - Solokiller
float UTIL_DegreesToRadians( float flDegrees )
{
	return flDegrees * ( M_PI / 180.0 );
}

float UTIL_RadiansToDegrees( float flRadians )
{
	return flRadians * ( 180.0 / M_PI );
}

static void RegisterScriptMathFunctions( asIScriptEngine& engine )
{
	engine.RegisterGlobalFunction(
		"int32 RandomLong(int32 low, int32 high)",
		asFUNCTION( UTIL_RandomLong ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"float RandomFloat(float low, float high)",
		asFUNCTION( UTIL_RandomFloat ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void MakeVectors(const Vector& in vecAngles)",
		asFUNCTION( UTIL_MakeVectors ), asCALL_CDECL );

	//TODO: implement. - Solokiller
	/*
	engine.RegisterGlobalFunction(
		"Vector RotateVector(Vector& in vecToRotate,"
		"const Vector& in vecAngles, const Vector& in vecOffset)",
		asFUNCTION( UTIL_RotateVector ), asCALL_CDECL );
		*/

	engine.RegisterGlobalFunction(
		"void MakeAimVectors(const Vector& in vecAngles)",
		asFUNCTION( UTIL_MakeAimVectors ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void MakeInvVectors(const Vector& in vecAngles)",
		asFUNCTION( UTIL_MakeInvVectors ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"float VecToYaw(const Vector& in vec)",
		asFUNCTION( UTIL_VecToYaw ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"Vector VecToAngles(const Vector& in vec)",
		asFUNCTION( UTIL_VecToAngles ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"float AngleMod(float flAngle)",
		asFUNCTION( UTIL_AngleMod ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"float AngleDiff(float flDestAngle, float flSrcAngle)",
		asFUNCTION( UTIL_AngleDiff ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"float ApproachAngle(float target, float value, float speed)",
		asFUNCTION( UTIL_ApproachAngle ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"float AngleDistance(float flNext, float flCur)",
		asFUNCTION( UTIL_AngleDistance ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"uint64 min(uint64 lhs, uint64 rhs)",
		asFUNCTION( AS_Min<uint64_t> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"int64 min(int64 lhs, int64 rhs)",
		asFUNCTION( AS_Min<int64_t> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"double min(double lhs, double rhs)",
		asFUNCTION( AS_Min<double> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"uint64 max(uint64 lhs, uint64 rhs)",
		asFUNCTION( AS_Max<uint64_t> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"int64 max(int64 lhs, int64 rhs)",
		asFUNCTION( AS_Max<int64_t> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"double max(double lhs, double rhs)",
		asFUNCTION( AS_Max<double> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"uint64 clamp(uint64 value, uint64 min, uint64 max)",
		asFUNCTION( AS_Clamp<uint64_t> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"int64 clamp(int64 value, int64 min, int64 max)",
		asFUNCTION( AS_Clamp<int64_t> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"double clamp(double value, double min, double max)",
		asFUNCTION( AS_Clamp<double> ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"float DegreesToRadians(float flDegrees)",
		asFUNCTION( UTIL_DegreesToRadians ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"float RadiansToDegrees(float flRadians)",
		asFUNCTION( UTIL_RadiansToDegrees ), asCALL_CDECL );
}

void RegisterScriptHLMath( asIScriptEngine& engine )
{
	RegisterScriptVector2D( engine );
	RegisterScriptVector3D( engine );
	RegisterScriptMathConstants( engine );
	RegisterScriptMathFunctions( engine );
}
