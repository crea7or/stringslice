#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>

template < typename _inputIterator, typename _unaryFuntion >
void each_tag( _inputIterator _from, _inputIterator _to, typename _inputIterator::value_type _delimiter, _unaryFuntion _function )
{
	std::basic_string<typename _inputIterator::value_type> tag;
	_inputIterator _cur = _from;
	for ( ; _cur != _to; ++_cur )
	{
		if ( *_cur != _delimiter )
		{
			tag.push_back( *_cur );
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
}

int main(int argc, char* argv[])
{
	std::string test( " 498924782h7 nuncinsiunc 77826376276572  736736473467 " );
	std::wstring wtest( L" 498924782h7 nuncinsiunc 77826376276572  736736473467 " );

	each_tag( test.begin(), test.end(), ' ', []( std::string& tag )
	{
		std::cout << tag << std::endl;
	});

	each_tag( wtest.begin(), wtest.end(), ' ', []( std::wstring& tag )
	{
		std::wcout << tag << std::endl;
	} );

// 	std::vector<int> vtest{ 0, 2, 4, 3, 4, 5, 6, 7, 8, 9, 0, 4, 6, 33, 4, 5, 66, 7, 7, 8, 9, 2, 1, 1, 2, 2, 3, 4, 5, 79, 3, 3, 2, 7, 8, 9, 9 };
// 	each_tag( vtest.begin(), vtest.end(), 9 , []( std::string& tag )
// 	{
// 		std::cout << tag << std::endl;
// 	} );

	return 0;
}

