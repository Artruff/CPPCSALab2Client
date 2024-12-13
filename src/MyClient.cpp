#pragma once
#include <string>
#include <map>
#include <vector>
#include "../hv/hv.h"
#include "../hv/requests.h"
using json = nlohmann::json;
using namespace hv;
enum Roles
{
	UserRole,
	AdminRole
};

typedef struct UserStruct
{
	std::string login;
	std::string password;
	Roles role;

	bool operator==(const UserStruct &other) const
	{
		return login == other.login && password == other.password && role == other.role;
	}

	bool operator<(const UserStruct &other) const
	{
		if (login != other.login)
			return login < other.login;
		if (password != other.password)
			return password < other.password;
		return role < other.role;
	}
} User;

class MyClient
{
public:
	std::string GetServerTime()
	{
		try
		{
			char url[] = "http://127.0.0.1:8080/time";
			auto response = requests::get(url);

			if (response != NULL)
			{
				json response_data = json::parse(response->body.c_str());
				return response_data["time"].get<std::string>();
			}
			else
			{
				return "Err";
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "Exception caught: " << e.what() << std::endl;
			return "Err";
		}
		return "Err";
	}
	std::map<std::string, std::string> *Authorization(const std::string &login, const std::string &password)
	{
		char url[] = "http://127.0.0.1:8080/authorization";
		json data = {
			{"login", login},
			{"password", password}};
		std::string json_data = data.dump();
		http_headers headers;
		headers["Content-Type"] = "application/json";
		auto r = requests::post(url, json_data, headers);

		if (r != NULL)
		{
			json response_data = json::parse(r->body.c_str());
			return new std::map<std::string, std::string>(response_data.get<std::map<std::string, std::string>>());
			// return nullptr;
		}
		else
		{
			// Handle Error
			return nullptr;
		}
	}

	std::map<std::string, std::string> *Registration(const std::string &login, const std::string &password, Roles role = Roles::UserRole)
	{
		try
		{
			char url[] = "http://127.0.0.1:8080/registration";
			json data = {
				{"login", login},
				{"password", password},
				{"role", role}};

			std::string json_data = data.dump();
			http_headers headers;
			headers["Content-Type"] = "application/json";
			auto response = requests::post(url, json_data, headers);

			if (response != NULL)
			{
				json response_data = json::parse(response->body.c_str());
				if (response->body[0] == 'n')
					return nullptr;
				return new std::map<std::string, std::string>(response_data.get<std::map<std::string, std::string>>());
			}
			else
			{
				return nullptr;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "Exception caught: " << e.what() << std::endl;
			return nullptr;
		}
		return nullptr;
	}

	std::map<std::string, std::string> *Delete(const std::string &key, const std::string &login)
	{
		try
		{
			char url[] = "http://127.0.0.1:8080/delete";
			json data = {
				{"login", login},
				{"key", key}};

			std::string json_data = data.dump();
			http_headers headers;
			headers["Content-Type"] = "application/json";
			auto response = requests::post(url, json_data, headers);

			if (response != NULL)
			{
				json response_data = json::parse(response->body.c_str());
				if (response->body[0] == 'n')
					return nullptr;
				return new std::map<std::string, std::string>(response_data.get<std::map<std::string, std::string>>());
			}
			else
			{
				return nullptr;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "Exception caught: " << e.what() << std::endl;
			return nullptr;
		}
		return nullptr;
	}

	std::map<std::string, std::string> *ChangeUserData(const std::string &key, const std::string &old_login, const std::string &new_login, const std::string &new_password)
	{
		try
		{
			char url[] = "http://127.0.0.1:8080/change";
			json data = {
				{"old_login", old_login},
				{"new_login", new_login},
				{"new_password", new_password},
				{"key", key}};

			std::string json_data = data.dump();
			http_headers headers;
			headers["Content-Type"] = "application/json";
			auto response = requests::post(url, json_data, headers);

			if (response != NULL)
			{
				json response_data = json::parse(response->body.c_str());
				if (response->body[0] == 'n')
					return nullptr;
				return new std::map<std::string, std::string>(response_data.get<std::map<std::string, std::string>>());
			}
			else
			{
				return nullptr;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "Exception caught: " << e.what() << std::endl;
			return nullptr;
		}
		return nullptr;
	}

	std::vector<User> *AllUsers(const std::string &key)
	{
		try
		{
			char url[] = "http://127.0.0.1:8080/users";
			json data = {
				{"key", key}};

			std::string json_data = data.dump();
			http_headers headers;
			headers["Content-Type"] = "application/json";
			auto response = requests::post(url, json_data, headers);

			if (response != NULL)
			{
				json response_data = json::parse(response->body.c_str());
				if (response_data.find("users") != response_data.end() && response_data["users"].is_array())
				{
					std::vector<User> users;
					auto user_array = response_data["users"];
					for (const auto &item : user_array)
					{
						User user{
							item["login"].get<std::string>(),
							item["password"].get<std::string>(),
							(Roles)item["role"].get<int>()};
						users.push_back(user);
					}
					return new std::vector<User>(users);
				}
			}
			else
			{
				return nullptr;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "Exception caught: " << e.what() << std::endl;
			return nullptr;
		}
		return nullptr;
	}
};