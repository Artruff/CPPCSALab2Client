#include <assert.h>
#include <check.h>
#include <stdlib.h>
#include <time.h>

#include "MyClient.cpp"

START_TEST(test_authorization) {
    MyClient client;
    auto ptr = client.Authorization("admin_wrong", "123");
    ck_assert_ptr_null(ptr);
    ptr = client.Authorization("admin", "123");
    ck_assert_ptr_nonnull(ptr);
    delete ptr;
}

START_TEST(test_registration) {
    MyClient client;
    auto ptr = client.Registration("admin", "123", Roles::AdminRole);
    ck_assert_ptr_null(ptr);
    ptr = client.Registration("user", "123", Roles::UserRole);
    ck_assert_ptr_nonnull(ptr);
    delete ptr;
}

START_TEST(test_delete) {
    MyClient client;
    auto ptr = client.Delete("", "user");
    ck_assert_ptr_nonnull(ptr);
    delete ptr;
}

START_TEST(test_change_user_data) {
    MyClient client;
    auto ptr = client.ChangeUserData("", "user", "123", "1234");
    ck_assert_ptr_nonnull(ptr);
    delete ptr;
}

START_TEST(test_all_users) {
    MyClient client;
    auto ptr = client.AllUsers("");
    ck_assert_ptr_nonnull(ptr);
    delete ptr;
}
Suite *client_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Client");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_authorization);
  tcase_add_test(tc_core, test_registration);
  tcase_add_test(tc_core, test_delete);
  tcase_add_test(tc_core, test_change_user_data);
  tcase_add_test(tc_core, test_all_users);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = client_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);

  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
  }