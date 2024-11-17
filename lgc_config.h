// SPDX-License-Identifier: GPL-2.0
#ifndef LGC_CONFIG_H
#define LGC_CONFIG_H

#define CONFIG_PATH "/home/%s/.lgc.conf"
#define DEFAULT_FOLDER_NAME "IN"
#define MAX_FOLDER_NAME_LENGTH 256

int load_folder_name_from_config(const char *username, char *folder_name);

#endif
