#ifndef CONVERTOR_ENUM_H
#define CONVERTOR_ENUM_H
typedef enum ConversionMode
{
    NoneConversion,
    Flip,
    Uppercase,
    EnumEnd
} ConversionMode_t;

typedef void (*conversion_mode_changed_cb_t)(ConversionMode_t);
#endif