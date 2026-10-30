/**
 * @file fs_inspector.c
 * @brief Implementation of inode and file status inspection routines using POSIX stat/lstat APIs.
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/fs_inspector.h"

int run_filesystem_demo(void) {
    printf("\n=======================================================\n");
    printf("   FILESYSTEM & INODE INSPECTION EXPERIMENT            \n");
    printf("=======================================================\n");

    const char *target_file = "test_target.txt";
    const char *hard_link = "test_hard.link";
    const char *sym_link = "test_sym.link";

    /* 1. Create target file */
    int fd = open(target_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to create target file");
        return -1;
    }
    write(fd, "VFS Inode Experiment Data\n", 26);
    close(fd);

    /* 2. Create Hard Link and Symbolic Link */
    unlink(hard_link);
    unlink(sym_link);

    if (link(target_file, hard_link) == -1) {
        perror("Failed to create hard link");
    }
    if (symlink(target_file, sym_link) == -1) {
        perror("Failed to create symlink");
    }

    /* 3. Inspect using stat() and lstat() */
    struct stat st_target, st_hard, st_sym;

    stat(target_file, &st_target);
    stat(hard_link, &st_hard);
    lstat(sym_link, &st_sym);

    printf("[File Inspection Results]\n");
    printf("  Target File : Inode = %lu, Hard Links = %lu, Size = %ld bytes\n",
           (unsigned long)st_target.st_ino, (unsigned long)st_target.st_nlink, (long)st_target.st_size);
    printf("  Hard Link   : Inode = %lu, Hard Links = %lu (Same Inode as Target!)\n",
           (unsigned long)st_hard.st_ino, (unsigned long)st_hard.st_nlink);
    printf("  Symlink     : Inode = %lu, Hard Links = %lu (Different Inode / Pointer!)\n",
           (unsigned long)st_sym.st_ino, (unsigned long)st_sym.st_nlink);

    /* Clean up temporary files */
    unlink(target_file);
    unlink(hard_link);
    unlink(sym_link);

    printf("Filesystem & Inode Experiment Completed Successfully.\n");
    printf("=======================================================\n");

    return 0;
}