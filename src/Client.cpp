#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

#include "Common.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;


void SendOrder(
    tcp::socket& aSocket,
    const std::string& aId,
    const std::string& aRequestType,
    int vol,
    int price,
    const std::string& dir)
{
    nlohmann::json req;
    req["UserId"] = aId;
    req["ReqType"] = aRequestType;
    req["vol"] = vol;
    req["price"] = price;
    req["direction"] = dir;

    std::string request = req.dump();
    boost::asio::write(aSocket, boost::asio::buffer(request, request.size()));
}




// Отправка сообщения на сервер по шаблону.
void SendMessage(
    tcp::socket& aSocket,
    nlohmann::json &j
)
{
    std::string request = j.dump();
    boost::asio::write(aSocket, boost::asio::buffer(request, request.size()));
}

// Возвращает строку с ответом сервера на последний запрос.
nlohmann::json ReadMessage(tcp::socket& aSocket)
{
    boost::asio::streambuf b;
    boost::asio::read_until(aSocket, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    auto j = nlohmann::json::parse(line);
    return j;
}

// "Создаём" пользователя, получаем его ID.
std::string ProcessRegistration(tcp::socket& aSocket)
{
    std::string firstName, lastName, login, pswd;
    std::cout << "Registration required!" << std::endl;
    std::cout << "Enter your first namr, last name, login, password: ";
    std::cin >> firstName >> lastName >> login >>  pswd;

    nlohmann::json j;
    j["UserId"] = "0";
    j["ReqType"] = Requests::Registration;
    j["FirstName"] = firstName;
    j["LastName"] = lastName;
    j["Login"] = login;
    j["Password"] = pswd;

    // Для регистрации Id не нужен, заполним его нулём
    SendMessage(aSocket, j);
    auto rep = ReadMessage(aSocket);
    return rep["UserId"].get<std::string>();
}

std::string Login(tcp::socket& aSocket){
    std::string login, pswd;
    std::cout << "Hello! Enter your login and password: " << std::endl;
    std::cin >> login >>  pswd;

    nlohmann::json j;
    j["ReqType"] = Requests::SignIn;
    j["Login"] = login;
    j["Password"] = pswd;

    SendMessage(aSocket, j);
    auto rep = ReadMessage(aSocket);
    if (rep["Status"].get<std::string>() == "ok"){
        std::cout << rep["Message"].get<std::string>() << std::endl;
        return rep["UserId"].get<std::string>();
    }
    return rep["UserId"].get<std::string>();
}

int main()
{
    try
    {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        // Мы предполагаем, что для идентификации пользователя будет использоваться ID.
        // Тут мы "регистрируем" пользователя - отправляем на сервер имя, а сервер возвращает нам ID.
        // Этот ID далее используется при отправке запросов.
        std::string my_id = Login(s);
        while (my_id == "null"){
            std::cout << "Menu:\n"
                         "1) Login\n"
                         "2) Registration\n"
                         "3) Exit\n"
                        << std::endl;
            int n;
            std::cin >> n;
            switch (n){
                case 1:
                {
                    my_id = Login(s);
                    std::cout << my_id << std::endl;
                    break;
                }
                case 2:
                {
                    my_id = ProcessRegistration(s);
                    break;
                }
                case 3: 
                {
                    exit(0);
                    break;
                }
                default:
                {
                    std::cout << "Unknown menu option\n" << std::endl;
                }
            } 
        }

        while (true)
        {
            // Тут реализовано "бесконечное" меню.
            std::cout << "Menu:\n"
                         "1) Hello Request\n"
                         "2) Exit\n"
                         "3) Make Order\n"
                         "4) Show balance\n"
                         << std::endl;

            short menu_option_num;
            std::cin >> menu_option_num;
            switch (menu_option_num)
            {
                case 1:
                {
                    // Для примера того, как может выглядить взаимодействие с сервером
                    // реализован один единственный метод - Hello.
                    // Этот метод получает от сервера приветствие с именем клиента,
                    // отправляя серверу id, полученный при регистрации.
                    nlohmann::json req;
                    req["UserId"] = my_id;
                    req["ReqType"] = Requests::Hello;
                    SendMessage(s, req);
                    auto rep = ReadMessage(s);
                    std::cout << rep["Name"].get<std::string>() << std::endl;
                    break;
                }
                case 2:
                {
                    exit(0);
                    break;
                }
                case 3:
                {
                    int vol, price;
                    std::string dir;

                    std::cin >> vol >> price >> dir;

                    nlohmann::json req;
                    req["UserId"] = my_id;
                    req["ReqType"] = Requests::Order;
                    req["vol"] = vol;
                    req["price"] = price;
                    req["direction"] = "'" + dir + "'";
                
                    SendMessage(s, req);
                    auto resp = ReadMessage(s);
                    std::cout << resp["Message"].get<std::string>() << std::endl;
                    break;

                }
                case 4:
                {
                    nlohmann::json req;
                    req["UserId"] = my_id;
                    req["ReqType"] = Requests::Balance;
                    SendMessage(s, req);
                    auto resp = ReadMessage(s);
                    auto usd = resp["USD"].get<std::vector<std::string>>();
                    auto rub = resp["RUB"].get<std::vector<std::string>>();
                    for (int i = 0; i < usd.size(); i++){
                        std::cout << usd[i] << " " << rub[i] << std::endl;
                    }

                }
                default:
                {
                    std::cout << "Unknown menu option\n" << std::endl;
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}