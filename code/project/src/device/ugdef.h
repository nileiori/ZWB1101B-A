
#ifndef __UG_DEF_H__
#define __UG_DEF_H__


#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif
           


typedef signed   char                   ug_int8_t;    
typedef signed   short                  ug_int16_t;   
typedef signed   long                   ug_int32_t;   
typedef unsigned char                   ug_uint8_t;   
typedef unsigned short                  ug_uint16_t; 
typedef unsigned long                   ug_uint32_t; 
typedef int                             ug_bool_t;    

/* 32bit CPU */
typedef long                            ug_base_t;    
typedef unsigned long                   ug_ubase_t;   

typedef ug_base_t                       ug_err_t;     
typedef ug_uint32_t                     ug_time_t;    
typedef ug_uint32_t                     ug_tick_t;    
typedef ug_base_t                       ug_flag_t;    
typedef ug_ubase_t                      ug_size_t;    
typedef ug_ubase_t                      ug_dev_t;     
typedef ug_base_t                       ug_off_t;     


#define UG_TRUE                         1              
#define UG_FALSE                        0              


#define UG_UINT8_MAX                    0xff           
#define UG_UINT16_MAX                   0xffff         
#define UG_UINT32_MAX                   0xffffffff     
#define UG_TICK_MAX                     UG_UINT32_MAX  

/* Compiler Related Definitions */
#ifdef __CC_ARM                         /* ARM Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define UG_UNUSED                   __attribute__((unused))
    #define UG_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define UG_WEAK                     __weak
    #define ug_inline                   static __inline
    /* module compiling */
    #ifdef UG_USING_MODULE
        #define RTT_API                 __declspec(dllimport)
    #else
        #define RTT_API                 __declspec(dllexport)
    #endif

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  @ x
    #define UG_UNUSED
    #define UG_USED                     __root
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
    #define UG_WEAK                     __weak
    #define ug_inline                   static inline
    #define RTT_API

#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #ifdef RT_USING_NEWLIB
        #include <stdarg.h>
    #else
        /* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list   __gnuc_va_list;
        typedef __gnuc_va_list      va_list;
        #define va_start(v,l)       __builtin_va_start(v,l)
        #define va_end(v)           __builtin_va_end(v)
        #define va_arg(v,l)         __builtin_va_arg(v,l)
    #endif

    #define SECTION(x)                  __attribute__((section(x)))
    #define UG_UNUSED                   __attribute__((unused))
    #define UG_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define UG_WEAK                     __attribute__((weak))
    #define ug_inline                   static __inline
    #define RTT_API
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define UG_UNUSED                   __attribute__((unused))
    #define UG_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define UG_WEAK                     __attribute__((weak))
    #define ug_inline                   static inline
    #define RTT_API
#elif defined (_MSC_VER)
    #include <stdarg.h>
    #define SECTION(x)
    #define UG_UNUSED
    #define UG_USED
    #define ALIGN(n)                    __declspec(align(n))
    #define UG_WEAK
    #define ug_inline                   static __inline
    #define RTT_API
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define SECTION(x)
    #define UG_UNUSED
    #define UG_USED
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)
    #define UG_WEAK
    #define ug_inline                   static inline
    #define RTT_API
#else
    #error not supported tool chain
#endif


/* event length */
#define UG_EVENT_LENGTH                 32

/* memory management option */
#define UG_MM_PAGE_SIZE                 4096
#define UG_MM_PAGE_MASK                 (UG_MM_PAGE_SIZE - 1)
#define UG_MM_PAGE_BITS                 12


/**
 * @addtogroup Error
 */

/*@{*/

/* error code definitions */
#define UG_EOK                          0               /**< There is no error */
#define UG_ERROR                        1               /**< A generic error happens */
#define UG_ETIMEOUT                     2               /**< Timed out */
#define UG_EFULL                        3               /**< The resource is full */
#define UG_EEMPTY                       4               /**< The resource is empty */
#define UG_ENOMEM                       5               /**< No memory */
#define UG_ENOSYS                       6               /**< No system */
#define UG_EBUSY                        7               /**< Busy */
#define UG_EIO                          8               /**< IO error */
#define UG_EINTR                        9               /**< Interrupted system call */
#define UG_EINVAL                       10              /**< Invalid argument */


#define UG_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

#define UG_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define UG_NULL                         (0)

/**
 * Double List structure
 */
struct ug_list_node
{
    struct ug_list_node *next;                          /**< point to next node. */
    struct ug_list_node *prev;                          /**< point to prev node. */
};
typedef struct ug_list_node ug_list_t;                  /**< Type for lists. */

/**
 * Single List structure
 */
struct ug_slist_node
{
    struct ug_slist_node *next;                         /**< point to next node. */
};
typedef struct ug_slist_node ug_slist_t;                /**< Type for single list. */

/**
 * @addtogroup KernelObject
 */

/*@{*/

/*
 * kernel object macros
 */
#define UG_OBJECT_FLAG_MODULE           0x80            /**< is module object. */

/**
 * Base structure of Kernel object
 */
struct ug_object
{
    char       name[UG_NAME_MAX];                       /**< name of kernel object */
    ug_uint8_t type;                                    /**< type of kernel object */
    ug_uint8_t flag;                                    /**< flag of kernel object */

#ifdef UG_USING_MODULE
    void      *module_id;                               /**< id of application module */
#endif
    ug_list_t  list;                                    /**< list node of kernel object */
};
typedef struct ug_object *ug_object_t;                  /**< Type for kernel objects. */

/**
 *  The object type can be one of the follows with specific
 *  macros enabled:
 *  - Thread
 *  - Semaphore
 *  - Mutex
 *  - Event
 *  - MailBox
 *  - MessageQueue
 *  - MemHeap
 *  - MemPool
 *  - Device
 *  - Timer
 *  - Module
 *  - Unknown
 *  - Static
 */
enum ug_object_class_type
{
    UG_Object_Class_Thread = 0,                         /**< The object is a thread. */
    UG_Object_Class_Semaphore,                          /**< The object is a semaphore. */
    UG_Object_Class_Mutex,                              /**< The object is a mutex. */
    UG_Object_Class_Event,                              /**< The object is a event. */
    UG_Object_Class_MailBox,                            /**< The object is a mail box. */
    UG_Object_Class_MessageQueue,                       /**< The object is a message queue. */
    UG_Object_Class_MemHeap,                            /**< The object is a memory heap */
    UG_Object_Class_MemPool,                            /**< The object is a memory pool. */
    UG_Object_Class_Device,                             /**< The object is a device */
    UG_Object_Class_Timer,                              /**< The object is a timer. */
    UG_Object_Class_Module,                             /**< The object is a module. */
    UG_Object_Class_Unknown,                            /**< The object is unknown. */
    UG_Object_Class_Static = 0x80                       /**< The object is a static object. */
};

/**
 * The information of the kernel object
 */
struct ug_object_information
{
    enum ug_object_class_type type;                     /**< object class type */
    ug_list_t                 object_list;              /**< object list */
    ug_size_t                 object_size;              /**< object size */
};

/**
 * The hook function call macro
 */
#ifdef UG_USING_HOOK
#define UG_OBJECT_HOOK_CALL(func, argv) \
    do { if ((func) != UG_NULL) func argv; } while (0)
#else
#define UG_OBJECT_HOOK_CALL(func, argv)
#endif



/*@{*/

/**
 * device (I/O) class type
 */
enum ug_device_class_type
{
    UG_Device_Class_Char = 0,                           /**< character device */
    UG_Device_Class_Block,                              /**< block device */
    UG_Device_Class_NetIf,                              /**< net interface */
    UG_Device_Class_MTD,                                /**< memory device */
    UG_Device_Class_CAN,                                /**< CAN device */
    UG_Device_Class_RTC,                                /**< RTC device */
    UG_Device_Class_Sound,                              /**< Sound device */
    UG_Device_Class_Graphic,                            /**< Graphic device */
    UG_Device_Class_I2CBUS,                             /**< I2C bus device */
    UG_Device_Class_USBDevice,                          /**< USB slave device */
    UG_Device_Class_USBHost,                            /**< USB host bus */
    UG_Device_Class_SPIBUS,                             /**< SPI bus device */
    UG_Device_Class_SPIDevice,                          /**< SPI device */
    UG_Device_Class_SDIO,                               /**< SDIO bus device */
    UG_Device_Class_PM,                                 /**< PM pseudo device */
    UG_Device_Class_Pipe,                               /**< Pipe device */
    UG_Device_Class_Portal,                             /**< Portal device */
    UG_Device_Class_Timer,                              /**< Timer device */
    UG_Device_Class_Miscellaneous,                      /**< Miscellaneous device */
    UG_Device_Class_Unknown                             /**< unknown device */
};

/**
 * device flags defitions
 */
#define UG_DEVICE_FLAG_DEACTIVATE       0x000           /**< device is not not initialized */

#define UG_DEVICE_FLAG_RDONLY           0x001           /**< read only */
#define UG_DEVICE_FLAG_WRONLY           0x002           /**< write only */
#define UG_DEVICE_FLAG_RDWR             0x003           /**< read and write */

#define UG_DEVICE_FLAG_REMOVABLE        0x004           /**< removable device */
#define UG_DEVICE_FLAG_STANDALONE       0x008           /**< standalone device */
#define UG_DEVICE_FLAG_ACTIVATED        0x010           /**< device is activated */
#define UG_DEVICE_FLAG_SUSPENDED        0x020           /**< device is suspended */
#define UG_DEVICE_FLAG_STREAM           0x040           /**< stream mode */

#define UG_DEVICE_FLAG_INT_RX           0x100           /**< INT mode on Rx */
#define UG_DEVICE_FLAG_DMA_RX           0x200           /**< DMA mode on Rx */
#define UG_DEVICE_FLAG_INT_TX           0x400           /**< INT mode on Tx */
#define UG_DEVICE_FLAG_DMA_TX           0x800           /**< DMA mode on Tx */

#define UG_DEVICE_OFLAG_CLOSE           0x000           /**< device is closed */
#define UG_DEVICE_OFLAG_RDONLY          0x001           /**< read only access */
#define UG_DEVICE_OFLAG_WRONLY          0x002           /**< write only access */
#define UG_DEVICE_OFLAG_RDWR            0x003           /**< read and write */
#define UG_DEVICE_OFLAG_OPEN            0x008           /**< device is opened */
#define UG_DEVICE_OFLAG_MASK            0xf0f           /**< mask of open flag */

/**
 * general device commands
 */
#define UG_DEVICE_CTRL_RESUME           0x01            /**< resume device */
#define UG_DEVICE_CTRL_SUSPEND          0x02            /**< suspend device */
#define UG_DEVICE_CTRL_CONFIG           0x03            /**< configure device */

#define UG_DEVICE_CTRL_SET_INT          0x10            /**< set interrupt */
#define UG_DEVICE_CTRL_CLR_INT          0x11            /**< clear interrupt */
#define UG_DEVICE_CTRL_GET_INT          0x12            /**< get interrupt status */

/**
 * special device commands
 */
#define UG_DEVICE_CTRL_CHAR_STREAM      0x10            /**< stream mode on char device */
#define UG_DEVICE_CTRL_BLK_GETGEOME     0x10            /**< get geometry information   */
#define UG_DEVICE_CTRL_BLK_SYNC         0x11            /**< flush data to block device */
#define UG_DEVICE_CTRL_BLK_ERASE        0x12            /**< erase block on block device */
#define UG_DEVICE_CTRL_BLK_AUTOREFRESH  0x13            /**< block device : enter/exit auto refresh mode */
#define UG_DEVICE_CTRL_NETIF_GETMAC     0x10            /**< get mac address */
#define UG_DEVICE_CTRL_MTD_FORMAT       0x10            /**< format a MTD device */
#define UG_DEVICE_CTRL_RTC_GET_TIME     0x10            /**< get time */
#define UG_DEVICE_CTRL_RTC_SET_TIME     0x11            /**< set time */
#define UG_DEVICE_CTRL_RTC_GET_ALARM    0x12            /**< get alarm */
#define UG_DEVICE_CTRL_RTC_SET_ALARM    0x13            /**< set alarm */

#define UG_WAITING_FOREVER              -1              /**< Block forever until get resource. */
#define UG_WAITING_NO                   0               /**< Non-block. */

typedef struct ug_device *ug_device_t;
/**
 * operations set for device object
 */
struct ug_device_ops
{
    /* common device interface */
    ug_err_t  (*init)   (ug_device_t dev);
    ug_err_t  (*open)   (ug_device_t dev, ug_uint16_t oflag);
    ug_err_t  (*close)  (ug_device_t dev);
    ug_size_t (*read)   (ug_device_t dev, ug_off_t pos, void *buffer, ug_size_t size);
    ug_size_t (*write)  (ug_device_t dev, ug_off_t pos, const void *buffer, ug_size_t size);
    ug_err_t  (*control)(ug_device_t dev, int cmd, void *args);
};

/**
 * WaitQueue structure
 */
struct ug_wqueue
{
    ug_uint32_t flag;
    ug_list_t waiting_list;
};
typedef struct ug_wqueue ug_wqueue_t;

/**
 * Device structure
 */
struct ug_device
{
    struct ug_object          parent;                   /**< inherit from rt_object */

    enum ug_device_class_type type;                     /**< device type */
    ug_uint16_t               flag;                     /**< device flag */
    ug_uint16_t               open_flag;                /**< device open flag */

    ug_uint8_t                ref_count;                /**< reference count */
    ug_uint8_t                device_id;                /**< 0 - 255 */

    /* device call back */
    ug_err_t (*rx_indicate)(ug_device_t dev, ug_size_t size);
    ug_err_t (*tx_complete)(ug_device_t dev, void *buffer);

#ifdef UG_USING_DEVICE_OPS
    const struct ug_device_ops *ops;
#else
    /* common device interface */
    ug_err_t  (*init)   (ug_device_t dev);
    ug_err_t  (*open)   (ug_device_t dev, ug_uint16_t oflag);
    ug_err_t  (*close)  (ug_device_t dev);
    ug_size_t (*read)   (ug_device_t dev, ug_off_t pos, void *buffer, ug_size_t size);
    ug_size_t (*write)  (ug_device_t dev, ug_off_t pos, const void *buffer, ug_size_t size);
    ug_err_t  (*control)(ug_device_t dev, int cmd, void *args);
#endif

    void                     *user_data;                /**< device private data */
};


/**
 * sector arrange struct on block device
 */
struct ug_device_blk_sectors
{
    ug_uint32_t sector_begin;                           /**< begin sector */
    ug_uint32_t sector_end;                             /**< end sector   */
};

/**
 * cursor control command
 */
#define UG_DEVICE_CTRL_CURSOR_SET_POSITION  0x10
#define UG_DEVICE_CTRL_CURSOR_SET_TYPE      0x11


#ifdef __cplusplus
}
#endif


#endif




