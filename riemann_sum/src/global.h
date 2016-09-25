#include <map>
#include <memory>
#include <mutex>

extern std::map<int, std::unique_ptr<std::mutex>> mutex_map;