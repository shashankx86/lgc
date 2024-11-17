# lgc (Login Check Kernel Module)

`lgc` is a Linux kernel module that checks if a specific folder (e.g., `IN`) exists on the desktop when a user logs in. If the folder is not found within 5 seconds, the system will notify that an unauthorized user has been detected.

## Why?
A certain someone always seems to be able to read my key type and guess what I'm typing. This module was created to tackle that issue.
~~I sell drugs~~

## Features

- Monitors user login.
- Checks for the existence of a specific folder (`IN` by default) on the user's desktop.
- Sends a system notification if the folder is not found.
- Configurable folder name from a `.lgc.conf` file in the user's home directory.
- The module will issue a warning notification if no folder is found within 5 seconds.

## Prerequisites

- A Linux machine running kernel version 5.10 or above (6.x preferred).
- Development tools like `gcc`, `make`, and the kernel headers for your specific kernel version.

## Installation

### Step 1: Clone the repository

Clone this repository to your local machine.

```bash
git clone https://github.com/shashankx86/lgc.git
cd lgc
```

### Step 2: Install kernel headers

Ensure that you have the correct kernel headers installed. You can install the headers for your current kernel version with the following command:

For **Debian/Ubuntu**:

```bash
sudo apt update
sudo apt install linux-headers-$(uname -r)
```

For **Fedora/CentOS**:

```bash
sudo dnf install kernel-devel-$(uname -r)
```

For **Arch Linux**:

```bash
sudo pacman -S linux-headers
```

### Step 3: Build the Kernel Module

Run the `make` command to build the kernel module:

```bash
make
```

This will compile the kernel module and create the `.ko` file.

### Step 4: Load the Module

After the kernel module is built, you can load it into the kernel:

```bash
sudo insmod lgc.ko
```

You can verify that the module has been loaded:

```bash
lsmod | grep lgc
```

To remove the module:

```bash
sudo rmmod lgc
```

### Step 5: Configure the Folder Name

The folder name can be configured from a `.lgc.conf` file located in the user's home directory. This file should contain the desired folder name, e.g.,:

```text
folder_name=IN
```

The default folder is `IN`, but you can change it by editing this file.

### Step 6: Testing the Module

Once the module is loaded and the configuration is in place, you can test the functionality by logging into your desktop as a user.

1. **Login as a user** and check if the folder (`IN`) exists on the Desktop.
2. **If the folder is missing**, the kernel module will issue a notification and log an alert that an unauthorized user has been detected.

You can also compile and run a user-space test program using:

```bash
gcc -o test_lgc test_lgc.c
./test_lgc
```

### Step 7: Clean the Build

To clean up the compiled files:

```bash
make clean
```

## How it Works

1. **User Login Detection**: The module hooks into the user login process.
2. **Folder Check**: It checks if the folder (`IN` by default) exists on the user's desktop. If the folder is missing, the system issues a notification.
3. **Configuration**: The folder name can be customized via a `.lgc.conf` file in the user's home directory.

## Troubleshooting

- If the folder is not found, ensure that the `.lgc.conf` file exists in the home directory and is correctly configured.
- Ensure that you are running a compatible kernel (5.10+).
- If there are issues with the module loading, check the kernel logs with `dmesg` for any error messages related to the module.

## License

This project is licensed under the **GPL-2.0** License - see the [LICENSE](LICENSE) file for details.

---
