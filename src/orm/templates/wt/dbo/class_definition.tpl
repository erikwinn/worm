{{=<% %>=}}<%! The marker to the left resets the limiters - alter to taste %>

<%#INCLUDES%><%INCLUDE%>
<%/INCLUDES%>
<%CLASS_NAME%>::<%CLASS_NAME%>()
{
    <%#BELONGS_TO%>ptr<%REFERING_VARIABLE_NAME%> = 0;
    ptrTo<%REFERING_VARIABLE_NAME%>IsMine = false;<%#BELONGS_TO_separator%>
    <%/BELONGS_TO_separator%><%/BELONGS_TO%>
}

<%CLASS_NAME%>::~<%CLASS_NAME%>()
{
    <%#BELONGS_TO%>if(ptrTo<%REFERING_VARIABLE_NAME%>IsMine && ptr<%REFERING_VARIABLE_NAME%>)
        delete ptr<%REFERING_VARIABLE_NAME%>;<%#BELONGS_TO_separator%>
    <%/BELONGS_TO_separator%><%/BELONGS_TO%>
}

<%CLASS_NAME%>::<%CLASS_NAME%> ( const <%CLASS_NAME%> &other )
{
    <%#COLUMNS%><%VARIABLE_NAME%>_ = other.<%VARIABLE_NAME%>_;<%#COLUMNS_separator%>
    <%/COLUMNS_separator%><%/COLUMNS%>
}

<%CLASS_NAME%> &<%CLASS_NAME%>::operator= ( const <%CLASS_NAME%> &other )
{
    <%#COLUMNS%><%VARIABLE_NAME%>_ = other.<%VARIABLE_NAME%>_;<%#COLUMNS_separator%>
    <%/COLUMNS_separator%><%/COLUMNS%>
    return *this;
}

bool <%CLASS_NAME%>::operator== ( const <%CLASS_NAME%> &other ) const
{
    return (
            <%#COLUMNS%><%VARIABLE_NAME%>_ == other.<%VARIABLE_NAME%>_ <%#COLUMNS_separator%>&&
            <%/COLUMNS_separator%><%/COLUMNS%>
           );
}

<%#BELONGS_TO%>/**
* NOTE that the gettor returns a pointer that it _owns_ (and will be deleted in dtor) -
* IF AND ONLY IF it HAS NOT BEEN SET WITH THE SETTOR. If you have NOT USED the settor
* the object dtor will do trash collection for ptr<%REFERING_VARIABLE_NAME%>. Setting this
* with set<%REFERING_VARIABLE_NAME%> will trigger trash collection on ptr<%REFERING_VARIABLE_NAME%>
* if we already own one and replace it - and be a bad parent by not taking ownership of the passed pointer.
* In any case it returns a valid pointer to a valid object; either one we loaded and own (lazy loading) or one
* that was manually set which we do not own.
* \see set<%REFERING_VARIABLE_NAME%>.
*/
Dbo::ptr<<%REFERENCED_CLASSNAME%>> <%CLASS_NAME%>::get<%REFERING_VARIABLE_NAME%>()
{
    if(!ptr<%REFERING_VARIABLE_NAME%>)
    {
        ptr<%REFERING_VARIABLE_NAME%> = <%REFERENCED_CLASSNAME%>::LoadById(<%REFERENCED_VARIABLE_NAME%>);
        ptrTo<%REFERING_VARIABLE_NAME%>IsMine = true;
    }
    return ptr<%REFERING_VARIABLE_NAME%>;
}

/**
* NOTE that the settor DELETES any pointer that has been under the ownership of
* the object and sets ptrTo<%REFERING_VARIABLE_NAME%>IsMine to  be false. This means
* that any ptr<%REFERING_VARIABLE_NAME%> SET WITH THE SETTOR WILL NOT BE DELETED.
*
* IE., If you HAVE USED the settor the object dtor WILL NOT do trash collection for
* ptr<%REFERING_VARIABLE_NAME%>.
*/
void <%CLASS_NAME%>::set<%REFERING_VARIABLE_NAME%>(<%REFERENCED_CLASSNAME%> *p)
{
    if(ptrTo<%REFERING_VARIABLE_NAME%>IsMine && ptr<%REFERING_VARIABLE_NAME%>)
        delete ptr<%REFERING_VARIABLE_NAME%>;
    ptr<%REFERING_VARIABLE_NAME%> = p;
    ptrTo<%REFERING_VARIABLE_NAME%>IsMine = false;
}

Dbo::collection<Dbo::ptr<<%CLASS_NAME%>>> <%CLASS_NAME%>::LoadBy<%REFERING_VARIABLE_NAME%>(std::string obj_id)
{
    Dbo::collection<Dbo::ptr<<%CLASS_NAME%>>> listToReturn;
	listToReturn = session_.find<<%CLASS_NAME%>>().where("<%REFERING_COLUMN_NAME%> = ?").bind(obj_id);
    return listToReturn;
}
<%/BELONGS_TO%>

bool <%CLASS_NAME%>::Save()
{
	session_.add(this);
    return true;
}
<%#PK_SECTION%>
bool <%CLASS_NAME%>::Update()
{
	session_.update(this);
    return true;
}

bool <%CLASS_NAME%>::Delete()
{
	session_.remove(this);
    return true;
}

Dbo::ptr<<%CLASS_NAME%>> <%CLASS_NAME%>::LoadById(std::string obj_id)
{
	Dbo::ptr<<%CLASS_NAME%>> ptrToReturn;
	ptrToReturn = session_.find<<%CLASS_NAME%>>().where("<%PK_COLUMN_NAME%> = ?").bind(obj_id);
	return ptrToReturn;
}

<%/PK_SECTION%>

Dbo::collection<Dbo::ptr<<%CLASS_NAME%>>> <%CLASS_NAME%>::LoadAll()
{
    Dbo::collection<Dbo::ptr<<%CLASS_NAME%>>> listToReturn;
    listToReturn = session_.find<<%CLASS_NAME%>>();
    return listToReturn;
}
