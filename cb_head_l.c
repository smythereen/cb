/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Circular Buffer Header Memory Format Data
 ******************************************************************************/
#include	"cblocal.h"
#include	<mem_io.h>

#define	STRUCT	cb_head_t
	Mitem		cb_head_l[ ] =	{
	X32(		mask,		"Circular Buffer Size Bit Mask"		),
	U16(		buf_size,	"Buffer Size"				),
	S16(		signal,		"Real-Time Signal Number"		),
	U32(		owner,		"Circular Buffer Owner"			),
	U32(		used,		"Number Of Buffers In Use"		),
	U32(		next,		"Last Buffer Consumed"			),
	{ END_OF_LIST }
};
#undef	STRUCT

