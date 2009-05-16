/*
 * vmfs-tools - Tools to access VMFS filesystems
 * Copyright (C) 2009 Christophe Fillot <cf@utc.fr>
 * Copyright (C) 2009 Mike Hommey <mh@glandium.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef VMFS_FS_H
#define VMFS_FS_H

#include <stddef.h>

/* === FS Info === */
#define VMFS_FSINFO_BASE   0x0200000
#define VMFS_FSINFO_MAGIC  0x2fabf15e

struct vmfs_fsinfo_raw {
   uint32_t magic;
   uint32_t volver;
   u_char ver;
   uuid_t uuid;
   uint32_t _unknown0;
   char label[128];
   uint32_t dev_blocksize;
   uint64_t blocksize;
   uint32_t ctime; /* ctime? in seconds */
   uint32_t _unknown3;
   uuid_t lvm_uuid;
   u_char _unknown4[16];
   uint32_t fdc_header_size;
   uint32_t fdc_bitmap_count;
   uint32_t subblock_size;
} __attribute__((packed));

#define VMFS_FSINFO_OFS_MAGIC    offsetof(struct vmfs_fsinfo_raw, magic)
#define VMFS_FSINFO_OFS_VOLVER   offsetof(struct vmfs_fsinfo_raw, volver)
#define VMFS_FSINFO_OFS_VER      offsetof(struct vmfs_fsinfo_raw, ver)
#define VMFS_FSINFO_OFS_UUID     offsetof(struct vmfs_fsinfo_raw, uuid)
#define VMFS_FSINFO_OFS_LABEL    offsetof(struct vmfs_fsinfo_raw, label)
#define VMFS_FSINFO_OFS_BLKSIZE  offsetof(struct vmfs_fsinfo_raw, blocksize)
#define VMFS_FSINFO_OFS_LVM_UUID offsetof(struct vmfs_fsinfo_raw, lvm_uuid)
#define VMFS_FSINFO_OFS_SBSIZE   offsetof(struct vmfs_fsinfo_raw, subblock_size)

#define VMFS_FSINFO_OFS_FDC_HEADER_SIZE \
   offsetof(struct vmfs_fsinfo_raw, fdc_header_size)

#define VMFS_FSINFO_OFS_FDC_BITMAP_COUNT \
   offsetof(struct vmfs_fsinfo_raw, fdc_bitmap_count)

#define VMFS_FSINFO_OFS_LABEL_SIZE sizeof(((struct vmfs_fsinfo_raw *)(0))->label)

struct vmfs_fsinfo {
   uint32_t magic;
   uint32_t vol_version;
   uint32_t version;
   uuid_t uuid;
   char *label;

   uint64_t block_size;
   uint32_t subblock_size;

   uint32_t fdc_header_size;
   uint32_t fdc_bitmap_count;

   uuid_t lvm_uuid;
};

/* === VMFS filesystem === */
struct vmfs_fs {
   int debug_level;

   /* FS information */
   vmfs_fsinfo_t fs_info;

   /* Associated VMFS Volume */
   vmfs_lvm_t *lvm;

   /* Meta-files containing file system structures */
   vmfs_bitmap_t *fbb,*fdc,*pbc,*sbc;

   /* Root directory */
   vmfs_file_t *root_dir;
};

/* Get block size of a volume */
static inline uint64_t vmfs_fs_get_blocksize(const vmfs_fs_t *fs)
{
   return(fs->fs_info.block_size);
}

/* Read a block from the filesystem */
ssize_t vmfs_fs_read(const vmfs_fs_t *fs,uint32_t blk,off_t offset,
                     u_char *buf,size_t len);

/* Write a block to the filesystem */
ssize_t vmfs_fs_write(const vmfs_fs_t *fs,uint32_t blk,off_t offset,
                      const u_char *buf,size_t len);

/* Show FS information */
void vmfs_fs_show(const vmfs_fs_t *fs);

/* Dump volume bitmaps */
int vmfs_fs_dump_bitmaps(const vmfs_fs_t *vol);

/* Create a FS structure */
vmfs_fs_t *vmfs_fs_create(vmfs_lvm_t *lvm);

/* Open a FS */
int vmfs_fs_open(vmfs_fs_t *fs);

/* Close a FS */
void vmfs_fs_close(vmfs_fs_t *fs);

#endif
