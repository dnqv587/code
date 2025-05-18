#include <infiniband/verbs.h>
#include <infiniband/verbs_exp.h>
#include <iostream>
#include <cerrno>
#include <cstring>

struct Context {
  struct ibv_context	*context;
  struct ibv_comp_channel *channel;
  struct ibv_pd		*pd;
  struct ibv_mr		*mr;
  struct ibv_exp_dm	*dm;
  struct ibv_cq		*cq;
  struct ibv_qp		*qp;
  void			*buf;
  unsigned long long	 size;
  int			 rx_depth;
  int			 pending;
  struct ibv_port_attr	 portinfo;
  int			 inlr_recv;
};

void server() {

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
  ctx.inlr_recv = 0;

  //创建RDMA完成事件通道
  ctx.channel = ibv_create_comp_channel(ctx.context);

  //获取RDMA设备内存管理器
  auto* pd=ibv_alloc_pd(ctx.context);

  //申请并注册内存
  ctx.size = 1024;
  ctx.buf = malloc(ctx.size);
  ctx.mr = ibv_reg_mr(pd, ctx.buf, ctx.size, IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);
  memset(ctx.buf, 0x00, ctx.size);

  //创建CQ
  ctx.rx_depth = 100;
  struct ibv_comp_channel *channel;
  ctx.cq=ibv_create_cq(ctx.context, ctx.rx_depth + 1, nullptr, ctx.channel, 0);

  //创建QP
  struct ibv_exp_qp_init_attr attr = {
    .send_cq = ctx->cq,
    .recv_cq = ctx->cq,
    .cap     = {
      .max_send_wr  = 1,
      .max_recv_wr  = rx_depth,
      .max_send_sge = 1,
      .max_recv_sge = 1
},
.qp_type = IBV_QPT_RC,
.pd = ctx->pd,
.comp_mask = IBV_EXP_QP_INIT_ATTR_PD,
.max_inl_recv = ctx->inlr_recv
};
  if (ctx->inlr_recv)
    attr.comp_mask |= IBV_EXP_QP_INIT_ATTR_INL_RECV;

  ctx->qp = ibv_exp_create_qp(ctx->context, &attr);

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