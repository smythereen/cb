/* SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later */
/* Copyright (c) 2008-2026 John R. Smythe Jr. */

#include	"cblocal.h"
#include	<signal.h>

static	int			CB_Child;
static	struct	sigaction	new;

s_t	CirBuf_Number_Read( int fork_flag, timespec_t * timeout )
{
	s_t		s;
	sigset_t	set;
	sigset_t	oldset;
	siginfo_t	info;
	timespec_t	save;
	char		buf[ 1024 ];

	if( CirBuf_Header == NULL ) return( -1 );

	if( fork_flag ) {
		if( CB_Child ) exit( 0 );
		if( new.sa_flags == 0 ) {
			memset( &new, 0, sizeof( new ) );
			new.sa_handler = SIG_DFL;
			sigfillset( &new.sa_mask );
			new.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT | SA_SIGINFO;
			for( s = 0; s < 65; s++ ) sigaction( s, &new, NULL );
		}
		CB_Child = 0;
	}
	
	sigfillset( &set );
	sigprocmask( SIG_SETMASK, &set, &oldset );			// Block All Signals

	if( timeout != NULL ) memcpy( &save, timeout, sizeof( timespec_t ) );

	for( ; ; ) {
		memset( &info, 0, sizeof( info ) );

		switch( ( s = sigtimedwait( &set, &info, timeout ) ) ) {	// Wait For Any Signal

		case 1 ... 64:
			if( (s_t)(*CirBuf_Context)->signal != s ) continue;
			if( info.si_code != SI_QUEUE ) continue;
			if( fork_flag ) {
				switch( fork( ) ) {
				default:	continue;	// Parent
				case	 0:	CB_Child = 1;	// Child
				case	-1:	break;		// Error
				}
			}
			return( (s_t)info.si_value.sival_ptr );
			

		case -1:
			switch( errno ) {
			case EAGAIN:					// Timeout
//				Trace( 0, "Recieved Timeout" );
				if( fork_flag ) {
					switch( fork( ) ) {
					default:	continue;	// Parent
					case	 0:	CB_Child = 1;	// Child
					case	-1:	break;		// Error
					}
				}
				return( (s_t)-2 );

			case EINVAL:
				Trace( 0, "Invalid Timeout" );
				return( (s_t)-1 );

			case EINTR:
				Trace( 0, "Interrupted By Other Signal" );
				continue;

			default:
				strerror_r( errno, buf, sizeof( buf ) );
				Trace( 0, "Unexpected Error %u - %s", errno, buf );
				return( (s_t)-1 );
			}
		
		default:
			Trace( 0, "Received Unknown Signal %d", s );
			continue;
		}
	}
}

void *	CirBuf_Pointer_Read( int fork_flag, timespec_t * timeout )
{
	return( CirBuf_Pointer( CirBuf_Number_Read( fork_flag, timeout ) ) );
}
