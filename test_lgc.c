/*
 * Author: shashankx86
 * License: GPL-2.0
 * Description: Test program for login-check module functionality
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define CONFIG_PATH ".lgc.conf"
#define DEFAULT_FOLDER_NAME "IN"

void load_folder_name(char *folder_name) {
    FILE *config_file = fopen(CONFIG_PATH, "r");
    char line[256];

    if (!config_file) {
        strcpy(folder_name, DEFAULT_FOLDER_NAME);
        return;
    }

    while (fgets(line, sizeof(line), config_file)) {
        if (sscanf(line, "folder_name=%s", folder_name) == 1) {
            fclose(config_file);
            return;
        }
    }
    strcpy(folder_name, DEFAULT_FOLDER_NAME);
    fclose(config_file);
}

int check_folder_exists(const char *folder_name) {
    char desktop_path[512];
    struct stat st;

    snprintf(desktop_path, sizeof(desktop_path), "%s/Desktop/%s", getenv("HOME"), folder_name);
    if (stat(desktop_path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return 1;
    }
    return 0;
}

void send_notification(const char *message) {
    char command[256];
    snprintf(command, sizeof(command), "notify-send '%s'", message);
    system(command);
}

int main() {
    char folder_name[256];
    load_folder_name(folder_name);

    printf("Checking for folder '%s' after 5 seconds...\n", folder_name);
    sleep(5);

    if (!check_folder_exists(folder_name)) {
        send_notification("Unauthorized user detected!");
    } else {
        printf("Folder exists. No notification sent.\n");
    }

    return 0;
}
