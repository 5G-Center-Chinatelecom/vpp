/*
 *------------------------------------------------------------------
 * Copyright (c) 2018 Cisco and/or its affiliates.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *------------------------------------------------------------------
 */

#ifndef _AF_XDP_H_
#define _AF_XDP_H_

#include <vlib/log.h>

#define foreach_af_xdp_device_flags \
  _(0, INITIALIZED, "initialized") \
  _(1, ERROR, "error") \
  _(2, ADMIN_UP, "admin-up") \
  _(3, VA_DMA, "vaddr-dma") \
  _(4, LINK_UP, "link-up") \
  _(5, SHARED_TXQ_LOCK, "shared-txq-lock") \
  _(6, ELOG, "elog")

enum
{
#define _(a, b, c) AF_XDP_DEVICE_F_##b = (1 << a),
  foreach_af_xdp_device_flags
#undef _
};

typedef struct
{
  CLIB_CACHE_LINE_ALIGN_MARK (cacheline0);
  u32 ifindex;
  u8 *ifname;
  struct xsk_socket_info *xsk;

  u32 flags;
  u32 per_interface_next_index;
  u32 clib_file_index;
  u8 is_admin_up;
  u8 custom_xdp;

  u32 dev_instance;
  u32 sw_if_index;
  u32 hw_if_index;

  u8 hwaddr[6];

  /* error */
  clib_error_t *error;
} af_xdp_device_t;

typedef struct
{
  CLIB_CACHE_LINE_ALIGN_MARK (cacheline0);

  af_xdp_device_t *devices;
  /* bitmap of pending rx interfaces */
  uword *pending_input_bitmap;
  /* rx buffer cache */
  u32 **rx_buffers;
  vlib_log_class_t log_class;
} af_xdp_main_t;

extern af_xdp_main_t af_xdp_main;

typedef struct
{
  u8 *ifname;
  u32 key;
  u8 custom_xdp;
  u32 queue_id;
  u8 force_zerocopy_bind;
  u8 force_copy_bind;

  /* return */
  int rv;
  u32 sw_if_index;
  clib_error_t *error;
} af_xdp_create_if_args_t;

void af_xdp_create_if (vlib_main_t * vm, af_xdp_create_if_args_t * args);
void af_xdp_delete_if (vlib_main_t * vm, af_xdp_device_t * ad);

extern vlib_node_registration_t af_xdp_input_node;
extern vnet_device_class_t af_xdp_device_class;

/* format.c */
format_function_t format_af_xdp_device;
format_function_t format_af_xdp_device_name;
format_function_t format_af_xdp_input_trace;

typedef struct
{
  u32 next_index;
  u32 hw_if_index;
} af_xdp_input_trace_t;

#endif /* _AF_XDP_H_ */

/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */
