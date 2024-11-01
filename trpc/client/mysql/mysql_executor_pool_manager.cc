#include "trpc/client/mysql/mysql_executor_pool_manager.h"
#include <sstream>

namespace trpc {
namespace mysql {

MysqlExecutorPoolManager::MysqlExecutorPoolManager(MysqlExecutorPoolOption&& option) { option_ = std::move(option); }

MysqlExecutorPool* MysqlExecutorPoolManager::Get(const NodeAddr& node_addr) {
  const int len = 64;
  std::string endpoint(len, 0x0);
  std::snprintf(const_cast<char*>(endpoint.c_str()), len, "%s:%d", node_addr.ip.c_str(), node_addr.port);

  MysqlExecutorPool* executor_pool{nullptr};
  bool ret = execuctor_pools_.Get(endpoint, executor_pool);

  if (ret) {
    return executor_pool;
  }

  MysqlExecutorPool* pool = CreateExecutorPool(node_addr);
  ret = execuctor_pools_.GetOrInsert(endpoint, pool, executor_pool);
  if (!ret) {
    return pool;
  }

  delete pool;
  pool = nullptr;

  return executor_pool;
}

MysqlExecutorPool* MysqlExecutorPoolManager::CreateExecutorPool(const NodeAddr& node_addr) {
  MysqlExecutorPool* new_pool{nullptr};
  new_pool = new MysqlExecutorPoolImpl(option_, node_addr);
  return new_pool;
}

void MysqlExecutorPoolManager::Stop() {
  execuctor_pools_.GetAllItems(pools_to_destroy_);

  for(auto& [key, pool] : pools_to_destroy_)
    pool->Stop();
}

void MysqlExecutorPoolManager::Destroy() {
  for (auto& [key, pool] : pools_to_destroy_) {
    pool->Destroy();
    delete pool;
    pool = nullptr;
  }

  execuctor_pools_.Reclaim();
  pools_to_destroy_.clear();
}

}  // namespace mysql
}  // namespace trpc
