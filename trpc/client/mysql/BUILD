licenses(["notice"])

package(default_visibility = ["//visibility:public"])


cc_library(
    name = "transaction",
    hdrs = ["transaction.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//trpc/client/mysql/executor:mysql_executor",
        "@trpc_cpp//trpc/util:ref_ptr",
    ]
)

cc_library(
    name = "mysql_executor_pool",
    srcs = ["mysql_executor_pool.cc"],
    hdrs = ["mysql_executor_pool.h"],
    deps = [
        "@trpc_cpp//trpc/transport/common:transport_message_common",
        "//trpc/client/mysql/executor:mysql_executor",
        "@trpc_cpp//trpc/util/log:logging",
        ":mysql_service_config",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "mysql_service_proxy",
    srcs = ["mysql_service_proxy.cc"],
    hdrs = ["mysql_service_proxy.h"],
    deps = [
        ":mysql_executor_pool",
        ":mysql_executor_pool_manager",
        ":mysql_service_config",
        ":transaction",
        "//trpc/client/mysql/executor:mysql_executor",
        "@trpc_cpp//trpc/client:service_proxy_option",
        "@trpc_cpp//trpc/codec:client_codec_factory",
        "@trpc_cpp//trpc/codec/mysql:mysql_client_codec",
        "@trpc_cpp//trpc/util/string:string_util",
        "@trpc_cpp//trpc/util/thread:thread_pool",
        "@trpc_cpp//trpc/util/thread:latch",
        "@trpc_cpp//trpc/coroutine:fiber",
        "@trpc_cpp//trpc/client:service_proxy",
        "@trpc_cpp//trpc/common/logging:trpc_logging",
    ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "mysql_executor_pool_manager",
    srcs = ["mysql_executor_pool_manager.cc"],
    hdrs = ["mysql_executor_pool_manager.h"],
    deps = [
        ":mysql_executor_pool",
        "@trpc_cpp//trpc/common/config:mysql_client_conf",
        "@trpc_cpp//trpc/util/concurrency:lightly_concurrent_hashmap"
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "mysql_service_config",
    hdrs = ["mysql_service_config.h"],
    visibility = ["//visibility:public"],
)




cc_test(
    name = "mysql_service_proxy_test",
    srcs = ["mysql_service_proxy_test.cc"],
        visibility = ["//visibility:public"],
    deps = [
        ":mysql_service_proxy",
        ":mysql_executor_pool_manager",
        "@trpc_cpp//trpc/client:make_client_context",
        "@trpc_cpp//trpc/client:service_proxy_option_setter",
        "@trpc_cpp//trpc/common:trpc_plugin",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ]
)

filegroup(
    name = "test_yaml_files",
    srcs = glob([
        "testing/*.yaml",
    ]),
)

cc_test(
    name = "mysql_executor_pool_manager_test",
    srcs = ["mysql_executor_pool_manager_test.cc"],
    data = [
        ":test_yaml_files"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//trpc/client/mysql/executor:mysql_executor",
        ":mysql_executor_pool_manager",
        ":mysql_service_config",
        "@trpc_cpp//trpc/common/config:trpc_config",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ],
)