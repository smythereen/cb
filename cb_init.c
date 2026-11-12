/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	"cblocal.h"

static	cb_head_t	*	CirBuf_Stack[ 10 ];
	cb_head_t	**	CirBuf_Context;

int	CirBuf_Push_Context( void * cb )
{
	if( CirBuf_Context == NULL || CirBuf_Stack[ 0 ] == NULL ) {
		CirBuf_Context = &CirBuf_Stack[ 0 ];
		*CirBuf_Context = (cb_head_t *)cb;
		return( 0 );
	}
	if( CirBuf_Context >= &CirBuf_Stack[ 9 ] ) {
		return( -1 );				// Failure
	}
	*++CirBuf_Context = (cb_head_t *)cb;
	return( 1 );
}

int	CirBuf_Pop_Context( void )
{
	if( CirBuf_Context == NULL ) {
		CirBuf_Context = &CirBuf_Stack[ 0 ];
		*CirBuf_Context = NULL;
		return( 0 );
	}
	if( CirBuf_Context == &CirBuf_Stack[ 0 ] ) {
		*CirBuf_Context = NULL;
		return( 0 );
	}
	if( CirBuf_Context > &CirBuf_Stack[ 0 ] ) {
		*CirBuf_Context-- = NULL;
		return( 1 );
	}
	return( -1 );
}

void	CirBuf_Initialize( void * cb, cb_cap_t capability, u16 buf_size, s16 signal )
{
	cb_head_t *	hdr;

	memset( cb, 0, sizeof( cb_head_t ) + ( ( capability + 1 ) * buf_size ) );

	CirBuf_Push_Context( cb );
	hdr = *CirBuf_Context;

	hdr->owner	= getpid( );
	hdr->signal	= signal;
	hdr->buf_size	= ( buf_size + sizeof( pid_t ) );
	hdr->mask	= capability;
	hdr->used	= 0;
	hdr->next	= capability;
}
