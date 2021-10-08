// if using the Microsoft Visual C++ compiler version .NET, 6.0 or earlier
#if defined(_MSC_VER) && (_MSC_VER <= 1300)

	// disable the compiler warning that says operator-> should not be
	// applied to built in types (i.e. int, float, etc.)
	#pragma warning( disable : 4284 )

	// disable the compiler warning that says "identifier was truncated"
	#pragma warning( disable : 4786 )

#endif	// defined(_MSC_VER) && (_MSC_VER <= 1300)


// if using the Microsoft Visual C++ compiler version .NET2003, .NET, 6.0 or earlier
#if defined(_MSC_VER) && (_MSC_VER <= 1400)

   // disable the compiler warning stating that C++ Exception
	// Specifications are ignored
	#pragma warning( disable : 4290 )

#endif	// defined(_MSC_VER) && (_MSC_VER <= 1300)
