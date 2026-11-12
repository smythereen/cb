/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	This file defines the following function.
 ******************************************************************************/
#include	"cblocal.h"
#include	<atomic_32.h>

int	CirBuf_Number_Free( s32 b )
{
	s32	*	owner;
	pid_t		pid;

	if( ( owner = CirBuf_Pointer( b ) ) == NULL ) return( 0 );
	if( ( pid = S32_Read( owner ) ) == 0 ) return( 1 );
	if( pid == getpid( ) || pid == getppid( ) ) {
		S32_Set( owner, 0 );
		S32_Dec( &(*CirBuf_Context)->used );
		return( 1 );
	}
	Trace(	0,
		"ERROR: Cannot Free Buffer %u - PID: %u Is Not Buffer Owner %u!",
		b,
		getpid( ),
		S32_Read( owner )
	);
	return( 0 );
}

int	CirBuf_Pointer_Free( void * cb )
{
	return( CirBuf_Number_Free( CirBuf_Number( cb ) ) );
}
