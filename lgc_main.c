// SPDX-License-Identifier: GPL-2.0
/*
 * Author: shashankx86
 * Description: Login Checker Kernel Module with Notification
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include "lgc_config.h"

#define NETLINK_USER 31

MODULE_LICENSE("GPL");
MODULE_AUTHOR("shashankx86");
MODULE_DESCRIPTION("Login Checker with Notification");
MODULE_VERSION("1.0");

struct sock *nl_sk = NULL;
static struct task_struct *task;
static char folder_name[MAX_FOLDER_NAME_LENGTH] = DEFAULT_FOLDER_NAME;
static char desktop_path[512];
static char username[256];

static void send_notification(const char *message) {
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    int pid = 0;
    int msg_size = strlen(message);

    skb = nlmsg_new(msg_size, 0);
    if (!skb) return;

    nlh = nlmsg_put(skb, pid, 0, NLMSG_DONE, msg_size, 0);
    strncpy(nlmsg_data(nlh), message, msg_size);

    nlmsg_unicast(nl_sk, skb, pid);
}

static bool check_folder_exists(const char *path) {
    struct file *file;
    mm_segment_t old_fs;
    bool exists = false;

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    file = filp_open(path, O_RDONLY, 0);

    if (!IS_ERR(file)) {
        exists = true;
        filp_close(file, NULL);
    }

    set_fs(old_fs);
    return exists;
}

static int lgc_check_thread(void *data) {
    ssleep(5);

    snprintf(desktop_path, sizeof(desktop_path), "/home/%s/Desktop/%s", username, folder_name);
    if (!check_folder_exists(desktop_path)) {
        send_notification("Unauthorized user detected!");
    }
    return 0;
}

static int __init lgc_init(void) {
    snprintf(username, sizeof(username), "%d", current_uid().val);
    load_folder_name_from_config(username, folder_name);
    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, NULL);
    kthread_run(lgc_check_thread, NULL, "lgc_thread");
    return 0;
}

static void __exit lgc_exit(void) {
    netlink_kernel_release(nl_sk);
}

module_init(lgc_init);
module_exit(lgc_exit);
