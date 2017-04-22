#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

template < typename _containerType >
struct back_insert_functor
{
	back_insert_functor( _containerType& _conttype ) : _cont{ _conttype } {};
	void operator()( typename _containerType::value_type _item ) const
	{
		_cont.push_back( _item );
	}

private:

	_containerType& _cont;

};

template < typename _inputIterator, typename _unaryFunction >
void each_slice( _inputIterator _begin, _inputIterator _end, typename _inputIterator::value_type _delimiter, const _unaryFunction& _function )
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
void each_slice( _inputIterator _begin, _inputIterator _end, typename _inputIterator::value_type* _delimiter, const _unaryFunction& _function )
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
void any_slice( _inputIterator _begin, _inputIterator _end, typename _inputIterator::value_type* _delimiter, const _unaryFunction& _function )
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
	each_slice( test.begin(), test.end(), ' ', [&count]( const std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	});
	std::cout << "total tags: " << count << std::endl;


	std::cout << std::endl << "char + outputIterator (' '):" << std::endl;
	std::vector< std::string > slices;
	each_slice( test.begin(), test.end(), ' ', back_insert_functor<std::vector< std::string >>( slices ) );

	for_each( slices.begin(), slices.end(), []( const std::string& tag )
	{
		std::cout << tag << std::endl;
	} );
	std::cout << "total tags: " << slices.size() << std::endl;


	count = 0;
	std::cout << std::endl << std::endl << "wchar (' '):" << std::endl;
	each_slice( wtest.begin(), wtest.end(), ' ', [ &count ]( const std::wstring& tag )
	{
		std::wcout << tag << std::endl;
		++count;
	});
	std::cout << "total tags: " << count << std::endl;

	std::cout << std::endl << "wchar + outputIterator (' '):" << std::endl;
	std::vector< std::wstring > wslices;
	each_slice( wtest.begin(), wtest.end(), L' ', back_insert_functor<std::vector< std::wstring >>( wslices ) );

	for_each( wslices.begin(), wslices.end(), []( const std::wstring& tag )
	{
		std::wcout << tag << std::endl;
	} );
	std::cout << "total tags: " << wslices.size() << std::endl;
#pragma endregion


#pragma region multi chars slices

	count = 0;
	std::cout << std::endl << "multi char 1 (cpp):" << std::endl;
	each_slice( test.begin(), test.end(), "cpp", [ &count ]( const std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	} );
	std::cout << "total tags: " << count << std::endl;


	count = 0;
	std::cout << std::endl << "multi char 2 (source):" << std::endl;
	each_slice( test.begin(), test.end(), "source", [ &count ]( const std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	} );
	std::cout << "total tags: " << count << std::endl;


	count = 0;
	std::cout << std::endl << "multi char as one (' '):" << std::endl;
	each_slice( test.begin(), test.end(), " ", [ &count ]( const std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	} );
	std::cout << "total tags: " << count << std::endl;

	std::cout << std::endl << "multi char push (source):" << std::endl;
	std::vector< std::string > mslices;
	each_slice( test.begin(), test.end(), "source", back_insert_functor<std::vector< std::string >>( mslices ) );

	for_each( mslices.begin(), mslices.end(), []( std::string& tag )
	{
		std::cout << tag << std::endl;
	} );
	std::cout << "total tags: " << mslices.size() << std::endl;

#pragma endregion

#pragma region any_slice test

	count = 0;
	std::cout << std::endl << "any slice (c or _):" << std::endl;
	any_slice( test.begin(), test.end(), "c_", [ &count ]( const std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	} );
	std::cout << "total tags: " << count << std::endl;


	std::cout << std::endl << "any slice push (c or _):" << std::endl;
	std::vector< std::string > maslices;
	any_slice( test.begin(), test.end(), "c_", back_insert_functor<std::vector< std::string >>( maslices ) );

	for_each( maslices.begin(), maslices.end(), []( std::string& tag )
	{
		std::cout << tag << std::endl;
	} );
	std::cout << "total tags: " << maslices.size() << std::endl;


#pragma endregion

	return 0;
}

