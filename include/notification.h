#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Layout used by the PS5 notification request API. */
typedef struct {
    int32_t type;
    int32_t req_id;
    int32_t priority;
    int32_t msg_id;
    int32_t target_id;
    int32_t user_id;
    int32_t unk1;
    int32_t unk2;
    int32_t app_id;
    int32_t error_num;
    int32_t unk3;
    char use_icon_image_uri;
    char message[1024];
    char uri[1024];
    char unkstr[1024];
} OrbisNotificationRequest;

int send_notification(const char *message);

#ifdef __cplusplus
}
#endif
