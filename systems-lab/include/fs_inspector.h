/**
 * @file fs_inspector.h
 * @brief Interfaces for file system inspection, inode tracking, and link analysis.
 */

#ifndef FS_INSPECTOR_H
#define FS_INSPECTOR_H

/**
 * @brief Demonstrates file status inspection using stat(), comparing inodes
 *        between hard links and symbolic links.
 * 
 * @return 0 on success, -1 on error.
 */
int run_filesystem_demo(void);

#endif /* FS_INSPECTOR_H */