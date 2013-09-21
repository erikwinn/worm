{{=<% %>=}} 
<%! The line above resets the limiters - alter to taste %>
<%#INCLUDES%><%INCLUDE%>
<%/INCLUDES%>

<%#FORWARD_DECLARATIONS%>class <%REFERENCED_CLASSNAME%>;
<%/FORWARD_DECLARATIONS%>

class <%CLASS_NAME%> {
        <%CLASS_NAME%>::<%CLASS_NAME%>();
        virtual ~<%CLASS_NAME%>::<%CLASS_NAME%>();
        virtual <%CLASS_NAME%>& operator=( const <%CLASS_NAME%>& other );
        virtual bool operator==( const <%CLASS_NAME%>& other ) const;
        inline bool operator!=(const <%CLASS_NAME%> &other) const { return !operator==(other); }
        
        <%#BELONGS_TO%><%REFERENCED_CLASSNAME%> *ptr<%REFERENCED_CLASSNAME%>;
        <%/BELONGS_TO%>
        <%#HAS_MANY%>vector<<%FOREIGNKEY_CLASSNAME%>*> vec<%FOREIGNKEY_CLASS_PLURAL%>;
        <%/HAS_MANY%>
    public:  <%! UNSUPPORTED refers to enum and set which we don't handle yet - they must be set manually %>
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%><%DATATYPE%> <%VARIABLE_GETTOR%>()const{return <%VARIABLE_NAME%>;}
        <%/COLUMNS%>
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%>void <%VARIABLE_SETTOR%>(<%DATATYPE%> val){<%VARIABLE_NAME%>=val;}
        <%/COLUMNS%>       
        <%#BELONGS_TO%>const <%REFERENCED_CLASSNAME%>* <%REFERENCED_CLASSNAME%>()const{return ptr<%REFERENCED_CLASSNAME%>;}
        <%/BELONGS_TO%>
        <%#HAS_MANY%>const vector<<%FOREIGNKEY_CLASSNAME%>*> <%FOREIGNKEY_CLASS_PLURAL%>()const{return vec<%FOREIGNKEY_CLASS_PLURAL%>;}
        <%/HAS_MANY%>
        <%#BELONGS_TO%>void set<%REFERENCED_CLASSNAME%>(<%REFERENCED_CLASSNAME%> *p){ptr<%REFERENCED_CLASSNAME%>=p;}
        <%/BELONGS_TO%>
        <%#HAS_MANY%>void set<%FOREIGNKEY_CLASS_PLURAL%>(vector<<%FOREIGNKEY_CLASSNAME%>*> v){vec<%FOREIGNKEY_CLASS_PLURAL%>=v;}
        <%/HAS_MANY%>
    private:
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%> <%DATATYPE%> <%VARIABLE_NAME%>;
        <%/COLUMNS%>
};
