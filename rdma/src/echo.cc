#include <infiniband/verbs.h>
#include <infiniband/verbs.h>
#include <iostream>
#include <cerrno>
#include <cstring>

//上下文
struct Context {
  struct ibv_context	*context;
  struct ibv_comp_channel *channel;
  struct ibv_pd		*pd;
  struct ibv_mr		*mr;
  struct ibv_exp_dm	*dm;
  union {
    struct ibv_cq		*cq;
    struct ibv_cq_ex	*cq_ex;
  } cq_s;
  struct ibv_qp		*qp;
  struct ibv_qp_ex	*qpx;
  void			*buf;
  unsigned long long	 size;
  int			 send_flags;
  int			 rx_depth;
  int			 pending;
  struct ibv_port_attr	 portinfo;
  uint64_t		 completion_timestamp_mask;
};

//目的信息
struct dest {
  int lid;
  int qpn;
  int psn;
  union ibv_gid gid;
};

void server() {

  uint8_t ib_port = 1;

  Context ctx{}; //上下文
  //获取RDMA设备
  auto** device = ibv_get_device_list(nullptr);
  ctx.context = ibv_open_device(device[0]);
  struct ibv_device_attr attr{};
  //获取RDMA设备属性
  if (const int err=ibv_query_device(ctx.context,&attr);err != 0) {
    std::cerr<<std::strerror(err)<<std::endl;
  }

  //暂不内联接收
  // ctx.inlr_recv = 0;

  //创建RDMA完成事件通道
  ctx.channel = ibv_create_comp_channel(ctx.context);

  //获取RDMA设备内存管理器
  auto* pd=ibv_alloc_pd(ctx.context);

  //申请并注册内存
  ctx.size = 1024;
  ctx.buf = malloc(ctx.size);
  ctx.mr = ibv_reg_mr(pd, ctx.buf, ctx.size, IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);
  memset(ctx.buf, 0x00, ctx.size);

  //创建CQ -- ibv_create_cq_ex支持存放完成时间
  ctx.rx_depth = 100;
  struct ibv_comp_channel *channel;
  ctx.cq_s.cq = ibv_create_cq(ctx.context, ctx.rx_depth + 1, nullptr, ctx.channel, 0);

  //创建QP
  struct ibv_qp_attr qp_attr{};
  struct ibv_qp_init_attr init_attr = {
      .send_cq = ctx.cq_s.cq,
      .recv_cq = ctx.cq_s.cq,
      .cap = {
          .max_send_wr = 1,
          .max_recv_wr = 100,
          .max_send_sge = 1,
          .max_recv_sge = 1,
        .max_inline_data = 0,
      },
      .qp_type = IBV_QPT_RC
  };
  ctx.qp = ibv_create_qp(ctx.pd, &init_attr);
  ibv_query_qp(ctx.qp, &qp_attr, IBV_QP_CAP, &init_attr);
  if (init_attr.cap.max_inline_data >= ctx.size) {
    ctx.send_flags |= IBV_SEND_INLINE;
  }

  struct ibv_qp_attr qp_attr2 = {
    .qp_state        = IBV_QPS_INIT,
    .pkey_index      = 0,
    .port_num        = ib_port,
    .qp_access_flags = 0
};

  ibv_modify_qp(ctx.qp, &qp_attr2,
                IBV_QP_STATE |
                IBV_QP_PKEY_INDEX |
                IBV_QP_PORT |
                IBV_QP_ACCESS_FLAGS);

//发布接收任务
  {
    struct ibv_sge list = {
        .addr = reinterpret_cast<uintptr_t>(ctx.buf),
        .length = static_cast<uint32_t>(ctx.size),
        .lkey = ctx.mr->lkey
    };
    struct ibv_recv_wr wr = {
        .wr_id = 1,
        .sg_list = &list,
        .num_sge = 1,
    };
    struct ibv_recv_wr *bad_wr;
    int i;

    for (i = 0; i < ctx.rx_depth; ++i)
      if (ibv_post_recv(ctx.qp, &wr, &bad_wr))
        break;
  }

  //开启事件通知
  ibv_req_notify_cq(ctx.cq_s.cq,0);

  //获取端口信息
  ibv_query_port(ctx.context, ib_port, &ctx.portinfo);

  //目的信息
  dest my_dest{};

  my_dest.lid = ctx.portinfo.lid;
  if (ctx.portinfo.link_layer != IBV_LINK_LAYER_ETHERNET &&
      !my_dest.lid) {
    fprintf(stderr, "Couldn't get local LID\n");
  }

  ibv_query_gid(ctx.context, ib_port, gidx, &my_dest.gid)

clean:
  ibv_destroy_qp(ctx.qp);
  ibv_destroy_comp_channel(ctx.channel);
  free(ctx.buf);
  ibv_dereg_mr(ctx.mr);
  ibv_dealloc_pd(pd);
  ibv_close_device(ctx.context);
  ibv_free_device_list(device);
}

int main() {
  return 0;
}