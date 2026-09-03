#include "account_activator.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

extern int sceUserServiceGetForegroundUser(int *user_id);
extern int sceUserServiceGetUserName(int user_id, char *username, size_t size);

extern int sceRegMgrGetStr(int entity, char *value, size_t size);
extern int sceRegMgrGetBin(int entity, void *value, size_t size);
extern int sceRegMgrSetInt(int entity, int value);
extern int sceRegMgrSetBin(int entity, const void *value, size_t size);
extern int sceRegMgrSetStr(int entity, const char *value, size_t size);

#define USERNAME_BUFFER_SIZE 100
#define MAX_ACCOUNT_SLOTS    100
#define MAX_USER_INDEX       16
#define REGISTRY_STR_SUCCESS 0

#define ERR_NO_USERNAME      (-1001)
#define ERR_NO_USER          (-1002)
#define ERR_NO_ACCOUNT_SLOT  (-1003)
#define ERR_ZERO_ACCOUNT_ID  (-1004)

static int get_entity_number(int account_number,
                             int per_user_entity,
                             int fallback_entity)
{
    if (account_number < 1 || account_number > MAX_USER_INDEX)
        return fallback_entity;

    return (account_number - 1) * 65536 + per_user_entity;
}

static int find_account_slot(const char *username)
{
    char registry_username[USERNAME_BUFFER_SIZE];

    if (username == NULL || username[0] == '\0')
        return -1;

    for (int i = 0; i < MAX_ACCOUNT_SLOTS; ++i) {
        const int entity = get_entity_number(i,
                                             USERNAME_ENTITY_NUMBER,
                                             USERNAME_ENTITY_NUMBER_2);

        memset(registry_username, 0, sizeof(registry_username));
        const int ret = sceRegMgrGetStr(entity,
                                        registry_username,
                                        sizeof(registry_username));
        if (ret != REGISTRY_STR_SUCCESS)
            continue;

        registry_username[sizeof(registry_username) - 1] = '\0';
        if (strcmp(username, registry_username) == 0)
            return i;
    }

    return -1;
}

static int read_account_id(int account_number, uint64_t *account_id)
{
    const int entity = get_entity_number(account_number,
                                         ACCOUNT_ID_ENTITY_NUMBER,
                                         ACCOUNT_ID_ENTITY_NUMBER_2);

    if (account_id == NULL)
        return ERR_NO_ACCOUNT_SLOT;

    *account_id = 0;
    return sceRegMgrGetBin(entity, account_id, sizeof(*account_id));
}

static uint64_t generate_account_id(const char *username)
{
    uint64_t base = 0x5EAF00DULL / 0xCA7F00DULL;

    if (username != NULL && *username != '\0') {
        do {
            base = 0x100000001B3ULL *
                   (base ^ (uint8_t)*username++);
        } while (*username != '\0');
    }

    return base;
}

int account_activator_run(char *username,
                          uint32_t username_size,
                          uint64_t *account_id,
                          int *changed,
                          int *error_code)
{
    int user_id = -1;
    int account_number = -1;
    int ret;

    if (username == NULL || username_size == 0 || account_id == NULL ||
        changed == NULL || error_code == NULL)
        return -1;

    username[0] = '\0';
    *account_id = 0;
    *changed = 0;
    *error_code = 0;

    ret = sceUserServiceGetForegroundUser(&user_id);
    if (ret != 0) {
        *error_code = ret;
        return ret;
    }

    if (sceUserServiceGetUserName(user_id, username, username_size) != 0) {
        *error_code = ERR_NO_USERNAME;
        return ERR_NO_USERNAME;
    }

    username[username_size - 1] = '\0';
    account_number = find_account_slot(username);
    if (account_number < 0) {
        *error_code = ERR_NO_ACCOUNT_SLOT;
        return ERR_NO_ACCOUNT_SLOT;
    }

    ret = read_account_id(account_number, account_id);
    if (ret != 0) {
        *error_code = ret;
        return ret;
    }

    /* Already activated: leave all registry fields untouched. */
    if (*account_id != 0)
        return 0;

    *account_id = generate_account_id(username);
    if (*account_id == 0) {
        *error_code = ERR_ZERO_ACCOUNT_ID;
        return ERR_ZERO_ACCOUNT_ID;
    }

    const int id_entity = get_entity_number(account_number,
                                            ACCOUNT_ID_ENTITY_NUMBER,
                                            ACCOUNT_ID_ENTITY_NUMBER_2);
    const int type_entity = get_entity_number(account_number,
                                              ACCOUNT_TYPE_ENTITY_NUMBER,
                                              ACCOUNT_TYPE_ENTITY_NUMBER_2);
    const int flags_entity = get_entity_number(account_number,
                                               ACCOUNT_FLAGS_ENTITY_NUMBER,
                                               ACCOUNT_FLAGS_ENTITY_NUMBER_2);
    static const char account_type[ACCOUNT_TYPE_MAX] = "np";

    /* Keep the operation ordered exactly like etaHEN: ID, type, flags. */
    ret = sceRegMgrSetBin(id_entity, account_id, sizeof(*account_id));
    if (ret != 0) {
        *error_code = ret;
        return ret;
    }

    ret = sceRegMgrSetStr(type_entity,
                          account_type,
                          sizeof(account_type));
    if (ret != 0) {
        *error_code = ret;
        return ret;
    }

    ret = sceRegMgrSetInt(flags_entity, ACCOUNT_FLAGS_VALUE);
    if (ret != 0) {
        *error_code = ret;
        return ret;
    }

    *changed = 1;
    return 0;
}
