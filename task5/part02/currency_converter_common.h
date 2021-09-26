#ifndef CURRENCY_CONVERTER_COMMON_H
#define CURRENCY_CONVERTER_COMMON_H

typedef void (*conversion_value_changed_cb)(char*);
typedef void (*conversion_factor_changed_cb)(int);
typedef void (*conversion_result_ready_cb)(int,int);
#endif