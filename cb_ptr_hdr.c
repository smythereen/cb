/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	This file defines the following function.
 ******************************************************************************/
#include	"cblocal.h"

void	*	CirBuf_Pointer_Header( s32 b, cb_head_t ** hdr )
{
	cb_head_t	*	h;

	h = CirBuf_Header( );
	if( hdr != NULL ) *hdr = h;
	if( b < 0 || h == NULL ) return( NULL );
	return( &((uchar *)h)[ ( h->buf_size * ( b & h->mask ) ) + sizeof( cb_head_t ) ] );
}
