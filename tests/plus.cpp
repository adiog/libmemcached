/*
  C++ interface test
*/
#include "libmemcached/memcached.hh"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "server.h"

#include "test.h"

#include <string>

using namespace std;

extern "C" {
   test_return basic_test(memcached_st *memc);
   uint8_t increment_test(memcached_st *memc);
   test_return basic_master_key_test(memcached_st *memc);
   void *world_create(void);
   void world_destroy(void *p);
}

test_return basic_test(memcached_st *memc)
{
  Memcached foo(memc);
  const char *value_set= "This is some data";
  string value;
  size_t value_length;

  foo.set("mine", value_set, strlen(value_set));
  value= foo.get("mine", &value_length);

  assert((memcmp(value.c_str(), value_set, value_length) == 0));

  return TEST_SUCCESS;
}

uint8_t increment_test(memcached_st *memc)
{
  Memcached mcach(memc);
  memcached_return rc;
  const string key("inctest");
  const char *inc_value= "1";
  string ret_value;
  uint64_t int_inc_value;
  uint64_t int_ret_value;
  size_t value_length;

  mcach.set(key, inc_value, strlen(inc_value));
  ret_value= mcach.get(key, &value_length);
  printf("\nretvalue %s\n",ret_value.c_str());
  int_inc_value= uint64_t(atol(inc_value));
  int_ret_value= uint64_t(atol(ret_value.c_str()));
  assert(int_ret_value == int_inc_value); 

  rc= mcach.increment(key, 1, &int_ret_value);
  assert(rc == MEMCACHED_SUCCESS);
  assert(int_ret_value == 2);

  rc= mcach.increment(key, 1, &int_ret_value);
  assert(rc == MEMCACHED_SUCCESS);
  assert(int_ret_value == 3);

  rc= mcach.increment(key, 5, &int_ret_value);
  assert(rc == MEMCACHED_SUCCESS);
  assert(int_ret_value == 8);

  return 0;
}

test_return basic_master_key_test(memcached_st *memc)
{
   Memcached foo(memc);
  const char *value_set= "Data for server A";
  const string master_key_a("server-a");
  const string master_key_b("server-b");
  const string key("xyz");
  string value;
  size_t value_length;

  foo.set_by_key(master_key_a, key, value_set, strlen(value_set));
  value= foo.get_by_key(master_key_a, key, &value_length);

  assert((memcmp(value.c_str(), value_set, value_length) == 0));

  value= foo.get_by_key(master_key_b, key, &value_length);
  assert((memcmp(value.c_str(), value_set, value_length) == 0));

  return TEST_SUCCESS;
}


test_st tests[] ={
  {"basic", 0, basic_test },
  {"basic_master_key", 0, basic_master_key_test },
  {0, 0, 0}
};

collection_st collection[] ={
  {"block", 0, 0, tests},
  {0, 0, 0, 0}
};

#define SERVERS_TO_CREATE 1

extern "C" void *world_create(void)
{
  server_startup_st *construct;

  construct= (server_startup_st *)malloc(sizeof(server_startup_st));
  memset(construct, 0, sizeof(server_startup_st));

  construct->count= SERVERS_TO_CREATE;
  server_startup(construct);

  return construct;
}

void world_destroy(void *p)
{
  server_startup_st *construct= static_cast<server_startup_st *>(p);
  memcached_server_st *servers=
    static_cast<memcached_server_st *>(construct->servers);
  memcached_server_list_free(servers);

  server_shutdown(construct);
  free(construct);
}

void get_world(world_st *world)
{
  world->collections= collection;
  world->create= world_create;
  world->destroy= world_destroy;
}
