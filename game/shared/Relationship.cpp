#include "extdll.h"
#include "util.h"

#include "Relationship.h"

const char* RelationshipToPrettyString( const Relationship relationship )
{
	switch( relationship )
	{
	case R_AL:	return "Ally";
	case R_FR:	return "Fear";
	case R_NO:	return "None";
	case R_DL:	return "Dislike";
	case R_HT:	return "Hate";
	case R_NM:	return "Nemesis";
	default: break;
	}

	ASSERT( false );

	return "Unknown";
}
