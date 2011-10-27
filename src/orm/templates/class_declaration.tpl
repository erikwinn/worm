#include <Wt/Dbo/Dbo>

{{#STRING_INCLUDE}}
#include <string>
{{/STRING_INCLUDE}}

namespace dbo = Wt::Dbo;

class {{CLASS_NAME}} {

    public:
        {{#COLUMNS}}
        {{#UNSUPPORTED}}//unsupported {{/UNSUPPORTED}}{{UNSIGNED}} {{DATATYPE}} {{VARIABLENAME}};
        {{/COLUMNS}}

    template<class Action> void persist(Action& a)
    {
        {{#COLUMNS}}
        {{#UNSUPPORTED}}//unsupported {{/UNSUPPORTED}}dbo::field(a, {{VARIABLENAME}},"{{COLUMNNAME}}");
        {{/COLUMNS}}
    }
};
