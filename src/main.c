#include "account_activator.h"
#include "notification.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern int sceUserServiceInitialize(const uint32_t *init_param);
extern int sceUserServiceTerminate(void);

#define USERNAME_BUFFER_SIZE 100
#define MESSAGE_BUFFER_SIZE  1200

static void make_error_message(char *out,
                               size_t out_size,
                               const char *username,
                               int error_code)
{
    (void)snprintf(out,
                    out_size,
                    "Account Activator\n\n"
                    "Activation failed\n"
                    "User: %s\n"
                    "Error: 0x%08X\n\n"
                    "Coded by EPINOR",
                    (username != NULL && username[0] != '\0') ? username : "Unknown",
                    (uint32_t)error_code);
}

int main(void)
{
    char username[USERNAME_BUFFER_SIZE] = {0};
    char message[MESSAGE_BUFFER_SIZE] = {0};
    uint64_t account_id = 0;
    int changed = 0;
    int error_code = 0;
    int ret;

    ret = sceUserServiceInitialize(NULL);
    if (ret != 0) {
        make_error_message(message, sizeof(message), username, ret);
        (void)send_notification(message);
        return 1;
    }

    ret = account_activator_run(username,
                                sizeof(username),
                                &account_id,
                                &changed,
                                &error_code);

    if (ret == 0) {
        (void)snprintf(message,
                       sizeof(message),
                       "Account Activator\n\n"
                       "%s\n"
                       "User: %s\n\n"
                       "Coded by EPINOR",
                       changed ? "Account activated successfully"
                               : "Account is already activated",
                       username[0] != '\0' ? username : "Unknown");
    } else {
        make_error_message(message,
                           sizeof(message),
                           username,
                           error_code != 0 ? error_code : ret);
    }

    (void)send_notification(message);
    (void)sceUserServiceTerminate();

    return ret == 0 ? 0 : 1;
}
