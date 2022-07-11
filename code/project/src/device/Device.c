#include "gd32f3x0.h"
#include "string.h"
#include "stdlib.h"
#include "device.h"
#include "ugservice.h"


#ifdef RT_USING_DEVICE_OPS
#define device_init     (dev->ops->init)
#define device_open     (dev->ops->open)
#define device_close    (dev->ops->close)
#define device_read     (dev->ops->read)
#define device_write    (dev->ops->write)
#define device_control  (dev->ops->control)
#else
#define device_init     (dev->init)
#define device_open     (dev->open)
#define device_close    (dev->close)
#define device_read     (dev->read)
#define device_write    (dev->write)
#define device_control  (dev->control)
#endif




void *ug_memset(void *s, int c, ug_ubase_t count)
{

#define LBLOCKSIZE      (sizeof(ug_int32_t))
#define UNALIGNED(X)    ((ug_int32_t)X & (LBLOCKSIZE - 1))
#define TOO_SMALL(LEN)  ((LEN) < LBLOCKSIZE)

    int i;
    char *m = (char *)s;
    ug_uint32_t buffer;
    ug_uint32_t *aligned_addr;
    ug_uint32_t d = c & 0xff;

    if (!TOO_SMALL(count) && !UNALIGNED(s))
    {
        /* If we get this far, we know that n is large and m is word-aligned. */
        aligned_addr = (ug_uint32_t *)s;

        /* Store D into each char sized location in BUFFER so that
         * we can set large blocks quickly.
         */
        if (LBLOCKSIZE == 4)
        {
            buffer = (d << 8) | d;
            buffer |= (buffer << 16);
        }
        else
        {
            buffer = 0;
            for (i = 0; i < LBLOCKSIZE; i ++)
                buffer = (buffer << 8) | d;
        }

        while (count >= LBLOCKSIZE * 4)
        {
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            count -= 4 * LBLOCKSIZE;
        }

        while (count >= LBLOCKSIZE)
        {
            *aligned_addr++ = buffer;
            count -= LBLOCKSIZE;
        }

        /* Pick up the remainder with a bytewise loop. */
        m = (char *)aligned_addr;
    }

    while (count--)
    {
        *m++ = (char)d;
    }

    return s;

#undef LBLOCKSIZE
#undef UNALIGNED
#undef TOO_SMALL

}


void *ug_memcpy(void *dst, const void *src, ug_ubase_t count)
{

#define UNALIGNED(X, Y)                                               \
                        (((ug_int32_t)X & (sizeof(ug_int32_t) - 1)) | \
                         ((ug_int32_t)Y & (sizeof(ug_int32_t) - 1)))
#define BIGBLOCKSIZE    (sizeof(ug_int32_t) << 2)
#define LITTLEBLOCKSIZE (sizeof(ug_int32_t))
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)

    char *dst_ptr = (char *)dst;
    char *src_ptr = (char *)src;
    ug_int32_t *aligned_dst;
    ug_int32_t *aligned_src;
    int len = count;

    /* If the size is small, or either SRC or DST is unaligned,
    then punt into the byte copy loop.  This should be rare. */
    if (!TOO_SMALL(len) && !UNALIGNED(src_ptr, dst_ptr))
    {
        aligned_dst = (ug_int32_t *)dst_ptr;
        aligned_src = (ug_int32_t *)src_ptr;

        /* Copy 4X long words at a time if possible. */
        while (len >= BIGBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            len -= BIGBLOCKSIZE;
        }

        /* Copy one long word at a time if possible. */
        while (len >= LITTLEBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            len -= LITTLEBLOCKSIZE;
        }

        /* Pick up any residual with a byte copier. */
        dst_ptr = (char *)aligned_dst;
        src_ptr = (char *)aligned_src;
    }

    while (len--)
        *dst_ptr++ = *src_ptr++;

    return dst;
#undef UNALIGNED
#undef BIGBLOCKSIZE
#undef LITTLEBLOCKSIZE
#undef TOO_SMALL

}


void *ug_memmove(void *dest, const void *src, ug_ubase_t n)
{
    char *tmp = (char *)dest, *s = (char *)src;

    if (s < tmp && tmp < s + n)
    {
        tmp += n;
        s += n;

        while (n--)
            *(--tmp) = *(--s);
    }
    else
    {
        while (n--)
            *tmp++ = *s++;
    }

    return dest;
}


ug_int32_t ug_memcmp(const void *cs, const void *ct, ug_ubase_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;

    return res;
}

ug_size_t ug_strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc) /* nothing */
        ;

    return sc - s;
}

char *ug_strstr(const char *s1, const char *s2)
{
    int l1, l2;

    l2 = ug_strlen(s2);
    if (!l2)
        return (char *)s1;
    l1 = ug_strlen(s1);
    while (l1 >= l2)
    {
        l1 --;
        if (!ug_memcmp(s1, s2, l2))
            return (char *)s1;
        s1 ++;
    }

    return UG_NULL;
}

ug_uint32_t ug_strcasecmp(const char *a, const char *b)
{
    int ca, cb;

    do
    {
        ca = *a++ & 0xff;
        cb = *b++ & 0xff;
        if (ca >= 'A' && ca <= 'Z')
            ca += 'a' - 'A';
        if (cb >= 'A' && cb <= 'Z')
            cb += 'a' - 'A';
    }
    while (ca == cb && ca != '\0');

    return ca - cb;
}


char *ug_strncpy(char *dst, const char *src, ug_ubase_t n)
{
    if (n != 0)
    {
        char *d = dst;
        const char *s = src;

        do
        {
            if ((*d++ = *s++) == 0)
            {
                /* NUL pad the remaining n-1 bytes */
                while (--n != 0)
                    *d++ = 0;
                break;
            }
        } while (--n != 0);
    }

    return (dst);
}

ug_int32_t ug_strncmp(const char *cs, const char *ct, ug_ubase_t count)
{
    register signed char __res = 0;

    while (count)
    {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
        count --;
    }

    return __res;
}

ug_int32_t ug_strcmp(const char *cs, const char *ct)
{
    while (*cs && *cs == *ct)
        cs++, ct++;

    return (*cs - *ct);
}

ug_size_t ug_strnlen(const char *s, ug_ubase_t maxlen)
{
    const char *sc;

    for (sc = s; *sc != '\0' && sc - s < maxlen; ++sc) /* nothing */
        ;

    return sc - s;
}


enum ug_object_info_type
{
    UG_Object_Info_Device,

    UG_Object_Info_Unknown,
};

#define _OBJ_CONTAINER_LIST_INIT(c)     \
    {&(ug_object_container[c].object_list), &(ug_object_container[c].object_list)}
static struct ug_object_information ug_object_container[UG_Object_Info_Unknown] =
{
    /* initialize object container - device */
    {UG_Object_Class_Device, _OBJ_CONTAINER_LIST_INIT(UG_Object_Info_Device), sizeof(struct ug_device)},

};

struct ug_object_information *
ug_object_get_information(enum ug_object_class_type type)
{
    int index;

    for (index = 0; index < UG_Object_Info_Unknown; index ++)
        if (ug_object_container[index].type == type) return &ug_object_container[index];

    return UG_NULL;
}

long list(void)
{

    return 0;
}

ug_device_t ug_device_find(const char *name)
{
    struct ug_object *object;
    struct ug_list_node *node;
    struct ug_object_information *information;


    /* try to find device object */
    information = ug_object_get_information(UG_Object_Class_Device);
    configASSERT(information != UG_NULL);
    for (node  = information->object_list.next;
            node != &(information->object_list);
            node  = node->next)
    {
        object = ug_list_entry(node, struct ug_object, list);
        if (ug_strncmp(object->name, name, UG_NAME_MAX) == 0)
        {
            return (ug_device_t)object;
        }
    }

    /* not found */
    return UG_NULL;
}
#ifdef UG_USING_HOOK
static void (*ug_object_attach_hook)(struct ug_object *object);
static void (*ug_object_detach_hook)(struct ug_object *object);
void (*ug_object_trytake_hook)(struct ug_object *object);
void (*ug_object_take_hook)(struct ug_object *object);
void (*ug_object_put_hook)(struct ug_object *object);


void ug_object_attach_sethook(void (*hook)(struct ug_object *object))
{
    ug_object_attach_hook = hook;
}

void ug_object_detach_sethook(void (*hook)(struct ug_object *object))
{
    ug_object_detach_hook = hook;
}

void ug_object_trytake_sethook(void (*hook)(struct ug_object *object))
{
    ug_object_trytake_hook = hook;
}

void ug_object_take_sethook(void (*hook)(struct ug_object *object))
{
    ug_object_take_hook = hook;
}

void ug_object_put_sethook(void (*hook)(struct ug_object *object))
{
    ug_object_put_hook = hook;
}

/**@}*/
#endif

void ug_object_init(struct ug_object         *object,
                    enum ug_object_class_type type,
                    const char               *name)
{
    struct ug_object_information *information;

    /* get object information */
    information = ug_object_get_information(type);
    configASSERT(information != UG_NULL);

    /* initialize object's parameters */

    /* set object type to static */
    object->type = type | UG_Object_Class_Static;

    /* copy name */
    ug_strncpy(object->name, name, UG_NAME_MAX);

    UG_OBJECT_HOOK_CALL(ug_object_attach_hook, (object));

    /* insert object into information object list */
    ug_list_insert_after(&(information->object_list), &(object->list));

}

ug_err_t ug_device_register(ug_device_t dev,
                            const char *name,
                            ug_uint16_t flags)
{
    if (dev == UG_NULL)
        return -UG_ERROR;

    if (ug_device_find(name) != UG_NULL)
        return -UG_ERROR;

    ug_object_init(&(dev->parent), UG_Object_Class_Device, name);
    dev->flag = flags;
    dev->ref_count = 0;
    dev->open_flag = 0;


    return UG_EOK;
}

ug_bool_t ug_object_is_systemobject(ug_object_t object)
{
    /* object check */
    configASSERT(object != UG_NULL);

    if (object->type & UG_Object_Class_Static)
        return UG_TRUE;

    return UG_FALSE;
}

/**
 * This function will return the type of object without
 * RT_Object_Class_Static flag.
 *
 * @param object the specified object to be get type.
 *
 * @return the type of object.
 */
ug_uint8_t ug_object_get_type(ug_object_t object)
{
    /* object check */
    configASSERT(object != UG_NULL);

    return object->type & ~UG_Object_Class_Static;
}

void ug_object_detach(ug_object_t object)
{

    /* object check */
    configASSERT(object != UG_NULL);

    UG_OBJECT_HOOK_CALL(ug_object_detach_hook, (object));

    /* reset object type */
    object->type = 0;

    /* remove from old list */
    ug_list_remove(&(object->list));
}

ug_err_t ug_device_unregister(ug_device_t dev)
{
    configASSERT(dev != UG_NULL);
    configASSERT(ug_object_get_type(&dev->parent) == UG_Object_Class_Device);
    configASSERT(ug_object_is_systemobject(&dev->parent));

    ug_object_detach(&(dev->parent));

    return UG_EOK;
}

ug_device_t ug_device_create(int type, int attach_size)
{
    int size;
    ug_device_t device;

    size = UG_ALIGN(sizeof(struct ug_device), UG_ALIGN_SIZE);
    attach_size = UG_ALIGN(attach_size, UG_ALIGN_SIZE);
    /* use the totoal size */
    size += attach_size;

    device = (ug_device_t)malloc(size);
    if (device)
    {
        ug_memset(device, 0x0, sizeof(struct ug_device));
        device->type = (enum ug_device_class_type)type;
    }

    return device;
}

void ug_device_destroy(ug_device_t dev)
{
    configASSERT(dev != UG_NULL);
    configASSERT(ug_object_get_type(&dev->parent) == UG_Object_Class_Device);
    configASSERT(ug_object_is_systemobject(&dev->parent) == UG_FALSE);

    ug_object_detach(&(dev->parent));

    /* release this device object */
    free(dev);
}

ug_err_t ug_device_init(ug_device_t dev)
{
    ug_err_t result = UG_EOK;

    configASSERT(dev != UG_NULL);

    /* get device init handler */
    if (device_init != UG_NULL)
    {
        if (!(dev->flag & UG_DEVICE_FLAG_ACTIVATED))
        {
            result = device_init(dev);
            if (result != UG_EOK)
            {

            }
            else
            {
                dev->flag |= UG_DEVICE_FLAG_ACTIVATED;
            }
        }
    }

    return result;
}

ug_err_t ug_device_open(ug_device_t dev, ug_uint16_t oflag)
{
    ug_err_t result = UG_EOK;

    configASSERT(dev != UG_NULL);
    configASSERT(ug_object_get_type(&dev->parent) == UG_Object_Class_Device);

    /* if device is not initialized, initialize it. */
    if (!(dev->flag & UG_DEVICE_FLAG_ACTIVATED))
    {
        if (device_init != UG_NULL)
        {
            result = device_init(dev);
            if (result != UG_EOK)
            {
                return result;
            }
        }

        dev->flag |= UG_DEVICE_FLAG_ACTIVATED;
    }

    /* device is a stand alone device and opened */
    if ((dev->flag & UG_DEVICE_FLAG_STANDALONE) &&
            (dev->open_flag & UG_DEVICE_OFLAG_OPEN))
    {
        return -UG_EBUSY;
    }

    /* call device open interface */
    if (device_open != UG_NULL)
    {
        result = device_open(dev, oflag);
    }
    else
    {
        /* set open flag */
        dev->open_flag = (oflag & UG_DEVICE_OFLAG_MASK);
    }

    /* set open flag */
    if (result == UG_EOK || result == -UG_ENOSYS)
    {
        dev->open_flag |= UG_DEVICE_OFLAG_OPEN;

        dev->ref_count++;
        /* don't let bad things happen silently. If you are bitten by this assert,
         * please set the ref_count to a bigger type. */
        configASSERT(dev->ref_count != 0);
    }

    return result;
}

ug_err_t ug_device_close(ug_device_t dev)
{
    ug_err_t result = UG_EOK;

    configASSERT(dev != UG_NULL);
    configASSERT(ug_object_get_type(&dev->parent) == UG_Object_Class_Device);

    if (dev->ref_count == 0)
        return -UG_ERROR;

    dev->ref_count--;

    if (dev->ref_count != 0)
        return UG_EOK;

    /* call device close interface */
    if (device_close != UG_NULL)
    {
        result = device_close(dev);
    }

    /* set open flag */
    if (result == UG_EOK || result == -UG_ENOSYS)
        dev->open_flag = UG_DEVICE_OFLAG_CLOSE;

    return result;
}

ug_size_t ug_device_read(ug_device_t dev,
                         ug_off_t    pos,
                         void       *buffer,
                         ug_size_t   size)
{
    configASSERT(dev != UG_NULL);
    configASSERT(ug_object_get_type(&dev->parent) == UG_Object_Class_Device);

    if (dev->ref_count == 0)
    {
        return 0;
    }

    /* call device read interface */
    if (device_read != UG_NULL)
    {
        return device_read(dev, pos, buffer, size);
    }

    return 0;
}

ug_size_t ug_device_write(ug_device_t dev,
                          ug_off_t    pos,
                          const void *buffer,
                          ug_size_t   size)
{
    configASSERT(dev != UG_NULL);
    configASSERT(ug_object_get_type(&dev->parent) == UG_Object_Class_Device);

    if (dev->ref_count == 0)
    {
        return 0;
    }

    /* call device write interface */
    if (device_write != UG_NULL)
    {
        return device_write(dev, pos, buffer, size);
    }


    return 0;
}

ug_err_t ug_device_control(ug_device_t dev, int cmd, void *arg)
{
    configASSERT(dev != UG_NULL);
    configASSERT(ug_object_get_type(&dev->parent) == UG_Object_Class_Device);

    /* call device write interface */
    if (device_control != UG_NULL)
    {
        return device_control(dev, cmd, arg);
    }

    return -UG_ENOSYS;
}


