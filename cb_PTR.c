/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	"cblocal.h"

void	*	CirBuf_Pointer( s32 b )
{
	return( b < 0 || CirBuf_Header( ) == NULL ? NULL : CirBuf_ADDR( b ) );
}
