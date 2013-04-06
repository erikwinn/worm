/*
    WORM - a DAL/ORM code generation framework
    Copyright (C) 2011  Erik Winn <sidewalksoftware@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wmysqlresult.h"
#include "wmysqldriver.h"
namespace WSql
{

/*!\class WMysqlResult - a result set object for MySQL
    * \brief WMysqlResult is the type of result set used by the MySQL driver
    * 
    * \note Results from mysql are _always_ cached at the moment so this class is mostly
    * a stub - no methods have been implemented.
    * 
    * \todo uncached results if possible.
    * 
    *    \ingroup WSql
    *    \sa WSqlResult
    */    
WMysqlResult::WMysqlResult( const WSql::WSqlDriver* driver ): WSqlResult( driver )
{
    _mysqlResult=0;
}
    
// WMysqlResult::WMysqlResult( const WMysqlResult& other )
// {
// 
// }

WMysqlResult::~WMysqlResult()
{
    freeResult();
}

// WMysqlResult& WMysqlResult::operator=( const WMysqlResult & other )
// {
//     ///!\todo 
//     return *this;
// }
// 
// bool WMysqlResult::operator==( const WMysqlResult& other ) const
// {
// ///!\todo return ...;
// }

void WMysqlResult::freeResult()
{
    mysql_free_result(_mysqlResult);
}

WSqlRecord WMysqlResult::current()
{
    if(_isCached)
        return WSql::WSqlResult::current();
}

WSqlRecord WMysqlResult::fetch( int i )
{
    if(_isCached)
        return WSql::WSqlResult::fetch( i );
}

WSqlRecord WMysqlResult::fetchFirst()
{
    if(_isCached)
        return WSql::WSqlResult::fetchFirst();
}

WSqlRecord WMysqlResult::fetchLast()
{
    if(_isCached)
        return WSql::WSqlResult::fetchLast();
}

WSqlRecord WMysqlResult::fetchNext()
{
    if(_isCached)
        return WSql::WSqlResult::fetchNext();
}

WSqlRecord WMysqlResult::fetchPrevious()
{
    if(_isCached)
        return WSql::WSqlResult::fetchPrevious();
}

bool WMysqlResult::first()
{
    if(_isCached)
        return WSql::WSqlResult::first();
}

bool WMysqlResult::last()
{
    if(_isCached)
        return WSql::WSqlResult::last();
}

bool WMysqlResult::next()
{
    if(_isCached)
        return WSql::WSqlResult::next();
}

bool WMysqlResult::previous()
{
    if(_isCached)
        return WSql::WSqlResult::previous();
}

bool WMysqlResult::seek( int i, bool relative )
{
    if(_isCached)
        return WSql::WSqlResult::seek( i, relative );
}

} //namespace WSql
