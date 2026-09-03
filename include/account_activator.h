#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USERNAME_ENTITY_NUMBER        0x7800200
#define USERNAME_ENTITY_NUMBER_2      0x7940200
#define ACCOUNT_ID_ENTITY_NUMBER      0x7800500
#define ACCOUNT_ID_ENTITY_NUMBER_2    0x7940500
#define ACCOUNT_TYPE_ENTITY_NUMBER    0x780b007
#define ACCOUNT_TYPE_ENTITY_NUMBER_2  0x794b007
#define ACCOUNT_FLAGS_ENTITY_NUMBER   0x7800800
#define ACCOUNT_FLAGS_ENTITY_NUMBER_2 0x7940800
#define ACCOUNT_TYPE_MAX              17
#define ACCOUNT_FLAGS_VALUE           4098

/*
 * Returns 0 on success.
 * changed = 1 when the account was changed, 0 when it was already active.
 * error_code is the exact SCE error code for SCE calls, or one of our
 * negative local errors (documented in account_activator.c).
 */
int account_activator_run(char *username,
                          uint32_t username_size,
                          uint64_t *account_id,
                          int *changed,
                          int *error_code);

#ifdef __cplusplus
}
#endif
