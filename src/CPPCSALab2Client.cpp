#include "MyClient.cpp"
#include <iostream>
#include <string>
#include "../hv/s21_tetris_game_part.h"

enum Step
{
    StartStep,
    Authorization,
    Registration,
    AdminStep,
    AddNewUser,
    ChangeUserData,
    ChangeUserScore,
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
            return StartStep;
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
            return StartStep;
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
    std::cout << "User: " << user.login << " hi-score: " << user.score << std::endl;
    std::cout << "1. Change user data" << std::endl;
    std::cout << "2. Get score table" << std::endl;
    std::cout << "3. Start game" << std::endl;
    std::cout << "4. Delete user data" << std::endl;
    std::cout << "0. Exit" << std::endl;

    while (true)
    {
        std::string input;
        std::cin >> input;

        if (input == "0")
        {
            return StartStep;
        }
        else if (input == "1")
        {
            return ChangeUserData;
        }
        else if (input == "2")
        {
            std::vector<User>* response = client.ScoreTable(key);
            if (response != nullptr && !response->empty())
            {
                for (const User& user : (*response))
                {
                    std::cout << user.login <<  " " << user.role << " " << user.score << std::endl;
                }
            }
            if (response != nullptr)
                delete response;
        }
        else if (input == "3")
        {
            int score = start_game();
            endwin();
            refresh();
            std::cout << "new score" << std::endl;
            //int score = 0;
            std::map<std::string, std::string>* response = client.ChangeUserScore(key, user.login, score);
            if (response != nullptr && response->count("message") > 0)
            {
                std::cout << "Invalid login. Please try again." << std::endl;
                std::cout << (*response)["message"] << std::endl;
            }
            system("clear");
        }
        else if (input == "4")
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
    std::cout << "1. All scores" << std::endl;
    std::cout << "2. Change users" << std::endl;
    std::cout << "3. Change score data" << std::endl;
    std::cout << "4. Add user" << std::endl;
    std::cout << "5. Delete user" << std::endl;
    std::cout << "0. Exit" << std::endl;

    while (true)
    {
        std::string input;
        std::cin >> input;

        if (input == "0")
        {
            return Step::StartStep;
        }

        if(input == "1")
        {
            std::vector<User>* response = client.ScoreTable(key);

            if (response != nullptr && !response->empty())
            {
                for (const User& user : (*response))
                {
                    std::cout << user.login << " " << user.password << " " << user.role << " " << user.score << std::endl;
                }
            }
            if (response != nullptr)
                delete response;
        }
        else if(input == "2")
            return Step::ChangeUserData;
        else if(input == "3")
            return Step::ChangeUserScore;
        else if (input == "4")
            return Step::AddNewUser;
        else if (input == "5")
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
            return Step::StartStep;
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

Step ChangeScoreMenu()
{
    std::string login;

    std::cout << "Input login:" << std::endl;
    std::cout << "0. back" << std::endl;

    while (true)
    {
        if (user.role == Roles::UserRole)
        {
            login = user.login;
        }
        else
        {
            std::cin >> login;
        }

        if (login == "0")
        {
            return Step::StartStep;
        }

        std::cout << "Input new score:" << std::endl;
        int score;
        std::cin >> score;

        std::cout << "Process change..." << std::endl;
        std::map<std::string, std::string>* response = client.ChangeUserScore(key, login, score);

        if (response != nullptr && response->count("message") > 0)
        {
            std::cout << "Invalid login. Please try again." << std::endl;
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
        std::string roleInput;
        std::cin >> roleInput;
        Roles role;
        if(roleInput == "1" || roleInput == "2")
            role = static_cast<Roles>(std::stoi(roleInput));
        else if(roleInput == "admin" || roleInput == "Admin")
            role = Roles::AdminRole;
        else
            role = Roles::UserRole;

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
            login = user.login;
    }

    while (true)
    {
        if (login == "0")
        {
            return Step::StartStep;
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
                return Step::StartStep;
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
    Step step = StartStep;
    while (step != Exit)
    {
        system("clear");
        try
        {
            switch (step)
            {
            case StartStep:
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
                endwin();
                refresh();
                break;
            case AdminStep:
                step = AdminMenu();
                break;
            case ChangeUserData:
                step = ChangeUserMenu();
                break;
            case ChangeUserScore:
                step = ChangeScoreMenu();
                break;
            case AddNewUser:
                step = AddNewUserMenu();
                break;
            case DeleteUserData:
                step = DeleteMenu();
                break;
            default:
                step = StartStep;
                break;
            }
        }
        catch (std::exception& ex)
        {
            std::cout << ex.what() << std::endl;
            step = StartStep;
        }
    }

    return 0;
}