/*
 * File      : romfs.c
 * This file is part of Device File System in RT-Thread RTOS
 * COPYRIGHT (C) 2004-2011, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rtthread.h>
#ifdef RT_USING_DFS
#include <dfs_romfs.h>

const static unsigned char _info_txt[] =
{
    "V3s Rtthread Build VeiLiang\r\n"
};
extern const unsigned char nes_rom[786464];
const struct romfs_dirent _root_dirent[] =
{
    {ROMFS_DIRENT_DIR, "dev", 0, 0},
    {ROMFS_DIRENT_DIR, "flash", 0, 0},
	{ROMFS_DIRENT_DIR, "nand", 0, 0},
	{ROMFS_DIRENT_DIR, "udisk0", 0, 0},
    {ROMFS_DIRENT_DIR, "udisk1", 0, 0},
    {ROMFS_DIRENT_DIR, "udisk2", 0, 0},
    {ROMFS_DIRENT_DIR, "udisk3", 0, 0},
    {ROMFS_DIRENT_DIR, "mmc", 0, 0},
    {ROMFS_DIRENT_DIR, "ramfs", 0, 0},
    {ROMFS_DIRENT_DIR, "usb", 0, 0},
    {ROMFS_DIRENT_FILE, "info.txt", _info_txt, sizeof(_info_txt)},
    {ROMFS_DIRENT_FILE, "rexuegedou.nes", nes_rom, sizeof(nes_rom)},
};

const struct romfs_dirent romfs_root =
{
    ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_root_dirent, sizeof(_root_dirent)/sizeof(_root_dirent[0])
};


#include <dfs_fs.h>
int rt_hw_romfs_init(void)
{
    rt_kprintf("dfs_mount romfs\n");

    if(dfs_mount(RT_NULL, "/", "rom", 0, &romfs_root) < 0)
    {
        rt_kprintf("dfs_mount romfs failed %d\n",rt_get_errno());
    }

    return 0;
}
INIT_EXPORT(rt_hw_romfs_init, "4.end");
#endif
