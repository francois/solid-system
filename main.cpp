#include <iostream>
#include <pqxx/pqxx>

int main()
{
    try
    {
        pqxx::connection C;
        std::cout << "Connected to " << C.dbname() << std::endl;
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT table_schema, table_name FROM information_schema.tables WHERE table_schema = 'public'");

        std::cout << "Found " << R.size() << "employees:" << std::endl;
        for (auto row: R)
        {
            std::cout << row[0].c_str() << "." << row[1].c_str() << std::endl;
        }

        std::cout << "Doubling all employees' salaries..." << std::endl;
        W.exec("UPDATE employee SET salary=salary*2");

        std::cout << "Making changes definite: ";
        W.commit();
        std::cout << "ok." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
