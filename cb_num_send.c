/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	"cblocal.h"
#include	<signal.h>

int	CirBuf_Number_Send( pid_t target, s32 b )
{
	uchar	*	bp;

	if( ( bp = CirBuf_Pointer( b ) ) == NULL ) {
		errno = EFAULT;
		return( -1 );
	}
	*(pid_t *)bp = target;
	return( sigqueue( target, (*CirBuf_Context)->signal, (sigval_t)((void*)( (u_t)( b & (*CirBuf_Context)->mask ) ) ) ) );
}
