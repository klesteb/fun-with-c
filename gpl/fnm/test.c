
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include "vperror.h"
#include "fnm_util.h"

int main(void) {


    char *unix_path1 = "/d2/ssg_dev/source/cclibs/fnm/fnm_unix2vms.c";
    char *unix_path2 = "/disk$system/ssg_dev/source/cclibs/fnm/fnm_unix2vms.c";
    char *unix_path3 = "trex:/disk$system/ssg_dev/source/cclibs/fnm/fnm_unix2vms.c.001";
    char *syslogin = NULL;
    char *disk = NULL;
    char vms_path[1024];
    FileName fname;

    fnm_unix2vms(unix_path1, vms_path, sizeof(vms_path) - 1);
    printf("unix = %s \n vms = %s\n", unix_path1, vms_path);

    fnm_unix2vms(unix_path2, vms_path, sizeof(vms_path) - 1);
    printf("unix = %s \n vms = %s\n", unix_path2, vms_path);

    fnm_unix2vms(unix_path3, vms_path, sizeof(vms_path) - 1);
    printf("unix = %s \n vms = %s\n", unix_path3, vms_path);

    syslogin = getenv("sys$login");
    printf("sys$login: = %s\n", syslogin);

    disk = getenv("d0");
    printf("disk = %s\n", disk);

    getcwd(vms_path, sizeof(vms_path) - 1);
    printf("pwd = %s\n", vms_path);

    fname = fnm_create(FALSE, "test", ".c", ".1", "junk/", vms_path, NULL);
    printf("path = %s\n", fnm_path(fname));
    fnm_destroy(fname);

}

