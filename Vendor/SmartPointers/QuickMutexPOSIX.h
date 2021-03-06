////////////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2001, Brad Cupit -- see the bottom of this file for the BSD
//	license under which this source code is being released
////////////////////////////////////////////////////////////////////////////////
//	File Name:	QuickMutex POSIX.h
//
//	Author:		Brad Cupit
//	Project:	C++ Smart Pointers
//	Platforms:	POSIX compliant
//
//	Description:
//		Defines the QuickMutex class for use on a POSIX compliant platform.
//		Inline function implementations are included.
//
////////////////////////////////////////////////////////////////////////////////
//	History:
//	10/09/01	file created
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _QUICKMUTEX_POSIX_H_
#define _QUICKMUTEX_POSIX_H_



namespace ThreadSafe
{


// class declaration

////////////////////////////////////////////////////////////////////////////////
//	QuickMutex:	A thin wrapper around POSIX Mutexes. Each time Lock() is called,
//				Unlock() must be called to release the mutex.
////////////////////////////////////////////////////////////////////////////////
class QuickMutex
{
public:
	inline	QuickMutex( void );
	inline	~QuickMutex( void );

	inline	void Lock( void );
	inline	void Unlock( void );
	inline	bool IsLocked( void );

private:
	pthread_mutex_t		m_PosixMutex;
	bool				m_bLocked;

};	// class QuickMutex



////////////////////////////////////////////////////////////////////////////////
//	inline function implementations
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//	Constructor  [public]
//
// description:
//		Initializes the mutex
inline QuickMutex::QuickMutex( void )
:	m_bLocked( false )
{
	// initialize an intra-process mutex (i.e. only used by this process)
	pthread_mutex_init( &m_PosixMutex, NULL );
}


////////////////////////////////////////////////////////////////////////////////
//	Destructor  [public]
//
// description:
//		Destroys the mutex
inline QuickMutex::~QuickMutex( void )
{
	pthread_mutex_destroy( &m_PosixMutex );
}


////////////////////////////////////////////////////////////////////////////////
//	Lock  [public]
//
// description:
//		Locks the mutex. QuickMutex::Unlock() must be called for each time this
//		function is called
inline void QuickMutex::Lock( void )
{
	pthread_mutex_lock( &m_PosixMutex );
	m_bLocked = true;
}


////////////////////////////////////////////////////////////////////////////////
//	Unlock  [public]
//
// description:
//		Unlocks the mutex. QuickMutex::Lock() must get called before
//		this function can get called
inline void QuickMutex::Unlock( void )
{
	m_bLocked = false;
	pthread_mutex_unlock( &m_PosixMutex );
}


////////////////////////////////////////////////////////////////////////////////
//	IsLocked  [public]
//
// description:
//		determines whether or not the mutex is locked
// return:
//		true if the mutex is locked;
//		false if the mutex is not locked
inline bool QuickMutex::IsLocked( void )
{
	return( m_bLocked );
}


}	// namespace ThreadSafe



#endif	// _QUICKMUTEX_POSIX_H_


/*******************************************************************************
Copyright (c) 2001, Brad Cupit
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Neither the name of this project nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

