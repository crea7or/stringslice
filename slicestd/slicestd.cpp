#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <iterator>

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

template < typename _inputIterator, typename _outputIterator, typename _delimiterType, typename _valueType >
void each_tag_push( _inputIterator _from, _inputIterator _to, _delimiterType _delimiter, _outputIterator _output )
{
//	std::static_assert( std::is_same( typename _inputIterator::value_type, _delimiterType ));
	_inputIterator _cur = _from;
	for ( ; _from != _to; ++_from )
	{
		if ( *_from == _delimiter )
		{
			if ( _cur != _from )
			{
				_output = _valueType( _cur, _from );
				_cur = _from;
			}
		}
	}
	if ( _cur != _from )
	{
		_output = _valueType( _cur, _from );
		_cur = _from;
	}
}

int main(int argc, char* argv[])
{
	std::string test( " 498924782h7 nuncinsiunc 77826376276572  736736473467 sdfsdfw344224 d" );
	std::wstring wtest( L" 498924782h7 nuncinsiunc 77826376276572  736736473467 fefsdf22234 " );

	std::cout << "char:" << std::endl;
	each_tag( test.begin(), test.end(), ' ', []( std::string& tag )
	{
		std::cout << tag << std::endl;
	});

	std::cout << std::endl << "wchar:" << std::endl;
	each_tag( wtest.begin(), wtest.end(), L' ', []( std::wstring& tag )
	{
		std::wcout << tag << std::endl;
	} );

	/*
	std::vector< std::string > slices;
	std::back_insert_iterator<std::vector< std::string >>  output( slices );
	each_tag_push( test.begin(), test.end(), ' ', output );
	*/

	return 0;
}

