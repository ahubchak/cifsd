// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *   Copyright (C) 2018 Samsung Electronics Co., Ltd.
 */

#ifndef __SHARE_CONFIG_MANAGEMENT_H__
#define __SHARE_CONFIG_MANAGEMENT_H__

#include <linux/workqueue.h>
#include <linux/hashtable.h>
#include <linux/path.h>

#include "../glob.h"  /* FIXME */

struct cifsd_share_config {
	char			*name;
	char			*path;

	unsigned int		flags;
	struct list_head	veto_list;

	struct path		vfs_path;

	atomic_t		refcount;
	struct hlist_node	hlist;
	struct work_struct	free_work;
	int			create_mask;
	int			directory_mask;
};

static inline int share_config_create_mask(struct cifsd_share_config *share)
{
	return share->create_mask;
}

static inline int share_config_directory_mask(struct cifsd_share_config *share)
{
	return share->directory_mask;
}

static inline int test_share_config_flag(struct cifsd_share_config *share,
					 int flag)
{
	return share->flags & flag;
}

extern void __cifsd_share_config_put(struct cifsd_share_config *share);

static inline void cifsd_share_config_put(struct cifsd_share_config *share)
{
	if (!atomic_dec_and_test(&share->refcount))
		return;
	__cifsd_share_config_put(share);
}

struct cifsd_share_config *cifsd_share_config_get(char *name);
bool cifsd_share_veto_filename(struct cifsd_share_config *share,
			       const char *filename);
void cifsd_share_configs_cleanup(void);

#endif /* __SHARE_CONFIG_MANAGEMENT_H__ */
