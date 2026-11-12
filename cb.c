/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	<sys/time.h>
#include	<sys/types.h>
#include	<signal.h>

#include	<cb.h>
#include	<tps.h>
#include	<map.h>
#include	<mem_io.h>

	proto_cb( test, CB10, 64 );

typedef	struct	tstmem_s	{
	u32	trace;
	test_t	x;
	tps_t	tps;
}	tstmem_t;

	tstmem_t	*	tstmem;

extern	Mitem	tps_l[ ];
extern	Mitem	cb_head_l[ ];

#define STRUCT  test_b_t
	proto_cb_bio( test );
#undef  STRUCT

#define STRUCT  test_t
	proto_cb_mio( test );
#undef  STRUCT

#define	STRUCT	tstmem_t
	Mitem		tstmem_l[ ] =	{
	X32(		trace,		"Trace Flags"			),
	Struct(		test_t,		x,		test_l		),
	Struct(		tps_t,		tps,		tps_l		),
	{ END_OF_LIST }
};
#undef  STRUCT

static	void	server( int ac, char *av[ ] )
{
	s32	x;
	timespec_t	t;

	timeval_t *	s;
	timeval_t 	e;

	memset( &tstmem->x, 0, sizeof( test_t ) );
	memset( &tstmem->tps, 0, sizeof( tps_t ) );
	CirBuf_Initialize( &tstmem->x, CB10, 64, 40 );

	for( ; ; ) {
		t.tv_sec = 30;
		t.tv_nsec = 0;
		x = CirBuf_Number_Read( 1, &t );
		switch( x ) {
		case	-1:	Trace( 0, "Server Error! - Goodbye." );
				exit( 2 );
		case	-2:	Trace( 0, "Timeout." );
				kill( getppid( ), 9 );
				exit( 0 );
		default:	
				break;
		}
		TPS_Bump( &tstmem->tps );
		gettimeofday( &e, 0 );
		s = (timeval_t *)((test_b_t *)CirBuf_Pointer( x ))->data;
		while( e.tv_usec < s->tv_usec ) {
			e.tv_sec  -= 1;
			e.tv_usec += 1000000;
		}
#ifdef	JUNK
		Trace(	0,
			"Server Rcv(%4u): %d.%06u secs",
			x,
			e.tv_sec - s->tv_sec,
			e.tv_usec - s->tv_usec
		);
#endif
		CirBuf_Pointer_Free( s );
	}
	exit( 0 );
}

static	void	client( int ac, char * av[ ] )
{
	pid_t		tgt;
	timeval_t *	s;
	timespec_t	x;

	if( ac != 1 ) exit( 3 );

	tgt = strtoul( av[0], 0, 10 );

	CirBuf_Push_Context( &tstmem->x );

	for( ; ; ) {
		s = (timeval_t *)((test_b_t *)CirBuf_Pointer_Allocate( ))->data;
		if( s == NULL ) break;
		gettimeofday( s, 0 );
		if( CirBuf_Pointer_Send( tgt, s ) == -1 ) break;
		x.tv_sec = 0;
		x.tv_nsec = 25000;
		nanosleep( &x, NULL );
	}
	exit( 0 );
}

static	void	memory( int ac, char * av[ ] )
{
	static	char	prefix[ ] = "cb memory";

	switch( ac ) {
	default:	Input_Memory_Item(  tstmem, tstmem_l, av[0], av[1] );
			UpdateMap( tstmem, sizeof( tstmem_t ) );
	case	1:	Output_Memory_Item( tstmem, tstmem_l, av[0], prefix );
			break;
	case	0:	Output_Memory_Item( tstmem, tstmem_l,     0, prefix );
			break;
	}
	UnmapAddress( tstmem, 1 );
	exit( 0 );
}

static	struct	list_s	{
	char *	cmd;
	void (*func)( int ac, char *av[ ] );
}	list[ ]	=	{
	{	"client",		client		},
	{	"memory",		memory		},
	{	"server",		server		},
	{	(char *)0,		(void (*)( int, char *[ ] ))0	}
};

static	void	usage( char * av[ ] )
{
	fprintf( stderr,
		 "\n\tCircular Buffer (CB) Routines - Verification Program\n\n"
		 "Usage:\n"
		 "\t  \t c[lient]	server_pid\n"
		 "\t%s\t s[erver]\n"
		 "\t  \t m[emory]	[C.Variable.Name [ Value ] ]\n"
		 "\n",
		 av[ 0 ]
	);
	fflush( stderr );
	exit( 1 );
}

int	main( int ac, char * av[ ] )
{
struct	list_s	*	l;
	int		len;

	LocalName = av[ 0 ];
//	TraceLevel = CB_TRC_LVL;
	switch( ac ) {
	default:	len = strlen( av[ 1 ] );
			tstmem = MapFile2Mem( "cb_test.mem", sizeof( tstmem_t ), PROT_WRITE );
			if( tstmem != NULL ) {
				for( l = list; l->cmd != (char *)0; l++ ) {
					if( strncmp( av[ 1 ], l->cmd, len ) ) continue;
					l->func( ( ac - 2 ), &av[ 2 ] );
				}
			}
	case  1:
	case  0:	usage( av );
	}
	exit( 0 );
}
