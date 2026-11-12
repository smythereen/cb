/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */
/*******************************************************************************
 * 
 *	(CB) Circular Buffer Routines
 *
 *	This file defines common structures, variables, and routines
 *	needed for interface to CB.
 ******************************************************************************/
#ifndef	CB_H
#define	CB_H

#if	defined( __cplusplus )
extern	"C"	{
#endif

#include	<time.h>
#include	<common.h>

typedef	struct timespec	timespec_t;
typedef	struct timeval	timeval_t;

/*
 *	Common Data Definitions
 */

typedef	enum	cb_cap_e	{
	CB07		= 0x0000007F,
	CB08		= 0x000000FF,
	CB09		= 0x000001FF,
	CB10		= 0x000003FF,
	CB11		= 0x000007FF,
	CB12		= 0x00000FFF,
	CB13		= 0x00001FFF,
	CB14		= 0x00003FFF,
	CB15		= 0x00007FFF,
	CB16		= 0x0000FFFF,
	CB17		= 0x0001FFFF,
	CB18		= 0x0003FFFF,
	CB19		= 0x0007FFFF,
	CB20		= 0x000FFFFF,
	CB21		= 0x001FFFFF,
	CB22		= 0x003FFFFF,
	CB23		= 0x007FFFFF,
	CB24		= 0x00FFFFFF,
	CB25		= 0x01FFFFFF,
	CB26		= 0x03FFFFFF,
	CB27		= 0x07FFFFFF,
	CB28		= 0x0FFFFFFF,
	CB_128		= 0x0000007F,
	CB_256		= 0x000000FF,
	CB_512		= 0x000001FF,
	CB_1024		= 0x000003FF,
	CB_2048		= 0x000007FF,
	CB_4096		= 0x00000FFF,
	CB_8192		= 0x00001FFF,
	CB_16384	= 0x00003FFF,
	CB_32768	= 0x00007FFF,
	CB_65536	= 0x0000FFFF,
	CB_131072	= 0x0001FFFF,
	CB_262144	= 0x0003FFFF,
	CB_524288	= 0x0007FFFF,
	CB_1048576	= 0x000FFFFF,
	CB_2097152	= 0x001FFFFF,
	CB_4194304	= 0x003FFFFF,
	CB_8388608	= 0x007FFFFF,
	CB_16777216	= 0x00FFFFFF,
	CB_33554432	= 0x01FFFFFF,
	CB_67108864	= 0x03FFFFFF,
	CB_134217728	= 0x07FFFFFF,
	CB_268435456	= 0x0FFFFFFF
}	cb_cap_t;

typedef	struct	cb_head_s	{
	volatile	u32	mask;
	volatile	u16	buf_size;
	volatile	s16	signal;
	volatile	pid_t	owner;
	volatile	u32	used;
	volatile	u32	next;
}	cb_head_t;

#define	proto_cb( name, capacity, bufsize )		\
typedef	struct	name##_b_s	{			\
	volatile	pid_t	owner;			\
	volatile	uchar	data[ bufsize ];	\
}	name##_b_t;					\
typedef	struct	name##_s	{			\
	cb_head_t	head;				\
	name##_b_t	cb[ capacity + 1 ];		\
}	name##_t

#define	proto_cb_bio( name )					\
	Mitem	name##_b_l[ ] =	{				\
	S32(		owner,		"CirBuf Owner"		),	\
	Xchar(		data,		"CirBuf Data"		),	\
	{ END_OF_LIST } };

#define	proto_cb_Bio( name, list )					\
	Mitem	name##_b_l[ ] =	{				\
	S32(		owner,		"CirBuf Owner"		),	\
	Struct(		list##_t,	data,	list##_l	),	\
	{ END_OF_LIST } };

#define	proto_cb_mio( name )					\
	Mitem	name##_l[ ] =	{				\
	Struct(		cb_head_t,	head,	cb_head_l	),	\
	Struct_(4,	name##_b_t,	cb,	name##_b_l	),	\
	{ END_OF_LIST } };


/*
 *	Common	Data
 */

/*
 *	Common	Functions
 */

extern	void	CirBuf_Initialize(
	void	*	start_of_buffer_struct,
	cb_cap_t 	circular_buffer_capability,
	u16		buffer_size,
	s16		signal
);
extern	int	CirBuf_Pop_Context( void );
extern	int     CirBuf_Push_Context( void * cb );

extern	void *	CirBuf_Pointer(	s32	buffer_number	);
extern	s32	CirBuf_Number(	void *	buffer_pointer	);

extern	void *	CirBuf_POINTER(	s32	buffer_number	);
extern	s32	CirBuf_NUMBER(	void *	buffer_pointer	);

extern	s32	CirBuf_Number_Allocate( void	);
extern	void *	CirBuf_Pointer_Allocate( void	);

extern	s32	CirBuf_Number_ALLOCATE( void	);
extern	void *	CirBuf_Pointer_ALLOCATE( void	);

extern	int	CirBuf_Number_Free(	s32	buffer_number	);
extern	int	CirBuf_Pointer_Free(	void *	buffer_pointer	);

extern	int	CirBuf_Number_FREE(	s32	buffer_number	);
extern	int	CirBuf_Pointer_FREE(	void *	buffer_pointer	);

extern	s_t	CirBuf_Number_Read( int fork_flag, timespec_t * timeout );
extern	int	CirBuf_Number_Send( pid_t target, s32 buffer_number );

extern	void *	CirBuf_Pointer_Read( int fork_flag, timespec_t * timeout );
extern	int	CirBuf_Pointer_Send( pid_t target, void * buffer_pointer );

#if	defined( __cplusplus )
}
#endif

#endif
