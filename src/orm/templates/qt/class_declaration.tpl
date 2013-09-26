{{=<% %>=}}
<%! The line above resets the limiters - alter to taste %>
<%#INCLUDES%><%INCLUDE%>
<%/INCLUDES%>
<%#FORWARD_DECLARATIONS%>class <%REFERENCED_CLASSNAME%>;
<%/FORWARD_DECLARATIONS%>
class <%CLASS_NAME%> {

    public:  <%! UNSUPPORTED refers to enum and set which we don't handle yet - they must be set manually %>
        <%CLASS_NAME%>();
        virtual ~<%CLASS_NAME%>();
        virtual <%CLASS_NAME%>& operator=( const <%CLASS_NAME%>& other );
        virtual bool operator==( const <%CLASS_NAME%>& other ) const;
        inline bool operator!=(const <%CLASS_NAME%> &other) const { return !operator==(other); }
        
        bool Save();
        bool Update();
        bool Delete();
        static <%CLASS_NAME%>* LoadById(QString obj_id);
        static QList<<%CLASS_NAME%>> LoadAll();
        
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%><%DATATYPE%> <%VARIABLE_GETTOR%>()const{return <%VARIABLE_NAME%>;}
        <%/COLUMNS%>
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%>void <%VARIABLE_SETTOR%>(<%DATATYPE%> val){<%VARIABLE_NAME%>=val;}
        <%/COLUMNS%>
        <%#BELONGS_TO%><%REFERENCED_CLASSNAME%>* get<%REFERENCED_CLASSNAME%>(){
            if(!ptr<%REFERENCED_CLASSNAME%>)
				ptr<%REFERENCED_CLASSNAME%> = <%REFERENCED_CLASSNAME%>::LoadById(<%REFERENCED_VARIABLE_NAME%>);
            return ptr<%REFERENCED_CLASSNAME%>;
        }
        <%/BELONGS_TO%>
        <%#HAS_MANY%>const std::vector<<%FOREIGNKEY_CLASSNAME%>*> <%FOREIGNKEY_CLASS_PLURAL%>()const{return vec<%FOREIGNKEY_CLASS_PLURAL%>;}
        <%/HAS_MANY%>
        <%#BELONGS_TO%>void set<%REFERENCED_CLASSNAME%>(<%REFERENCED_CLASSNAME%> *p){ptr<%REFERENCED_CLASSNAME%>=p;}
        <%/BELONGS_TO%>
        <%#HAS_MANY%>void set<%FOREIGNKEY_CLASS_PLURAL%>(std::vector<<%FOREIGNKEY_CLASSNAME%>*> v){vec<%FOREIGNKEY_CLASS_PLURAL%>=v;}
        <%/HAS_MANY%>
    private:
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%><%DATATYPE%> <%VARIABLE_NAME%>;
        <%/COLUMNS%>
        <%#BELONGS_TO%><%REFERENCED_CLASSNAME%> *ptr<%REFERENCED_CLASSNAME%>;
        <%/BELONGS_TO%>
        <%#HAS_MANY%>std::vector<<%FOREIGNKEY_CLASSNAME%>*> vec<%FOREIGNKEY_CLASS_PLURAL%>;
        <%/HAS_MANY%>
};
