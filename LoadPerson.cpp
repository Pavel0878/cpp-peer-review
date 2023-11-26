
struct Db {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};

struct Person {
    int min_age;
    int max_age;
    string_view name_filter;
};

vector<Person> LoadPersons(Db& db_struct, Person& person)
{
    DBConnector connector(db_struct.db_allow_exceptions, db_struct.db_log_level);
    DBHandler db;
    if (db_struct.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_struct.db_name, db_struct.db_connection_timeout);
    }
    else {
        db = connector.Connect(db_struct.db_name, db_struct.db_connection_timeout);
    }
    if (!db_struct.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << person.min_age << " and "s << person.max_age << " "s
        << "and Name like '%"s << db.Quote(person.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}