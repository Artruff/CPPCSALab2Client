#include "MyClient.cpp"
#include <iostream>
#include <string>

enum Step
{
    Start,
    Authorization,
    Registration,
    AdminStep,
    AddNewUser,
    ChangeUserData,
    DeleteUserData,
    UserStep,
    Exit
};

User user;
std::string key;
MyClient client;

Step StartMenu()
{
    std::cout << "Select an action:" << std::endl;
    std::cout << "1. Authorization" << std::endl;
    std::cout << "2. Registration" << std::endl;
    std::cout << "0. Exit" << std::endl;

    while (true)
    {
        std::string input;
        std::cin >> input;
        if (input == "1")
        {
            return Authorization;
        }
        else if (input == "2")
        {
            return Registration;
        }
        else if (input == "0")
        {
            return Exit;
        }
        else
        {
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
}

Step AuthorizationMenu()
{
    std::cout << "Input login:" << std::endl;
    std::cout << "0. back" << std::endl;

    while (true)
    {
        std::string login;
        std::cin >> login;

        if (login == "0")
        {
            return Start;
        }

        std::cout << "Input password:" << std::endl;
        std::string password;
        std::cin >> password;

        std::cout << "Process authorization..." << std::endl;
        std::map<std::string, std::string> *response = client.Authorization(login, password);

        if (response!=nullptr && response->count("key") > 0)
        {
            key = (*response)["key"];
            user.login = login;
            user.password = password;
            user.role = static_cast<Roles>(std::stoi((*response)["role"]));

            delete response;
            if (user.role == AdminRole)
            {
                return AdminStep;
            }
            else
            {
                return UserStep;
            }
        }
        else
        {
            std::cout << "Invalid login or password. Please try again." << std::endl;
        }
        if (response != nullptr && response->count("message") > 0)
        {
            std::cout << (*response)["message"] << std::endl;
        }
        if(response != nullptr)
            delete response;
    }
}

Step RegistrationMenu()
{
    std::cout << "Input login:" << std::endl;
    std::cout << "0. back" << std::endl;

    while (true)
    {
        std::string login;
        std::cin >> login;

        if (login == "0")
        {
            return Start;
        }

        std::cout << "Input password:" << std::endl;
        std::string password;
        std::cin >> password;

        std::cout << "Process registration..." << std::endl;
        std::map<std::string, std::string>* response = client.Registration(login, password);

        if (response != nullptr && response->count("message") > 0)
        {
            std::cout << "Invalid login or password. Please try again.\n";
            std::cout << (*response)["message"] << std::endl;
        }
        else
        {
            if (response != nullptr)
                delete response;
            response = client.Authorization(login, password);
            key = (*response)["key"];
            user.login = login;
            user.password = password;
            user.role = UserRole;
            if (response != nullptr)
                delete response;
            return Step::UserStep;
        }
    }
}

Step UserMenu()
{
    std::cout << "User: " << user.login << std::endl;
    std::cout << "1. Change user data" << std::endl;
    std::cout << "2. Delete user data" << std::endl;
    std::cout << "0. Exit" << std::endl;

    while (true)
    {
        std::string input;
        std::cin >> input;

        if (input == "0")
        {
            return Start;
        }
        else if (input == "1")
        {
            return ChangeUserData;
        }
        else if (input == "2")
        {
            return DeleteUserData;
        }
        else
        {
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
}

Step AdminMenu()
{
    std::cout << "Admin: " << user.login << std::endl;
    std::cout << "1. All users" << std::endl;
    std::cout << "2. Change users" << std::endl;
    std::cout << "3. Add user" << std::endl;
    std::cout << "4. Delete user" << std::endl;
    std::cout << "0. Exit" << std::endl;

    while (true)
    {
        std::string input;
        std::cin >> input;

        if (input == "0")
        {
            return Step::Start;
        }

        if(input == "1")
        {
            std::vector<User>* response = client.AllUsers(key);

            if (response != nullptr && !response->empty())
            {
                for (const User& user : (*response))
                {
                    std::cout << user.login << " " << user.password << " " << user.role << std::endl;
                }
            }
            if (response != nullptr)
                delete response;
        }
        else if(input == "2")
            return Step::ChangeUserData;
        else if (input == "3")
            return Step::AddNewUser;
        else if (input == "4")
            return Step::DeleteUserData;
        else
        {
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
}

Step ChangeUserMenu()
{
    std::string old_login;

    if (user.role == Roles::AdminRole)
    {
        std::cout << "Input login:" << std::endl;
        std::cout << "0. back" << std::endl;
    }

    while (true)
    {
        if (user.role == Roles::UserRole)
        {
            old_login = user.login;
        }
        else
        {
            std::cin >> old_login;
        }

        if (old_login == "0")
        {
            return Step::Start;
        }

        std::cout << "Input new login:" << std::endl;
        std::cout << "0. back" << std::endl;
        std::string new_login;
        std::cin >> new_login;

        if (new_login == "0")
        {
            if (user.role == Roles::AdminRole)
            {
                return Step::AdminStep;
            }
            else
            {
                return Step::UserStep;
            }
        }

        std::cout << "Input new password:" << std::endl;
        std::string new_password;
        std::cin >> new_password;

        std::cout << "Process change..." << std::endl;
        std::map<std::string, std::string>* response = client.ChangeUserData(key, old_login, new_login, new_password);

        if (response != nullptr && response->count("message") > 0)
        {
            std::cout << "Invalid login or password. Please try again." << std::endl;
            std::cout << (*response)["message"] << std::endl;
        }
        else
        {
            user.login = new_login;
            user.password = new_password;

            if (response != nullptr)
                delete response;
            if (user.role == Roles::AdminRole)
            {
                return Step::AdminStep;
            }
            else
            {
                return Step::UserStep;
            }
        }
    }
}

Step AddNewUserMenu()
{
    std::cout << "Input login:" << std::endl;
    std::cout << "0. back" << std::endl;

    while (true)
    {
        std::string login;
        std::cin >> login;

        if (login == "0")
        {
            return Step::AdminStep;
        }

        std::cout << "Input password:" << std::endl;
        std::string password;
        std::cin >> password;

        std::cout << "Input role:" << std::endl;
        int roleInput;
        std::cin >> roleInput;
        Roles role = static_cast<Roles>(roleInput);

        std::cout << "Process add..." << std::endl;
        std::map<std::string, std::string> *response = client.Registration(login, password, role);

        if (response != nullptr && response->count("message") > 0)
        {
            std::cout << "Invalid login or password. Please try again.\n";
            std::cout << (*response)["message"] << std::endl;
        }
        else
        {
            if (response != nullptr)
                delete response;
            return Step::AdminStep;
        }
    }
}

Step DeleteMenu()
{
    std::string login = "";
    if (user.role == Roles::AdminRole)
    {
        std::cout << "Input login:" << std::endl;
        std::cout << "0. back" << std::endl;

        if (user.role == Roles::AdminRole)
            std::cin >> login;
        else
            user.login;
    }

    while (true)
    {
        if (login == "0")
        {
            return Step::Start;
        }

        std::cout << "Process delete..." << std::endl;
        std::map<std::string, std::string> *response = client.Delete(key, login);

        if ( response != nullptr && response->count("message") > 0)
        {
            std::cout << "Invalid login. Please try again." << std::endl;
            std::cout << (*response)["message"] << std::endl;
        }
        else
        {
            if (response != nullptr)
                delete response;
            if (user.login == login)
            {
                return Step::Start;
            }
            else
            {
                if (user.role == Roles::AdminRole)
                {
                    return Step::AdminStep;
                }
                else
                {
                    return Step::UserStep;
                }
            }
        }
    }
}

int main()
{
    Step step = Start;
    while (step != Exit)
    {
        system("clear");
        try
        {
            switch (step)
            {
            case Start:
                user.login = "";
                user.password = "";
                step = StartMenu();
                break;
            case Authorization:
                step = AuthorizationMenu();
                break;
            case Registration:
                step = RegistrationMenu();
                break;
            case UserStep:
                step = UserMenu();
                break;
            case AdminStep:
                step = AdminMenu();
                break;
            case ChangeUserData:
                step = ChangeUserMenu();
                break;
            case AddNewUser:
                step = AddNewUserMenu();
                break;
            case DeleteUserData:
                step = DeleteMenu();
                break;
            default:
                step = Start;
                break;
            }
        }
        catch (std::exception& ex)
        {
            std::cout << ex.what() << std::endl;
            step = Start;
        }
    }

    return 0;
}