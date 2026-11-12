/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	"cblocal.h"

s32	CirBuf_Number( void * cb )
{
	if( CirBuf_Header( ) == NULL ) return( -1 );

	if( cb < CirBuf_START ) {
		Trace(	0,
			"ERROR: Invalid CirBuf Pointer 0x%016LX! Data Start=0x%016LX",
			(u64)cb,
			(u64)CirBuf_START
		);
		return( -1 );
	}
	if( cb > CirBuf_LAST ) {
		Trace(	0,
			"ERROR: Invalid CirBuf Pointer 0x%016LX! Data Last=0x%016LX",
			(u64)cb,
			(u64)CirBuf_LAST
		);
		return( -1 );
	}
	return( CirBuf_INDEX( cb ) );
}
