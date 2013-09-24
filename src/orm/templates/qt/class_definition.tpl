{{=<% %>=}} 
<%! The line above resets the limiters - alter to taste %>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>

<%#INCLUDES%><%INCLUDE%>
<%/INCLUDES%>
#include "utilities.h"

<%CLASS_NAME%>::<%CLASS_NAME%>()
{}

<%CLASS_NAME%>::~<%CLASS_NAME%>()
{}

<%CLASS_NAME%>::<%CLASS_NAME%> ( const <%CLASS_NAME%> &other )
{
    <%#COLUMNS%><%VARIABLE_NAME%> = other.<%VARIABLE_NAME%>;
    <%/COLUMNS%>
}

<%CLASS_NAME%>::~<%CLASS_NAME%>()
{}
/*!
 *    Sets this <%CLASS_NAME%> equal to \a other.
 */
<%CLASS_NAME%> &<%CLASS_NAME%>::operator= ( const <%CLASS_NAME%> &other )
{
    <%#COLUMNS%><%VARIABLE_NAME%> = other.<%VARIABLE_NAME%>;
    <%/COLUMNS%>
	return *this;
}

/*! \fn bool <%CLASS_NAME%>::operator!=(const <%CLASS_NAME%> &other) const
 *    Returns true if this <%CLASS_NAME%> is not identical to \a other; otherwise returns
 *    false.
 */

/*!
 *    Returns true if this <%CLASS_NAME%> is identical to \a other; otherwise returns
 *    false.
 */
bool <%CLASS_NAME%>::operator== ( const <%CLASS_NAME%> &other ) const
{
	return (
			<%#COLUMNS%><%VARIABLE_NAME%> == other.<%VARIABLE_NAME%> <%#COLUMNS_separator%>&&<%/COLUMNS_separator%>
			<%/COLUMNS%>);
}


bool <%CLASS_NAME%>::Save()
{
    if(!Utilities::DbIsConnected())
        if(!Utilities::CreateDbConnection())
             return false;

    QSqlQuery query;
    query.prepare("INSERT into <%TABLE_NAME%> ( "
			<%#COLUMNS%>"<%COLUMN_NAME%><%#COLUMNS_separator%>,<%/COLUMNS_separator%> "				  
			<%/COLUMNS%>") VALUES ( "
			<%#COLUMNS%>":<%COLUMN_NAME%><%#COLUMNS_separator%>,<%/COLUMNS_separator%> "
			<%/COLUMNS%>")" );
    <%#COLUMNS%>query.bindValue("<%COLUMN_NAME%>", <%VARIABLE_GETTOR%>());
	<%/COLUMNS%>
    if(! query.exec())
    {
        qDebug(QString("Warning: unable to save <%CLASS_NAME%>\n  Error: %1").arg(query.lastError().text()).toUtf8());
        return false;
    }
    return true;
}
<%#PK_SECTION%>
bool <%CLASS_NAME%>::Update()
{
    if(!Utilities::DbIsConnected())
        if(!Utilities::CreateDbConnection())
            return false;
    QSqlQuery query;

    query.prepare(QString("UPDATE <%TABLE_NAME%> SET "
                          <%#COLUMNS%>"<%COLUMN_NAME%>='%<%COLUMN_COUNT%>'<%#COLUMNS_separator%>,<%/COLUMNS_separator%> "
						  <%/COLUMNS%>" WHERE id = '%<%COLUMN_COUNT%>' "
						 )
                  <%#COLUMNS%>.arg(QString(<%VARIABLE_GETTOR%>()).replace('\'',"''"))
				  <%/COLUMNS%>.arg(<%PRIMARY_KEY%>);

    if(! query.exec())
    {
        qDebug(QString("Warning: unable to update <%CLASS_NAME%> with ID: %1.\n  Error: %2 \n Query: %3")
               .arg(<%PRIMARY_KEY%>)
               .arg(query.lastQuery())
               .arg(query.lastError().text()).toUtf8());
        return false;
    }
    return true;
}

bool <%CLASS_NAME%>::Delete()
{
    if(!Utilities::DbIsConnected())
        if(!Utilities::CreateDbConnection())
            return false;
    QSqlQuery query;
    query.prepare(QString("DELETE * from <%TABLE_NAME%> WHERE id = '%1' ").arg(<%PRIMARY_KEY%>));

    if(! query.exec())
    {
        qDebug(QString("Warning: unable to delete <%CLASS_NAME%>\n  Error: %1").arg(query.lastError().text()).toUtf8());
        return false;
    }
    return true;
}
<%/PK_SECTION%>

bool <%CLASS_NAME%>::LoadById(QString obj_id)
{
    if(!Utilities::DbIsConnected())
        if(!Utilities::CreateDbConnection())
            return false;
    QSqlQuery query;
    query.prepare(QString("SELECT * from <%TABLE_NAME%> WHERE id = '%1' ").arg(obj_id));

    if(! query.exec() || !query.next())
    {
        qDebug(QString("Warning: unable to load job detail with ID: %1.\n  Error: %2").arg(obj_id).arg(query.lastError().text()).toUtf8());
        return false;
    }
   
    <%#COLUMNS%><%VARIABLE_SETTOR%>(query.value(0).toString());
	<%/COLUMNS%>
    return true;
}

QList<<%CLASS_NAME%>> <%CLASS_NAME%>::LoadAll()
{
    if(!Utilities::DbIsConnected())
        if(!Utilities::CreateDbConnection())
            return false;
    QList<<%CLASS_NAME%>> listToReturn;
    QSqlQuery query;
    query.prepare("SELECT * from <%TABLE_NAME%>");

    if(! query.exec())
        qDebug(QString("Warning: unable to load <%CLASS_NAME%> list.\n  Error: %2").arg(query.lastError().text()).toUtf8());

    while(query.next())
    {
        <%CLASS_NAME%> obj;
		<%#COLUMNS%>obj.<%VARIABLE_SETTOR%>(query.value(0).toString());
		<%/COLUMNS%>
        listToReturn.push_back(obj);
    }
    return listToReturn;
}
