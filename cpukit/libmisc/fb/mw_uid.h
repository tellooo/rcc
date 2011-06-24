/**
 * @file rtems/mw_uid.h
 *
 * This file defines the interface for input devices used by MicroWindows
 * in an embedded system environment.
 */

/*
 * Copyright (c) 2000 - Rosimildo da Silva
 *
 * $Id$
 */

#ifndef _MW_UID_H
#define _MW_UID_H

#include <sys/types.h>
#include <rtems/bspIo.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 0x41XX  -- IOCTL functions for the Micro Input Devices commands */
#define MW_UID_REGISTER_DEVICE     0x4100
#define MW_UID_UNREGISTER_DEVICE   0x4101

/* devices supported by MicroWindows */
enum MW_INPUT_DEVICE_TYPE {
  MV_UID_INVALID   = 0,
  MV_UID_REL_POS   = 1,   /* mouse        */
  MV_UID_ABS_POS   = 2,   /* touch-screen */
  MV_UID_KBD       = 3,   /* keyboard     */
  MV_UID_TIMER     = 4    /* timer -- not used */
};

/* matching MicroWindows */
#define MV_BUTTON_RIGHT                 0x01
#define MV_BUTTON_CENTER                0x02
#define MV_BUTTON_LEFT                  0x04

/* modifiers of the keyboard type devices */
#define MV_KEY_MODIFIER_SHIFT_DOWN      0x10
#define MV_KEY_MODIFIER_ALT_DOWN        0x20

/* indication of the LEDS */
#define MV_KEY_MODIFIER_CAPS_ON         0x04
#define MV_KEY_MODIFIER_NUN_LOCK_ON     0x02
#define MV_KEY_SCROLL_LOCK_ON           0x01

/* keyboard modes -- default ASCII     */
#define MV_KEY_MODE_ASCII               0x01
/*
 * This mode one event is sent when a key is pressed,
 * and another one is send when a key is released.
 */
#define MV_KEY_MODE_SCANCODE            0x00

/* these defines match with the linux keyboard range
 * for ioctls functions for the keyboard interface.
 * 0x4BXX --- keyboard related functions
 */
#define MV_KDGKBMODE  0x4B44   /* gets current keyboard mode */
#define MV_KDSKBMODE  0x4B45   /* sets current keyboard mode */

/*
 * Message generated by input devices controlled by MicroWindows.
 */
struct MW_UID_MESSAGE {
  enum MW_INPUT_DEVICE_TYPE type;  /* device type */
  union {
    /* fired when keyboard events are raised */
    struct kbd_t {
      unsigned short code;        /* keycode or scancode        */
      unsigned char  modifiers;   /* key modifiers              */
      unsigned char  mode;        /* current Kbd mode           */
    } kbd;

    /* fired when position events are raised, mouse, touch screen, etc */
    struct pos_t {
      unsigned short btns; /* indicates which buttons are pressed */
      short x;             /* x location */
      short y;             /* y location */
      short z;             /* z location, 0 for 2D */
    } pos;

    /* fired by a timer device periodically */
    struct timer_t {
      unsigned long  frt;   /* free running timer */
      unsigned long  seq;   /* sequence number */
    } tmr;
  } m;
};


/*
 * API for creating/closing/accessing the message queue used by the micro
 * input device interface. All functions in this interface returns a
 * zero ( 0 ) on success. One exception for that is the "read" routine
 * that returns the number of bytes read. Negaive numbers indicate errors
 *
 * The implementation of the message queue for RTEMS uses a POSIX message
 * queue interface. It should be very portable among systems with a POSIX
 * support.
 */

/**
 *  This method creates the message queue that holds events from the
 *  input devices.
 *
 *  @param[in] q_name is the name of the message queue
 *  @param[in] flags controls the behaviour of the queue
 *  @param[in] max_msgs specifies the maximum number of pending messages
 *
 *  @note The message queue is from the Classic API.
 *
 *  @return This method returns 0 on success and -1 on error.
 */
extern int uid_open_queue( const char *q_name, int flags, size_t max_msgs );

/**
 *  This method closes the message queue and deletes it.
 *
 *  @return This method returns 0 on success and -1 on error.
 */
extern int uid_close_queue( void );

/**
 *  This method reads a message from the queue. It waits up to the specified
 *  timeout in miliseconds. A @a timeout of 0 is a poll.
 *
 *  @param[in] m will be filled in with the received message
 *  @param[in] timeout is the maximum number of mulliseconds to wait
 *
 *  @return This method returns 0 on success and -1 on error.
 */
extern int uid_read_message( struct MW_UID_MESSAGE *m, unsigned long timeout );

/**
 *  This methods writes a message to the queue.
 *
 *  @param[in] m is the message to send
 *
 *  @return This method returns 0 on success and -1 on error.
 */
extern int uid_send_message( struct MW_UID_MESSAGE *m );

/**
 *  This method registers the device associated with @a fd to
 *  to insert data to the queue
 */
extern int uid_register_device( int fd, const char *q_name );

/* unregister device to stop adding messages to the queue */
extern int uid_unregister_device( int fd );

/* set the keyboard */
extern int uid_set_kbd_mode( int fd, int mode, int *old_mode );

/**
 *  This methods prints the specified UID message using printk
 *
 *  @param[in] uid points to the message to print
 */
void uid_print_message(
  struct MW_UID_MESSAGE *uid
);

/**
 *  This methods prints the specified UID message using your fprintf
 *  style method of choice.
 *
 *  @param[in] context is a pointer to a data area which may be
 *             used by some print handlers
 *  @param[in] handler is the fprintf style method to invoke
 *  @param[in] uid points to the message to print
 */
void uid_print_message_with_plugin(
  void                  *context,
  rtems_printk_plugin_t  handler,
  struct MW_UID_MESSAGE *uid
);

#ifdef __cplusplus
}
#endif

#endif /* _MW_UID_H */
