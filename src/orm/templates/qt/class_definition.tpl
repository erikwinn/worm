{{=<% %>=}}<%! The marker to the left resets the limiters - alter to taste %>#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>

<%#INCLUDES%><%INCLUDE%>
<%/INCLUDES%>#include "utilities.h"

<%CLASS_NAME%>::<%CLASS_NAME%>()
{
    <%#BELONGS_TO%>ptr<%REFERENCED_CLASSNAME%> = 0;
    ptrTo<%REFERENCED_CLASSNAME%>IsMine = false;<%#BELONGS_TO_separator%>
    <%/BELONGS_TO_separator%><%/BELONGS_TO%>
}

<%CLASS_NAME%>::~<%CLASS_NAME%>()
{
    <%#BELONGS_TO%>if(ptrTo<%REFERENCED_CLASSNAME%>IsMine && ptr<%REFERENCED_CLASSNAME%>)
        delete ptr<%REFERENCED_CLASSNAME%>;<%#BELONGS_TO_separator%>
    <%/BELONGS_TO_separator%><%/BELONGS_TO%>
}

<%CLASS_NAME%>::<%CLASS_NAME%> ( const <%CLASS_NAME%> &other )
{
    <%#COLUMNS%><%VARIABLE_NAME%> = other.<%VARIABLE_NAME%>;<%#COLUMNS_separator%>
    <%/COLUMNS_separator%><%/COLUMNS%>
}

<%CLASS_NAME%> &<%CLASS_NAME%>::operator= ( const <%CLASS_NAME%> &other )
{
    <%#COLUMNS%><%VARIABLE_NAME%> = other.<%VARIABLE_NAME%>;<%#COLUMNS_separator%>
    <%/COLUMNS_separator%><%/COLUMNS%>
    return *this;
}

bool <%CLASS_NAME%>::operator== ( const <%CLASS_NAME%> &other ) const
{
    return (
            <%#COLUMNS%><%VARIABLE_NAME%> == other.<%VARIABLE_NAME%> <%#COLUMNS_separator%>&&
            <%/COLUMNS_separator%><%/COLUMNS%>
           );
}

<%#BELONGS_TO%>/**
* NOTE that the gettor returns a pointer that it _owns_ (and will be deleted in dtor)
* If and Only If it HAS NOT BEEN SET WITH THE SETTOR. If you have NOT USED the settor
* the object dtor will do trash collection for ptr<%REFERENCED_CLASSNAME%>.
*/
<%REFERENCED_CLASSNAME%>* <%CLASS_NAME%>::get<%REFERENCED_CLASSNAME%>()
{
    if(!ptr<%REFERENCED_CLASSNAME%>)
    {
        ptr<%REFERENCED_CLASSNAME%> = <%REFERENCED_CLASSNAME%>::LoadById(<%REFERENCED_VARIABLE_NAME%>);
        ptrTo<%REFERENCED_CLASSNAME%>IsMine = true;
    }
    return ptr<%REFERENCED_CLASSNAME%>;
}

/**
* NOTE that the settor DELETES any pointer that has been under the ownership of
* the object and sets ptrTo<%REFERENCED_CLASSNAME%>IsMine to  be false. This means
* that any ptr<%REFERENCED_CLASSNAME%> SET WITH THE SETTOR WILL NOT BE DELETED.
*
* IE., If you HAVE USED the settor the object dtor WILL NOT do trash collection for
* ptr<%REFERENCED_CLASSNAME%>.
*/
void <%CLASS_NAME%>::set<%REFERENCED_CLASSNAME%>(<%REFERENCED_CLASSNAME%> *p)
{
    if(ptrTo<%REFERENCED_CLASSNAME%>IsMine && ptr<%REFERENCED_CLASSNAME%>)
        delete ptr<%REFERENCED_CLASSNAME%>;
    ptr<%REFERENCED_CLASSNAME%> = p;
    ptrTo<%REFERENCED_CLASSNAME%>IsMine = false;
}

QList<<%CLASS_NAME%>> <%CLASS_NAME%>::LoadBy<%REFERENCED_CLASSNAME%>Id(QString obj_id)
{
    QList<<%CLASS_NAME%>> listToReturn;
    if(!Utilities::DbIsConnected())
        if(!Utilities::CreateDbConnection())
            return listToReturn;

    QSqlQuery query;
    query.prepare(QString("SELECT * from <%TABLE_NAME%> WHERE <%REFERENCED_COLUMN_NAME%> = '%1' ").arg(obj_id));

    if(! query.exec())
        qDebug(QString("Warning: unable to load <%CLASS_NAME%> list.\n  Error: %2").arg(query.lastError().text()).toUtf8());

    while(query.next())
    {
        <%CLASS_NAME%> obj;
        <%#COLUMNS%>obj.<%VARIABLE_SETTOR%>(query.value(<%CURRENT_COLUMN_NUMBER%>).to<%TO_DATATYPE%>());
        <%/COLUMNS%>
        listToReturn.push_back(obj);
    }
    return listToReturn;
}
<%/BELONGS_TO%>

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
                          <%/COLUMNS%>" WHERE <%PK_COLUMN_NAME%> = '%<%COLUMN_COUNT%>' "
                         )
                  <%#COLUMNS%>.arg(QString(<%VARIABLE_GETTOR%>()).replace('\'',"''"))
                  <%/COLUMNS%>.arg(<%PK_VARIABLE_NAME%>));

    if(! query.exec())
    {
        qDebug(QString("Warning: unable to update <%CLASS_NAME%> with ID: %1.\n  Error: %2 \n Query: %3")
               .arg(<%PK_VARIABLE_NAME%>)
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
    query.prepare(QString("DELETE * from <%TABLE_NAME%> WHERE <%PK_COLUMN_NAME%> = '%1' ").arg(<%PK_VARIABLE_NAME%>));

    if(! query.exec())
    {
        qDebug(QString("Warning: unable to delete <%CLASS_NAME%>\n  Error: %1").arg(query.lastError().text()).toUtf8());
        return false;
    }
    return true;
}

<%CLASS_NAME%>* <%CLASS_NAME%>::LoadById(QString obj_id)
{
    <%CLASS_NAME%> *ptrToReturn = 0;

    if(!Utilities::DbIsConnected())
        if(!Utilities::CreateDbConnection())
            return ptrToReturn;
    QSqlQuery query;
    query.prepare(QString("SELECT * from <%TABLE_NAME%> WHERE <%PK_COLUMN_NAME%> = '%1' ").arg(obj_id));

    if(! query.exec() || !query.next())
    {
        qDebug(QString("Warning: unable to load job detail with ID: %1.\n  Error: %2").arg(obj_id).arg(query.lastError().text()).toUtf8());
        return ptrToReturn;
    }

    ptrToReturn = new <%CLASS_NAME%>;
    <%#COLUMNS%>ptrToReturn-><%VARIABLE_SETTOR%>(query.value(<%CURRENT_COLUMN_NUMBER%>).to<%TO_DATATYPE%>());
    <%/COLUMNS%>
    return ptrToReturn;
}

<%/PK_SECTION%>

QList<<%CLASS_NAME%>> <%CLASS_NAME%>::LoadAll()
{
    QList<<%CLASS_NAME%>> listToReturn;
    if(!Utilities::DbIsConnected())
        if(!Utilities::CreateDbConnection())
            return listToReturn;

    QSqlQuery query;
    query.prepare("SELECT * from <%TABLE_NAME%>");

    if(! query.exec())
        qDebug(QString("Warning: unable to load <%CLASS_NAME%> list.\n  Error: %2").arg(query.lastError().text()).toUtf8());

    while(query.next())
    {
        <%CLASS_NAME%> obj;
        <%#COLUMNS%>obj.<%VARIABLE_SETTOR%>(query.value(<%CURRENT_COLUMN_NUMBER%>).to<%TO_DATATYPE%>());
        <%/COLUMNS%>
        listToReturn.push_back(obj);
    }
    return listToReturn;
}
