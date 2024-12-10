#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "MyClient.cpp"

TEST_CASE("Authorization test")
{
    MyClient client;
    SECTION("Authorization with wrong credentials should return null")
    {
        auto wr_ptr = client.Authorization("admin_wrong", "123");
        REQUIRE(wr_ptr == nullptr);
    }

    SECTION("Authorization with correct credentials should return pointer")
    {
        auto ptr = client.Authorization("admin", "123");
        REQUIRE(ptr != nullptr);
        delete ptr;
    }
}

TEST_CASE("test_registration")
{
    MyClient client;
    auto ptr = client.Registration("admin", "123", Roles::AdminRole);
    REQUIRE(ptr == nullptr);
    ptr = client.Registration("user", "123", Roles::UserRole);
    REQUIRE(ptr != nullptr);
    delete ptr;
}

TEST_CASE("test_delete")
{
    MyClient client;
    auto ptr = client.Delete("", "user");
    REQUIRE(ptr != nullptr);
    delete ptr;
}

TEST_CASE("test_change_user_data")
{
    MyClient client;
    auto ptr = client.ChangeUserData("", "user", "123", "1234");
    REQUIRE(ptr != nullptr);
    delete ptr;
}

TEST_CASE("test_all_users")
{
    MyClient client;
    auto ptr = client.AllUsers("");
    REQUIRE(ptr != nullptr);
    delete ptr;
}