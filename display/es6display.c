#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/io.h>
#include <mach/hardware.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/mutex.h>

#include "es6display.h"

#define DEVICE_NAME "display"
#define MAJOR_NR 252

#define DISPLAY_DIGIT_COUNT 8
#define DISPLAY_MESSAGE_SIZE_MAX 64

#define LCD_CFG 0x40004054
#define P2_MUX_SET 0x40028028
#define P2_MUX_CLR 0x4002802c
#define P2_MUX_SET_VALUE (1 << 3)

#define P2_INP_STATE 0x4002801C
#define P2_OUT_SET 0x40028020
#define P2_OUT_CLR 0x40028024
#define P2_DIR_SET 0x40028010
#define P2_DIR_CLR 0x40028014
#define P2_DIR_STATE 0x40028018

// j3 pins 45, 57, 58 respectively
#define DISPLAY_SER_BIT_OFFSET 7
#define DISPLAY_RCLK_BIT_OFFSET 3
#define DISPLAY_SRCLK_BIT_OFFSET 5

static char display_message[DISPLAY_MESSAGE_SIZE_MAX];
static struct task_struct *display_thread;
static int display_scroll_index;
static bool reset_display_scroll_index;

void set_gpio_state(u32 bit_offset, int value);
void write_display_string(char *input);
void thread_function(void *pv);
void stop_thread_if_necessary(void);

DEFINE_MUTEX(lock);

static void unlock_mutex(void)
{
    if (mutex_is_locked(&lock) == 1)
    {
        mutex_unlock(&lock);
    }
    else
    {
        printk(KERN_INFO DEVICE_NAME ": tried unlocking unlocked mutex\n");
    }
}

static int device_open(struct inode *inode, struct file *file)
{
    file->private_data = (void *)MINOR(inode->i_rdev);
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);
    return 0;
}

void stop_thread_if_necessary(void)
{
    if (display_thread)
    {
        if (!kthread_stop(display_thread))
        {
            printk(KERN_INFO DEVICE_NAME ": Could not stop display thread.\n");
        }
        display_thread = NULL;
    }
}

static ssize_t
device_write(struct file *file,
             const char __user *buffer, size_t length, loff_t *offset)
{
    int i;

    mutex_lock(&lock);
    for (i = 0; (i < length) && (i < (DISPLAY_MESSAGE_SIZE_MAX - 1)); i++)
    {
        get_user(display_message[i], buffer + i);
    }
    display_message[i] = '\0';
    reset_display_scroll_index = true;
    unlock_mutex();

    return i;
}

static struct file_operations fops =
    {
        .owner = THIS_MODULE,
        .write = device_write,
        .open = device_open,
        .release = device_release};

void write_display_string(char *input)
{
    int i;
    int j;
    int end_of_string_reached;

    set_gpio_state(DISPLAY_RCLK_BIT_OFFSET, 0);
    end_of_string_reached = 0;
    for (i = 0; i < DISPLAY_DIGIT_COUNT; i++)
    {
        char c;
        char sanitized_char;
        c = input[i];

        // Lower case into upper case
        if (c >= 'a' && c <= ('a' + ALPHABET_LENGTH))
        {
            c -= ('a' - 'A');
        }

        if (c >= '0' && c <= '9')
        {
            sanitized_char = ((digits[c - '0']));
        }
        else if (c >= 'A' && c <= ('A' + ALPHABET_LENGTH))
        {
            sanitized_char = ((letters[c - 'A']));
        }
        else
        {
            sanitized_char = SEVENDIGIT_NONE;
        }

        if (c == 0)
        {
            end_of_string_reached = 1;
        }
        if (end_of_string_reached)
        {
            sanitized_char = SEVENDIGIT_NONE;
        }

        for (j = 0; j < 8; j++)
        {
            set_gpio_state(DISPLAY_SRCLK_BIT_OFFSET, 0);
            set_gpio_state(DISPLAY_SER_BIT_OFFSET, ((sanitized_char >> j) & 0x01));
            set_gpio_state(DISPLAY_SRCLK_BIT_OFFSET, 1);
        }
    }
    set_gpio_state(DISPLAY_RCLK_BIT_OFFSET, 1);
}

void thread_function(void *pv)
{
    while (!kthread_should_stop())
    {
        int overflow;
        char trimmed_message[DISPLAY_DIGIT_COUNT + 1];
        mutex_lock(&lock);
        if (reset_display_scroll_index)
        {
            display_scroll_index = 0;
            reset_display_scroll_index = false;
        }
        strncpy(trimmed_message, (display_message + display_scroll_index), DISPLAY_DIGIT_COUNT);
        unlock_mutex();
        write_display_string(trimmed_message);

        overflow = (strlen(display_message) - 1) - DISPLAY_DIGIT_COUNT;
        if (display_scroll_index < overflow)
        {
            display_scroll_index++;
        }
        else
        {
            display_scroll_index = 0;
        }
        ssleep(1);
    }
}

void initialize_display_io(void)
{
    iowrite32(0x1 << DISPLAY_SER_BIT_OFFSET, io_p2v(P2_DIR_SET));
    iowrite32(0x1 << DISPLAY_RCLK_BIT_OFFSET, io_p2v(P2_DIR_SET));
    iowrite32(0x1 << DISPLAY_SRCLK_BIT_OFFSET, io_p2v(P2_DIR_SET));
    set_gpio_state(DISPLAY_SER_BIT_OFFSET, 0);
    set_gpio_state(DISPLAY_RCLK_BIT_OFFSET, 1);
    set_gpio_state(DISPLAY_SRCLK_BIT_OFFSET, 0);
}

void set_gpio_state(u32 bit_offset, int value)
{
    if (value)
    {
        iowrite32(0x1 << bit_offset, io_p2v(P2_OUT_SET));
    }
    else
    {
        iowrite32(0x1 << bit_offset, io_p2v(P2_OUT_CLR));
    }
}

int display_init(void)
{
    int result;
    result = register_chrdev(MAJOR_NR, DEVICE_NAME, &fops);
    if(result < 0)
    {
        printk(KERN_INFO DEVICE_NAME ": Error registering device");
        return result;
    }

    iowrite32(0, io_p2v(LCD_CFG));
    iowrite32(P2_MUX_SET_VALUE, io_p2v(P2_MUX_SET));

    initialize_display_io();
    reset_display_scroll_index = false;
    display_thread = kthread_run(thread_function, NULL, "display thread");
    if (IS_ERR(display_thread))
    {
        printk(KERN_INFO DEVICE_NAME ": Thread creation failed: '%d'\n", (int)display_thread);
    }

    return 0;
}

void cleanup_module()
{
    stop_thread_if_necessary();

    unregister_chrdev(MAJOR_NR, DEVICE_NAME);

    iowrite32(P2_MUX_SET_VALUE, io_p2v(P2_MUX_CLR));
}

module_init(display_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ajvreugdenhil");
MODULE_DESCRIPTION("display driver");
