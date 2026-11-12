/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	"cblocal.h"

cb_head_t	*	CirBuf_Header( void )
{
	if(	  CirBuf_Context		!= NULL	&&
		 *CirBuf_Context		!= NULL	&&
		(*CirBuf_Context)->mask		> 0	&& 
		(*CirBuf_Context)->buf_size	> 0	) return( *CirBuf_Context );
	Trace( 0, "ERROR: CirBuf_Initialize Or CirBuf_Push_Context has NOT been Called!" );
	return( NULL );
}
