#include <unistd.h>
#include <cassert>
#include <map>
#include <string>
#include <vector>

#include "../../test_utils/test_utils.h"

using namespace std;

int main() {
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  string hostname(hostnamebuf);

  string shardmaster_addr = hostname + ":8080";
  start_shardmaster(shardmaster_addr);

  string skv_1 = hostname + ":8081";
  string skv_2 = hostname + ":8082";
  map<string, vector<shard_t>> m;

  assert(test_join(shardmaster_addr, skv_1, true));
  m[skv_1].push_back({0, 1000});
  assert(test_query(shardmaster_addr, m));
  m.clear();

  assert(test_join(shardmaster_addr, skv_2, true));
  m[skv_1].push_back({0, 500});
  m[skv_2].push_back({501, 1000});
  assert(test_query(shardmaster_addr, m));
  m.clear();

  assert(test_leave(shardmaster_addr, {skv_1}, true));
  m[skv_2].push_back({0, 1000});
  assert(test_query(shardmaster_addr, m));

  return 0;
}
