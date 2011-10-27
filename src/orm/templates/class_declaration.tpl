#include <Wt/Dbo/Dbo>

{{#INCLUDES}}{{INCLUDE}}
{{/INCLUDES}}

namespace dbo = Wt::Dbo;

class {{CLASS_NAME}} {

    public:
        {{#COLUMNS}}{{#UNSUPPORTED}}//unsupported {{/UNSUPPORTED}}{{UNSIGNED}} {{DATATYPE}} {{VARIABLE_NAME}};
        {{/COLUMNS}}

    template<class Action> void persist(Action& a)
    {
        {{#COLUMNS}}{{#UNSUPPORTED}}//unsupported {{/UNSUPPORTED}}dbo::field(a, {{VARIABLE_NAME}},"{{COLUMN_NAME}}");
        {{/COLUMNS}}
    }
};
