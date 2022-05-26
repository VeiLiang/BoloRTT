#include <rtthread.h>
#ifdef RT_USING_DFS
#include "dfs_ramfs.h"


rt_uint8_t rampool[6*1024*1024];

int mnt_ram_elminit(void)
{
   if(dfs_mount (RT_NULL,"/ramfs","ram",0,dfs_ramfs_create(rampool, sizeof(rampool))) == 0)
   {
       rt_kprintf("ram file system initializated;\n");
   }
   else
   {
        rt_kprintf("ram file system initializate failed;\n");
   }
   return 0;
}
INIT_ENV_EXPORT(mnt_ram_elminit);

#endif