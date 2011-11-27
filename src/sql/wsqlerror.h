/*
 *    WORM - a DAL/ORM code generation framework
 *    Copyright (C) 2011  Erik Winn <sidewalksoftware@gmail.com>
 * 
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WSQLERROR_H
#define WSQLERROR_H

#include <string>

namespace WSql {

class  WSqlError
{
public:

	enum ErrorSeverity {
		NONE = 0x0,
		MESSAGE = 0x01,
        WARNING = 0x02,
		DANGER = 0x04,
		FATAL = 0x08,
        ALL = 0xff,
	};
    enum ErrorType {
        UNKNOWN = 0,
        DRIVER,
        SERVER,
		QUERY,
		SYSTEM
    };

	WSqlError();
    WSqlError( const std::string& text, ErrorType type = UNKNOWN);
    WSqlError( const std::string& text, 
							int errno = -1,
							ErrorType type = UNKNOWN,
							ErrorSeverity severity = NONE);
	
    WSqlError(const WSqlError& other);
    WSqlError& operator=(const WSqlError& other);
    bool operator==(const WSqlError& other)const;
    bool operator!=(const WSqlError& other)const{return ! operator==(other);}
    ~WSqlError();

    std::string text() const{return _text;}
    int errorNumber() const {return _errno;}
    ErrorType type() const {return _type;}
    ErrorSeverity severity() const {return _severity;}

	void setErrorNumber(int errno){_errno=errno;}
    void setText(const std::string& text){_text=text;}
    void setType(ErrorType type){_type=type;}
    void setSeverity(ErrorSeverity severity){_severity=severity;}

private:
    int _errno;
    std::string _text;
    ErrorType _type;
	ErrorSeverity _severity;
};

} //namespace WSql


#endif // WSQLERROR_H
