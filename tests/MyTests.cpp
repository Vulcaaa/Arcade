/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** MyTests
*/

#include <criterion/criterion.h>

Test(basic_tests, simple_addition) {
    int a = 2;
    int b = 2;
    cr_assert_eq(a + b, 4, "2 + 2 should be 4");
}
