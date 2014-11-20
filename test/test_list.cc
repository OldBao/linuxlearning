#include "ll_list.h"
#include "test_list.h"

typedef struct ll_list_container_s ll_list_container_t;
typedef struct ll_list_sb_s ll_list_sb_t;

//super block
struct ll_list_sb_s {
  ll_list_node_t head;
};

struct ll_list_container_s {
  int tag;

  ll_list_node_t node;
};

//测试插入大量的数据
TEST_F(LLListTest, TestAppendMass) {
  const int NCONTAINER = 10000;
  int   i;

  ll_list_sb_t sb;

  ll_list_init(&sb.head);

  ll_list_container_t containers[NCONTAINER];

  for (i = 0; i < NCONTAINER; i++) {
    containers[i].tag = i;;
    ll_list_append(&sb.head, &containers[i].node);
  }

  ll_list_node_t *pnode = ll_list_head(&sb.head);
  ll_list_container_t *pcontainer;
  i = 0;

  while(pnode != ll_list_end(&sb.head)) {
    pcontainer = ll_list_item(pnode, ll_list_container_t, node);
    ASSERT_EQ(pcontainer->tag, i++);
    pnode = ll_list_next(pnode);
  }

  //test foreach
  i = 0;
  ll_list_foreach_ex(&sb.head, pcontainer, ll_list_container_t, node) {
    ASSERT_EQ(pcontainer->tag, i);
    i++;
  }


  ll_list_node_t *tmp;

  pnode = ll_list_head(&sb.head);
  while(pnode != ll_list_end(&sb.head)) {
    tmp = pnode;
    pnode = ll_list_next(pnode);
    ll_list_remove(tmp);
  }

  ASSERT_TRUE(ll_list_empty(&sb.head));
}
