[#ftl]
#ifndef FLEXIBLE_BUTTON_CONF_H__
#define FLEXIBLE_BUTTON_CONF_H__

#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

[#list SWIPdatas as SWIP]

[#if SWIP.defines??]
    [#list SWIP.defines as definition]
#define ${definition.name} #t#t (${definition.value})
    [/#list]
[/#if]

#ifdef __cplusplus
}
#endif
#endif /* FLEXIBLE_BUTTON_CONF_H__ */

[/#list]
