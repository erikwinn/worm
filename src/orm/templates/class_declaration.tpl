{{=<% %>=}} 
<%! The line above resets the limiters - alter to taste %>
#include <Wt/Dbo/Dbo>
<%#INCLUDES%><%INCLUDE%>
<%/INCLUDES%>
namespace dbo = Wt::Dbo;

class <%CLASS_NAME%> {

    public:  <%! UNSUPPORTED refers to enum and set which we don't handle yet - they must be set manually %>
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%> <%DATATYPE%> <%VARIABLE_GETTOR%>()const(return _<%VARIABLE_NAME%>;}
        <%/COLUMNS%>
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%>void <%VARIABLE_SETTOR%>(<%DATATYPE%> var){_<%VARIABLE_NAME%>=var;}
        <%/COLUMNS%>
    
    template<class Action> void persist(Action& a)
    {
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%>dbo::field(a, _<%VARIABLE_NAME%>,"<%COLUMN_NAME%>"); 
        <%/COLUMNS%>
    }
    
    private:
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%> <%DATATYPE%> _<%VARIABLE_NAME%>;
        <%/COLUMNS%>
};
