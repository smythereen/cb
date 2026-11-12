/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	This file defines the following function.
 ******************************************************************************/
#include	"cblocal.h"

s32	CirBuf_Number_Header( void * cb, cb_head_t ** hdr )
{
	cb_head_t *	h;
	uchar	*	sp;
	uchar	*	lp;
	u_t		i;

	h = CirBuf_Header( );
	if( hdr != NULL ) *hdr = h;
	if( h == NULL ) return( -1 );

	sp = (uchar *)h;
	sp += sizeof( cb_head_t );
	if( cb < (void *)sp ) {
		Trace(	0,
			"ERROR: Invalid CirBuf Pointer 0x%016LX! Data Start=0x%016LX",
			(u64)cb,
			(u64)sp
		);
		return( -1 );
	}
	lp = sp;
	lp += ( ( ( h->mask + 1 ) * h->buf_size ) - 1 );
	if( cb > (void *)lp ) {
		Trace(	0,
			"ERROR: Invalid CirBuf Pointer 0x%016LX! Data Last=0x%016LX",
			(u64)cb,
			(u64)lp
		);
		return( -1 );
	}
	i = (u_t)( cb - (void *)sp );
	i /= h->buf_size;
	return( (s32)i );
}
