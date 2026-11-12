/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	"cblocal.h"
#include	<atomic_32.h>

s32	CirBuf_Number_Allocate( void )
{
	cb_head_t *	h;
	uchar	*	owner;
	u32		i;
	u32		j;
	pid_t		pid;

	if( ( h = CirBuf_Header( ) ) == NULL ) return( -1 );

	for( j = 0; j <= h->mask; j += 1 ) {
		i  = S32_Inc_Return( &h->next );
		i &= h->mask;
		owner = CirBuf_Pointer( i );
		if( ( pid = S32_Read( owner ) ) != 0 &&
		    pid != getpid( ) &&
		    pid != getppid( ) &&
		    kill( pid, 0 ) == 0 ) continue;
		S32_Set( owner, getpid( ) );
		if( S32_Read( owner ) != getpid( ) ) continue;
		if( pid == 0 ) S32_Inc( &h->used );
		return( ( i & h->mask ) );
	}
	Trace( 0, "ERROR: No Circular Buffer Available!" );
	return( -1 );
}

void *	CirBuf_Pointer_Allocate( void )
{
	return( CirBuf_Pointer( CirBuf_Number_Allocate( ) ) );
}
