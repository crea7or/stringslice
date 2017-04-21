#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

template < typename _inputIterator, typename _unaryFuntion >
void each_tag( _inputIterator _from, _inputIterator _to, typename _inputIterator::value_type _delimiter, _unaryFuntion _function )
{
	std::basic_string<typename _inputIterator::value_type> tag;
	for ( ; _from != _to; ++_from )
	{
		if ( *_from != _delimiter )
		{
			tag.push_back( *_from );
		}
		else
		{
			if ( tag.size() > 0 )
			{
				_function( tag );
			}
			tag.clear();
		}
	}
	if ( tag.size() > 0 )
	{
		_function( tag );
	}
}

template < typename _inputIterator, typename _outputIterator, typename _delimiterType >
void each_tag_push( _inputIterator _from, _inputIterator _to, _delimiterType _delimiter, _outputIterator _output )
{
	//std::static_assert( std::is_same< typename _inputIterator::value_type, _delimiterType >::value );
	_inputIterator _first = _from, _cur = _from;
	for ( ; _from != _to; ++_from )
	{
		if ( *_from == _delimiter )
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

int main(int argc, char* argv[])
{
	std::string test( " 498924782h7 nuncinsiunc 77826376276572  736736473467 sdfsdfw344224 d" );
	std::wstring wtest( L" 498924782h7 nuncinsiunc 77826376276572  736736473467 fefsdf22234 " );

	size_t count = 0;
	std::cout << "char:" << std::endl;
	each_tag( test.begin(), test.end(), ' ', [&count]( std::string& tag )
	{
		std::cout << tag << std::endl;
		++count;
	});
	std::cout << "total tags: " << count << std::endl;


	std::cout << std::endl << "char + outputIterator:" << std::endl;
	std::vector< std::string > slices;
	std::back_insert_iterator<std::vector< std::string >>  output( slices );
	each_tag_push( test.begin(), test.end(), ' ', output ); // <- char to wchar implict convertsion is possible

	for_each( slices.begin(), slices.end(), []( std::string& tag )
	{
		std::cout << tag << std::endl;
	} );
	std::cout << "total tags: " << slices.size() << std::endl;


	size_t wcount = 0;
	std::cout << std::endl << "wchar:" << std::endl;
	each_tag( wtest.begin(), wtest.end(), L' ', [ &wcount ]( std::wstring& tag )
	{
		std::wcout << tag << std::endl;
		++wcount;
	});
	std::cout << "total tags: " << wcount << std::endl;

	std::cout << std::endl << "wchar + outputIterator:" << std::endl;
	std::vector< std::wstring > wslices;
	std::back_insert_iterator<std::vector< std::wstring >> woutput( wslices );
	each_tag_push( wtest.begin(), wtest.end(), L' ', woutput );

	for_each( wslices.begin(), wslices.end(), []( std::wstring& tag )
	{
		std::wcout << tag << std::endl;
	} );
	std::cout << "total tags: " << wslices.size() << std::endl;







	return 0;
}

