#include "notification.h"

#include <string.h>

extern int sceKernelSendNotificationRequest(int device,
                                            OrbisNotificationRequest *req,
                                            size_t size,
                                            int blocking);

int send_notification(const char *message)
{
    OrbisNotificationRequest req;
    memset(&req, 0, sizeof(req));

    req.use_icon_image_uri = 1;
    req.target_id = -1;
    (void)strncpy(req.message,
                  message != NULL ? message : "",
                  sizeof(req.message) - 1);
    (void)strncpy(req.uri,
                  "cxml://psnotification/tex_icon_system",
                  sizeof(req.uri) - 1);

    return sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
}
