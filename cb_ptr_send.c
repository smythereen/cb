/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	"cblocal.h"
#include	<signal.h>

int	CirBuf_Pointer_Send( pid_t target, void * cb )
{
	s32	b;

	if( ( b = CirBuf_Number( cb ) ) == -1 ) {
		errno = EFAULT;
		return( -1 );
	}
	cb = &((uchar *)(*CirBuf_Context))[ ( (*CirBuf_Context)->buf_size * b ) + sizeof( cb_head_t ) ];
	*(pid_t *)cb = target;
	return( sigqueue( target, (*CirBuf_Context)->signal, (sigval_t)((void*)( (u_t)( b ) ) ) ) );
}
