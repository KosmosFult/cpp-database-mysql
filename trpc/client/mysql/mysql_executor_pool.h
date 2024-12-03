//
//
// Tencent is pleased to support the open source community by making tRPC available.
//
// Copyright (C) 2023 THL A29 Limited, a Tencent company.
// All rights reserved.
//
// If you have downloaded a copy of the tRPC source code from Tencent,
// please note that tRPC source code is licensed under the  Apache 2.0 License,
// A copy of the Apache 2.0 License is included in this file.
//
//

#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <list>
#include "trpc/client/mysql/executor/mysql_executor.h"
#include "trpc/transport/common/transport_message_common.h"


namespace trpc::mysql {

struct MysqlExecutorPoolOption {
  uint32_t max_size{0};       // Maximum number of connections in the pool

  uint64_t max_idle_time{0};  // Maximum idle time for connections

  uint32_t num_shard_group{4};

  std::string dbname;

  std::string username;

  std::string password;

  std::string char_set;
};

class MysqlExecutorPool {
 public:
  MysqlExecutorPool(const MysqlExecutorPoolOption& option, const NodeAddr& node_addr);

  RefPtr<MysqlExecutor> GetExecutor();

  void Reclaim(int ret, RefPtr<MysqlExecutor>&&);

  void Stop();

  void Destroy();

 private:
  RefPtr<MysqlExecutor> CreateExecutor(uint32_t shard_id);

  RefPtr<MysqlExecutor> GetOrCreate();

  bool IsIdleTimeout(RefPtr<MysqlExecutor> executor);

 private:
  MysqlExecutorPoolOption pool_option_;

  NodeAddr target_;

  std::atomic<uint32_t> executor_num_{0};

  // The maximum number of connections that can be stored per `Shard` in `conn_shards_`
  uint32_t max_num_per_shard_{0};

  struct alignas(hardware_destructive_interference_size) Shard {
    std::mutex lock;
    std::list<RefPtr<MysqlExecutor>> mysql_executors;
  };

  std::unique_ptr<Shard[]> executor_shards_;

  std::atomic<uint32_t> shard_id_gen_{0};
  std::atomic<uint32_t> executor_id_gen_{0};

};


} // namespace trpc::mysql

