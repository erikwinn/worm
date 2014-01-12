{{=<% %>=}}<%! The marker to the left resets the limiters - alter to taste %>#ifndef <%HEADER_GUARD%>
#define <%HEADER_GUARD%>
#include <Wt/Dbo/Dbo>
<%#INCLUDES%><%INCLUDE%>
<%/INCLUDES%>
using namespace Wt;

<%#FORWARD_DECLARATIONS%>class <%REFERENCED_CLASSNAME%>;
<%/FORWARD_DECLARATIONS%>
class <%CLASS_NAME%> {
        Dbo::Session session_;
        <%#BELONGS_TO%>Dbo::ptr<<%REFERENCED_CLASSNAME%>> ptr<%REFERING_VARIABLE_NAME%>;
        bool ptrTo<%REFERING_VARIABLE_NAME%>IsMine;
        <%/BELONGS_TO%>
        <%#HAS_MANY%>Dbo::collection<Dbo::ptr<<%FOREIGNKEY_CLASSNAME%>>> ptr<%FOREIGNKEY_CLASS_PLURAL%>;
        <%/HAS_MANY%>
    public:<%! UNSUPPORTED refers to enum and set which we don't handle yet - they must be set manually %>
        <%CLASS_NAME%>();
        <%CLASS_NAME%>(Dbo::Session s){ setSession(s);};
        <%CLASS_NAME%>( const <%CLASS_NAME%>& other );
        virtual ~<%CLASS_NAME%>();
        virtual <%CLASS_NAME%>& operator=( const <%CLASS_NAME%>& other );
        virtual bool operator==( const <%CLASS_NAME%>& other ) const;
        inline bool operator!=( const <%CLASS_NAME%> &other ) const { return !operator==(other); }

        bool Save();
        <%#PK_SECTION%>bool Update();
        bool Delete();
        static Dbo::ptr<<%CLASS_NAME%>> LoadById(QString obj_id);
        <%/PK_SECTION%>
        static Dbo::collection<Dbo::ptr<<%CLASS_NAME%>>> LoadAll();
		void setSession(Session &s){ session_ = s;}
		const Session& getSession()const{return session_;}  
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%> <%DATATYPE%> <%VARIABLE_GETTOR%>()const{return <%VARIABLE_NAME%>_;}
        <%/COLUMNS%>
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%>void <%VARIABLE_SETTOR%>(<%DATATYPE%> val){<%VARIABLE_NAME%>_=val;}
        <%/COLUMNS%>
        <%#BELONGS_TO%>Dbo::ptr<<%REFERENCED_CLASSNAME%>> get<%REFERING_VARIABLE_NAME%>();
        <%/BELONGS_TO%>
        <%#HAS_MANY%>Dbo::collection<Dbo::ptr<<%FOREIGNKEY_CLASSNAME%>>> <%FOREIGNKEY_CLASS_PLURAL%>(){return ptr<%FOREIGNKEY_CLASS_PLURAL%>;}
        <%/HAS_MANY%>
        <%#BELONGS_TO%>void set<%REFERING_VARIABLE_NAME%>(Dbo::ptr<<%REFERENCED_CLASSNAME%>> p);
        <%/BELONGS_TO%>
        <%#HAS_MANY%>void set<%FOREIGNKEY_CLASS_PLURAL%>(Dbo::collection< Dbo::ptr<<%FOREIGNKEY_CLASSNAME%>>> c){ptr<%FOREIGNKEY_CLASS_PLURAL%>=c;}
        <%/HAS_MANY%>
        template<class Action> void persist(Action& a)
        {
            <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%>Dbo::field( a, <%VARIABLE_NAME%>_, "<%COLUMN_NAME%>"); 
            <%/COLUMNS%>
            <%#BELONGS_TO%>Dbo::belongsTo(a, ptr<%REFERING_VARIABLE_NAME%>, "<%REFERENCED_TABLENAME%>");
            <%/BELONGS_TO%>
            <%#HAS_MANY%>Dbo::hasMany(a, ptr<%FOREIGNKEY_CLASS_PLURAL%>, Dbo::ManyToOne, "<%TABLE_NAME%>");
            <%/HAS_MANY%>
        }
    
    private:
        <%#COLUMNS%><%#UNSUPPORTED%>//unsupported <%/UNSUPPORTED%><%#UNSIGNED%>unsigned <%/UNSIGNED%> <%DATATYPE%> <%VARIABLE_NAME%>_;
        <%/COLUMNS%>
};

#endif // <%HEADER_GUARD%>

