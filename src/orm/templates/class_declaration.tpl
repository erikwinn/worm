{{=<% %>=}} 
<%! The line above resets the limiters - alter to taste %>
#include <Wt/Dbo/Dbo>
<%#INCLUDES%><%INCLUDE%>
<%/INCLUDES%>
namespace dbo = Wt::Dbo;

<%#FORWARD_DECLARATIONS%>class <%REFERENCED_CLASSNAME%>;
<%/FORWARD_DECLARATIONS%>
class <%CLASS_NAME%> {
        
        <%#BELONGS_TO%>dbo::ptr<<%REFERENCED_CLASSNAME%>> ptr<%REFERENCED_CLASSNAME%>;
        <%/BELONGS_TO%>
        <%#HAS_MANY%>dbo::collection< dbo::ptr<<%FOREIGNKEY_CLASSNAME%>> > ptr<%FOREIGNKEY_CLASS_PLURAL%>;
        <%/HAS_MANY%>
    public:  <%! UNSUPPORTED refers to enum and set which we don't handle yet - they must be set manually %>
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%> <%DATATYPE%> <%VARIABLE_GETTOR%>()const(return _<%VARIABLE_NAME%>;}
        <%/COLUMNS%>
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%>void <%VARIABLE_SETTOR%>(<%DATATYPE%> val){_<%VARIABLE_NAME%>=val;}
        <%/COLUMNS%>
    
    template<class Action> void persist(Action& a)
    {
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%>dbo::field( a, _<%VARIABLE_NAME%>, "<%COLUMN_NAME%>"); 
        <%/COLUMNS%>
        <%#BELONGS_TO%>dbo::belongsTo(a, ptr<%REFERENCED_CLASSNAME%>, "<%REFERENCED_TABLENAME%>");
        <%/BELONGS_TO%>
        <%#HAS_MANY%>dbo::hasMany(a, ptr<%FOREIGNKEY_CLASS_PLURAL%>, dbo::ManyToOne, "<%REFERENCED_TABLENAME%>");
        <%/HAS_MANY%>
    }
    
    private:
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%> <%DATATYPE%> _<%VARIABLE_NAME%>;
        <%/COLUMNS%>
};
