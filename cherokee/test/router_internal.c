#include    <criterion/criterion.h>
#include    "list.h"
#include    "router.h"

Test(router_internal, find_existing_route) {
    t_routes            *routes = new_list();
    t_route_handler     *needle = NULL;
    char                *route_name = "test";
    t_route_handler     handler = {
        .route = strdup(route_name),
        .methods = NULL,
        .handler = NULL,
    };

    cr_expect_not_null(routes, "Routes container shouldn't be empty");
    insert_list(routes, &handler);
    cr_expect(routes->size == 1, "Only expecting ONE route in this test container");
    needle = get_route_by_path(routes, route_name);
    cr_expect_not_null(needle, "Test is not found");
    cr_expect(needle == &handler, "Found route doesn't match");
}

Test(router_internal, find_undefined_route) {
    t_routes            *routes = new_list();
    t_route_handler     *needle = NULL;
    char                *route_name = "mushi";
    t_route_handler     handler = {
        .route = "test",
        .methods = NULL,
        .handler = NULL,
    };

    cr_expect_not_null(routes, "Routes container shouldn't be empty");
    insert_list(routes, &handler);
    cr_expect(routes->size == 1, "Only expecting ONE route in this test container");
    needle = get_route_by_path(routes, route_name);
    cr_expect_null(needle, "A route was found, but the route doesn't exist");
}