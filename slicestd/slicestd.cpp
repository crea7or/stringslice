#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

template < typename _inputIterator, typename _outputIterator>
void each_slice_push( _inputIterator _begin, _inputIterator _end, typename _inputIterator::value_type _delimiter, _outputIterator _output )
{
	_inputIterator _first = _begin, _cur = _begin;
	for ( ; _begin != _end; ++_begin )
	{
		if ( *_begin == _delimiter )
		{
			if ( _first != _cur )
			{
				_output = std::basic_string<typename _inputIterator::value_type>( _first, _cur );
			}
			_first = ++_cur;
		}
		else
		{
			++_cur;
		}
	}
	if ( _first != _cur )
	{
		_output = std::basic_string<typename _inputIterator::value_type>( _first, _cur );
	}
}

template < typename _inputIterator, typename _outputIterator >
void each_slice_push( _inputIterator _begin, _inputIterator _end, typename _inputIterator::value_type* _delimiter, _outputIterator _output )
{
	size_t indexCurrent = 0, stringSize = 0;
	while ( _delimiter[ stringSize ] != 0 )
	{
		++stringSize;
	};

	if ( stringSize > 1 )
	{
		--stringSize;
		_inputIterator _first = _begin, _final = _begin, _last, _fromPattern;
		for ( ; _begin != _end; ++_begin )
		{
			if ( *_begin == _delimiter[ indexCurrent ] )
			{
				_last = _fromPattern = _begin;
				while ( ++_fromPattern != _end && *_fromPattern == _delimiter[ ++indexCurrent ] )
				{
					if ( indexCurrent == stringSize )
					{
						if ( _first != _last )
						{
							_output = std::basic_string<typename _inputIterator::value_type>( _first, _last );
						}
						_begin = _fromPattern++;
						_final = _first = _fromPattern;
						break;
					}
				};
				indexCurrent = 0;
			}
		}

		// last slice
		if ( _final != _end )
		{
			_output = std::basic_string<typename _inputIterator::value_type>( _final, _end );
		}
	}
	else if ( stringSize == 1 )
	{
		// only one symbol in delimiter, use simplier variant
		each_slice_push( _begin, _end, _delimiter[ 0 ], _output );
	}
	/*
	else
	{
	// only one slice, no delimiters supplied
	_function( std::basic_string<typename _inputIterator::value_type>( _begin, _end ));
	}
	*/
}

template < typename _inputIterator, typename _unaryFunction >
void each_slice( _inputIterator _begin, _inputIterator _end, typename _inputIterator::value_type _delimiter, _unaryFunction _function )
{
	_inputIterator _first = _begin, _cur = _begin;
	for ( ; _begin != _end; ++_begin )
	{
		if ( *_begin == _delimiter )
		{
			if ( _first != _cur )
			{
				_function( std::basic_string<typename _inputIterator::value_type>( _first, _cur ));
			}
			_first = ++_cur;
		}
		else
		{
			++_cur;
		}
	}
	if ( _first != _cur )
	{
		_function( std::basic_string<typename _inputIterator::value_type>( _first, _cur ));
	}
}

template < typename _inputIterator, typename _unaryFunction >
void each_slice( _inputIterator _begin, _inputIterator _end, typename _inputIterator::value_type* _delimiter, _unaryFunction _function )
{
	size_t indexCurrent = 0, stringSize = 0;
	while ( _delimiter[ stringSize ] != 0 )
	{
		++stringSize;
	};

	if ( stringSize > 1 )
	{
		--stringSize;
		_inputIterator _first = _begin, _final = _begin, _last, _fromPattern;
		for ( ; _begin != _end; ++_begin )
		{
			if ( *_begin == _delimiter[ indexCurrent ] )
			{
				_last = _fromPattern = _begin;
				while ( ++_fromPattern != _end && *_fromPattern == _delimiter[ ++indexCurrent ] )
				{
					if ( indexCurrent == stringSize )
					{
						if ( _first != _last )
						{
							_function( std::basic_string<typename _inputIterator::value_type>( _first, _last ) );
						}
						_begin = _fromPattern++;
						_final = _first = _fromPattern;
						break;
					}
				};
				indexCurrent = 0;
			}
		}

		// last slice
		if ( _final != _end )
		{
			_function( std::basic_string<typename _inputIterator::value_type>( _final, _end ) );
		}
	}
	else if ( stringSize == 1 )
	{
		// only one symbol in delimiter, use simplier variant
		each_slice( _begin, _end, _delimiter[ 0 ], _function );
	}
	/*
	else
	{
		// only one slice, no delimiters supplied
		_function( std::basic_string<typename _inputIterator::value_type>( _begin, _end ));
	}
	*/
}

template < typename _inputIterator, typename _unaryFunction >
void any_slice( _inputIterator _begin, _inputIterator _end, typename _inputIterator::value_type* _delimiter, _unaryFunction _function )
{
	size_t indexCurrent = 0, stringSize = 0;
	while ( _delimiter[ stringSize ] != 0 )
	{
		++stringSize;
	};

	if ( stringSize > 0 )
	{
		_inputIterator _first = _begin, _cur = _begin;
		for ( ; _begin != _end; ++_begin )
		{
			for ( indexCurrent = 0; indexCurrent < stringSize; ++indexCurrent )
			{
				if ( *_begin == _delimiter[ indexCurrent ] )
				{
					break;
				}
			}
			if ( indexCurrent != stringSize )
			{
				if ( _first != _cur )
				{
					_function( std::basic_string<typename _inputIterator::value_type>( _first, _cur ) );
				}
				_first = ++_cur;
			}
			else
			{
				++_cur;
			}
		}
		if ( _first != _cur )
		{
			_function( std::basic_string<typename _inputIterator::value_type>( _first, _cur ) );
		}
	}
}



int main(int argc, char* argv[])
{
	std::string test( "cpp.kw1 source.cpp.kw2 source.cpp.kw3 sourcesource.cpp.kw4_source.cpp.co1 source.cpp.co2 source.cpp source.cpp.es0 source" );
	std::wstring wtest( test.begin(), test.end() );

	size_t count = 0;

#pragma region single char slices


	std::cout << std::endl << "char (' '):" << std::endl;
	each_slice( test.begin(), test.end(), ' ', [&count]( std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	});
	std::cout << "total tags: " << count << std::endl;


	std::cout << std::endl << "char + outputIterator (' '):" << std::endl;
	std::vector< std::string > slices;
	std::back_insert_iterator<std::vector< std::string >>  output( slices );
	each_slice_push( test.begin(), test.end(), ' ', output ); // <- char to wchar implict convertsion is possible

	for_each( slices.begin(), slices.end(), []( std::string& tag )
	{
		std::cout << tag << std::endl;
	} );
	std::cout << "total tags: " << slices.size() << std::endl;


	count = 0;
	std::cout << std::endl << std::endl << "wchar (' '):" << std::endl;
	each_slice( wtest.begin(), wtest.end(), ' ', [ &count ]( std::wstring& tag )
	{
		std::wcout << tag << std::endl;
		++count;
	});
	std::cout << "total tags: " << count << std::endl;

	std::cout << std::endl << "wchar + outputIterator (' '):" << std::endl;
	std::vector< std::wstring > wslices;
	std::back_insert_iterator<std::vector< std::wstring >> woutput( wslices );
	each_slice_push( wtest.begin(), wtest.end(), L' ', woutput );

	for_each( wslices.begin(), wslices.end(), []( std::wstring& tag )
	{
		std::wcout << tag << std::endl;
	} );
	std::cout << "total tags: " << wslices.size() << std::endl;
#pragma endregion


#pragma region multi chars slices

	count = 0;
	std::cout << std::endl << "multi char 1 (cpp):" << std::endl;
	each_slice( test.begin(), test.end(), "cpp", [ &count ]( std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	} );
	std::cout << "total tags: " << count << std::endl;


	count = 0;
	std::cout << std::endl << "multi char 2 (source):" << std::endl;
	each_slice( test.begin(), test.end(), "source", [ &count ]( std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	} );
	std::cout << "total tags: " << count << std::endl;


	count = 0;
	std::cout << std::endl << "multi char as one (' '):" << std::endl;
	each_slice( test.begin(), test.end(), " ", [ &count ]( std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	} );
	std::cout << "total tags: " << count << std::endl;

	std::cout << std::endl << "multi char push (source):" << std::endl;
	std::vector< std::string > mslices;
	std::back_insert_iterator<std::vector< std::string >> moutput( mslices );
	each_slice_push( test.begin(), test.end(), "source", moutput );

	for_each( mslices.begin(), mslices.end(), []( std::string& tag )
	{
		std::cout << tag << std::endl;
	} );
	std::cout << "total tags: " << mslices.size() << std::endl;

#pragma endregion


	count = 0;
	std::cout << std::endl << "any slice (c or _):" << std::endl;
	any_slice( test.begin(), test.end(), "c_", [ &count ]( std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	} );
	std::cout << "total tags: " << count << std::endl;


	return 0;
}

