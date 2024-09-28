/* GPL HEADER START
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 only,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License version 2 for more details (a copy is included
 * in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; If not, see
 * http://www.gnu.org/licenses/gpl-2.0.html
 *
 * GPL HEADER END
 */
/*
 * Copyright (c) 2007, 2010, Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 *
 * Copyright (c) 2011, 2014, Intel Corporation.
 */
/*
 * This file is part of Lustre, http://www.lustre.org/
 */

#ifndef __OBD_TARGET_H
#define __OBD_TARGET_H
#include <lprocfs_status.h>
#include <obd.h>

/* server-side individual type definitions */

#define OBT_MAGIC       0xBDDECEAE
/* hold common fields for "target" device */
struct obd_device_target {
	/* 魔法数字，用于验证结构体的正确性 */
	__u32			obt_magic;
	/* 实例编号，标识特定的目标设备实例 */
	__u32			obt_instance;
	/* 指向逻辑目标设备的指针，用于关联逻辑设备信息 */
	struct lu_target       *obt_lut;
	/* 目标设备的挂载次数，用于跟踪设备使用情况 */
	__u64			obt_mount_count;
	/* 目标设备的工作统计信息，用于性能监控和分析 */
	struct obd_job_stats	obt_jobstats;
	/* 指向节点映射配置文件的指针，用于网络通信配置 */
	struct nm_config_file	*obt_nodemap_config_file;
};

#define OBJ_SUBDIR_COUNT 32 /* set to zero for no subdirs */

struct echo_obd {
	struct obd_device_target	eo_obt;
	struct obdo			eo_oa;
	spinlock_t			eo_lock;
	u64				eo_lastino;
	struct lustre_handle		eo_nl_lock;
	atomic_t			eo_prep;
};

struct ost_obd {
	struct ptlrpc_service	*ost_service;
	struct ptlrpc_service	*ost_create_service;
	struct ptlrpc_service	*ost_io_service;
	struct ptlrpc_service	*ost_seq_service;
	struct ptlrpc_service	*ost_out_service;
	struct mutex		 ost_health_mutex;
};

static inline struct obd_device_target *obd2obt(struct obd_device *obd)
{
	struct obd_device_target *obt;

	BUILD_BUG_ON(sizeof(obd->u) < sizeof(*obt));

	if (!obd)
		return NULL;
	obt = (void *)&obd->u;
	LASSERT(obt->obt_magic == OBT_MAGIC);
	return obt;
}

static inline struct obd_device_target *obd_obt_init(struct obd_device *obd)
{
	struct obd_device_target *obt;

	obt = (void *)&obd->u;
	obt->obt_magic = OBT_MAGIC;
	obt->obt_instance = 0;

	return obt;
}

static inline struct echo_obd *obd2echo(struct obd_device *obd)
{
	struct echo_obd *echo;

	BUILD_BUG_ON(sizeof(obd->u) < sizeof(*echo));

	if (!obd)
		return NULL;
	echo = (void *)&obd->u;

	return echo;
}

static inline struct ost_obd *obd2ost(struct obd_device *obd)
{
	struct ost_obd *ost;

	BUILD_BUG_ON(sizeof(obd->u) < sizeof(*ost));

	if (!obd)
		return NULL;
	ost = (void *)&obd->u;

	return ost;
}

#endif /* __OBD_TARGET_H */
