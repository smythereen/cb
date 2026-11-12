/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	Circular Buffer Routines Private Header
 *
 *	This file defines local structures, variables, and routines
 *	needed for interface to CB.
 ******************************************************************************/
#ifndef	CBLOCAL_H
#define	CBLOCAL_H

#if	defined( __cplusplus )
extern	"C"	{
#endif

#include	<atomic_32.h>
#include	<signal.h>
#include	<common.h>
#include	<cb.h>

/*
 *	Private Definitions
 */

#define	CirBuf_HEADER	((cb_head_t *)((CirBuf_Context!=NULL&&*CirBuf_Context!=NULL&&(*CirBuf_Context)->mask>0&&(*CirBuf_Context)->buf_size>0)?*CirBuf_Context:NULL))

#define	CirBuf_START	((void*)&((uchar *)(*CirBuf_Context))[sizeof(cb_head_t)])
#define	CirBuf_LAST	((void*)&((uchar *)(*CirBuf_Context))[sizeof(cb_head_t)+((((*CirBuf_Context)->mask+1)*(*CirBuf_Context)->buf_size)-1)])

#define	CirBuf_ADDR(b)	((void*)&((uchar *)(*CirBuf_Context))[ sizeof(cb_head_t) + ( (*CirBuf_Context)->buf_size * ( b & (*CirBuf_Context)->mask ) ) ] )
#define	CirBuf_INDEX(p)	((s32)((u_t)(p-CirBuf_START)/(*CirBuf_Context)->buf_size))

/*
 *	Private Structures
 */

/*
 *	Private Data
 */

extern	cb_head_t    **      CirBuf_Context;

/*
 *	Private	Functions
 */

extern	cb_head_t	*	CirBuf_Header( void );

#if	defined( __cplusplus )
}
#endif

#endif
