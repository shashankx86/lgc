// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/slab.h>
#include "lgc_config.h"

// Load folder name from the configuration file in user space
int load_folder_name_from_config(const char *username, char *folder_name) {
    struct file *file;
    char config_path[256];
    char *buffer;
    ssize_t bytes_read;
    loff_t offset = 0;

    snprintf(config_path, sizeof(config_path), CONFIG_PATH, username);

    // Open the file
    file = filp_open(config_path, O_RDONLY, 0);
    if (IS_ERR(file)) {
        strncpy(folder_name, DEFAULT_FOLDER_NAME, MAX_FOLDER_NAME_LENGTH);
        return -1;
    }

    // Allocate a buffer to read data
    buffer = kzalloc(512, GFP_KERNEL);
    if (!buffer) {
        filp_close(file, NULL);
        return -ENOMEM;
    }

    // Read the file content
    bytes_read = kernel_read(file, buffer, 511, &offset);
    buffer[bytes_read] = '\0';

    // Parse the configuration value
    if (bytes_read > 0) {
        char *start = strstr(buffer, "folder_name=");
        if (start) {
            start += strlen("folder_name=");
            sscanf(start, "%s", folder_name);
        } else {
            strncpy(folder_name, DEFAULT_FOLDER_NAME, MAX_FOLDER_NAME_LENGTH);
        }
    }

    // Clean up
    kfree(buffer);
    filp_close(file, NULL);
    return 0;
}
