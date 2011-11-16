/*
 g++ -g worm_wt_example.cpp -o wormex -rdynamic -lworm -lmysqlclient -lsqlite3 -lctemplate_nothreads -lwt -lwtext -lwthttp -lwtdbo -lwtdbosqlite3 -lwtdbopostgres -lboost_date_time-mt -lboost_regex-mt -lboost_program_options-mt -lboost_signals-mt -lboost_system-mt -lboost_filesystem-mt -lboost_thread-mt
 */


#include <Wt/WApplication>
#include <Wt/WServer>
#include <Wt/WTable>
#include <Wt/WString>
#include <Wt/WEnvironment>
#include <Wt/WBoxLayout>
#include <Wt/WText>
#include <Wt/WContainerWidget>

#include "../src/sql/wsqldatabase.h"

using Wt::WApplication;
using Wt::WEnvironment;
using Wt::WString;
using Wt::WBoxLayout;
using Wt::WText;
using std::string;

Wt::WTable* getdbtables() {
    Wt::WTable *table = new Wt::WTable();
    table->setMargin("0.5em");
    Wt::WCssDecorationStyle *border = new Wt::WCssDecorationStyle();
    border->setBorder(Wt::WBorder(Wt::WBorder::Dotted));
    table->setDecorationStyle(*border);
    //  border->setBorder(Wt::WBorder(Wt::WBorder::Dashed ));


    WSql::WSqlDatabase db(WSql::WSQLITE);
    db.setDatabaseName(string("blog.db"));
    if (!db.open()) {
        std::cerr << "nope .." << db.error().text() << std::endl;
        return table;
    }
    std::vector<string>tables = db.tableNames();
    std::vector<string>::iterator it = tables.begin();
    WSql::WSqlTable stbl;
    int numflds = 0;
    int row = 0;
    while (it != tables.end()) {
        stbl = db.tableMetaData(*it++);
        numflds = stbl.count();
        Wt::WTableCell *cell = table->elementAt(row++, 0);
        cell->addWidget(new Wt::WText(WString("Table Name: {1}").arg(stbl.name())));
        cell->setColumnSpan(5);
        cell->setPadding("0.5em");
        cell->setContentAlignment(Wt::AlignCenter);
        int i = 0;
        for (; i < numflds; ++i) {
            WSql::WSqlColumn clm = stbl.column(i);

            table->elementAt(row + i, 0)->addWidget(new Wt::WText("Field: "));
            table->elementAt(row + i, 0)->setPadding("0.5em");
            table->elementAt(row + i, 0)->setDecorationStyle(*border);
            table->elementAt(row + i, 1)->addWidget(new Wt::WText(clm.columnName()));
            table->elementAt(row + i, 1)->setPadding("0.5em");
            table->elementAt(row + i, 1)->setDecorationStyle(*border);
            table->elementAt(row + i, 2)->addWidget(new Wt::WText(WSql::WSqlDataType::toString(clm.type())));
            table->elementAt(row + i, 2)->setPadding("0.5em");
            table->elementAt(row + i, 2)->setDecorationStyle(*border);
            table->elementAt(row + i, 3)->addWidget(new Wt::WText(clm.defaultValue<string > ()));
            table->elementAt(row + i, 3)->setPadding("0.5em");
            table->elementAt(row + i, 3)->setDecorationStyle(*border);
            table->elementAt(row + i, 4)->addWidget(new Wt::WText(clm.isPrimaryKey() ? "Primary Key" : "Not primary"));
            table->elementAt(row + i, 4)->setPadding("0.5em");
            table->elementAt(row + i, 4)->setDecorationStyle(*border);
            table->elementAt(row + i, 5)->addWidget(new Wt::WText(clm.canBeNull() ? "Nullable" : "Not nullable"));
            table->elementAt(row + i, 5)->setPadding("0.5em");
            table->elementAt(row + i, 5)->setDecorationStyle(*border);
        }
        row += i;
    }
    db.close();
    return table;
}

class wormApp : public Wt::WApplication
{
public:
    wormApp(const Wt::WEnvironment& env);
};

wormApp::wormApp(const Wt::WEnvironment& env) : Wt::WApplication(env) {
    setTitle("worm");
    Wt::WTable * tbl = getdbtables();
    root()->addWidget(new WText("Tables"));
    root()->addWidget(tbl);
}

WApplication *createwormApp(const WEnvironment& env)
{
    WApplication *app = new wormApp(env);
    return app;
}

int main(int argc, char **argv)
{
    Wt::WServer server(argv[0]);
    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
    server.addEntryPoint(Wt::Application, createwormApp);
    if (server.start()) {
        int sig = Wt::WServer::waitForShutdown(argv[0]);
        std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
        server.stop();
    }
}
