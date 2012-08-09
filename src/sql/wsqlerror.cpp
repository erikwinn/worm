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

#include "wsqlerror.h"

namespace WSql {

/*!
 *    \class WSqlError
 *    \brief The WSqlError class provides SQL database and driver error information.
 * 
 * WSqlError provides an error object with type and severity flags as well as convenience
 * methods for setting the error message or number.
 * 
 *   \ingroup WSql
 * 
*/

/*!
    \enum WSqlError::ErrorSeverity

    This is a flag indicating of what severity the error is, ie. how bad was it ..

    \li \c NONE  No error occurred.
    \li \c MESSAGE  Just a friendly message ..
    \li \c WARNING Advice that something is not right ..
    \li \c DANGER  Strong advice that something is very wrong
    \li \c FATAL  - self explanatory, action not completed.
*/

/*!
    \enum WSqlError::ErrorType

    This is a flag indicating what type of error occured

    \li \c NONE  No error occurred.
    \li \c DRIVER  An error in the driver itself.
    \li \c SERVER An error from the database
    \li \c QUERY  SQL statement syntax error
    \li \c SYSTEM An error from the system, eg. an exception was thrown
    \li \c UNKNOWN  Unable to determine.
*/

/*!
    \brief Constructs an empty WSqlError instance 
    \note The type and severity are set to UNKNOWN and NONE  and 
    error_number is -1 by default. 
    Use setType(), setSeverity() and setErrorNumber() to change this or a 
    constructor with the full set of arguments.
*/
WSqlError::WSqlError()
{
	_error_number = -1;
	_text = std::string();
	_type = UNKNOWN;
	_severity = NONE;
}

/*!
    \brief Constructs an error containing the text \a text of ErrorType \a type
    \note The severity is set to MESSAGE  and  error_number to -1 by default. 
    Use setSeverity() and setErrorNumber() to change this or a 
    constructor with the full set of arguments.
    
    \param string the message
    \param ErrorType type - the type flag
    \sa setSeverity() setErrorNumber()
*/
WSqlError::WSqlError(const std::string& text, WSqlError::ErrorType type)
{
	_text = text;
	_type = type;
	_severity = MESSAGE;
	_error_number = -1;
}
/*!
    \brief Constructs a fully populated error object 
    
    Constructs an error containing the text \a text of ErrorType \a type and ErrorSeverity
    \a severity with the error number error_number (usually from the database server ..).
    
    \param string the message
    \param int error_number - the error number
    \param ErrorType type - the type flag
    \param ErrorSeverity severity - the severity flag
*/
WSqlError::WSqlError(const std::string& text, int error_number, WSqlError::ErrorType type, WSqlError::ErrorSeverity severity)
{
	_text=text;
	_error_number=error_number;
	_type=type;
	_severity=severity;
}
/*!
    \brief Creates a copy of \a other.
*/
WSqlError::WSqlError(const WSqlError& other)
{
	_error_number = other._error_number;
	_text = other._text;
	_severity = other._severity;
	_type = other._type;
}
/*! \brief Assigns the \a other error's values to this error.
*/
WSqlError& WSqlError::operator=(const WSqlError& other)
{
	_error_number = other._error_number;
	_text = other._text;
	_severity = other._severity;
	_type = other._type;
    return *this;
}
/*! \brief Comparison operator - returns true if this WSqlError is identical to \a other.
*/
bool WSqlError::operator==(const WSqlError& other)const
{
	return _text.compare(other._text) == 0
				&& _error_number == other._error_number
				&& _type == other._type
				&& _severity == other._severity;
}

/*! \brief Destroys the object and frees any allocated resources.
*/
WSqlError::~WSqlError(){}

/*! \fn int WSqlError::errorNumber() const
 *    \brief Returns the error number

    This function returns an error number if possible - generally from a database driver
    or the system. If no error number was set it retuns -1.
    
    \retval int the error number
 */
/*!  \fn std::string WSqlError::text() const
     \brief Returns the text of the error message
 
    This function returns a single string of text containing the error message. This may be concatenated
    messages (eg. from a driver) or a single message or an empty string (eg. if only the
    errorNumber was set).
    
    \retval std::string  the text of the error message
 */
/*! \fn ErrorSeverity WSqlError::severity() const
 *    \brief Returns the severity of the error
 * 
 * This function returns a flag indicating the severity of the error.
 * 
 * \sa ErrorSeverity ErrorType
 *    \retval ErrorSeverity how bad it is ..
 */

/*! \fn ErrorType WSqlError::type()const
 *   \brief Returns the type of error
 * 
 * This function returns a flag indicating what type of error this is - eg. DRIVER, SYSTEM, etc..
 * 
 * \sa ErrorType ErrorSeverity
 * \retval ErrorType the type of this error
 */
/*!
 *    \fn WSqlError::setErrorNumber(const int error_number)
 *    \brief Sets the errorNumber to be \a error_number
 */
/*!
 *    \fn WSqlError::setText(const std::string& message )
 *    \brief Sets the text of the error to \a message
 */
/*!
 *    \fn WSqlError::setSeverity(ErrorSeverity severity)
 *    \brief Sets the severity flag to be \a severity
 */
/*!
 *    \fn WSqlError::setType(ErrorType type)
 *    \brief Sets the type flag to be \a type
 */

} //namespace WSql
