# EPINOR PS5 Account Activator

A small one-shot PS5 ELF payload that activates the current foreground user account using the same registry-based activation approach used by established PS5 offline-account activation projects.

## Behavior

The payload:

1. Initializes `SceUserService`.
2. Gets the foreground user and username.
3. Locates that user's account registry slot.
4. Reads the existing Account ID.
5. Leaves an already-active account untouched.
6. Otherwise generates the Account ID, writes account type `np`, and writes flags `4098`.
7. Sends a device notification with the result and `Coded by EPINOR`.

It does not enable Remote Play, export/import saves, or online PSN gaming.

## Build

This project uses the `ps5-payload-dev/sdk` Makefile toolchain.

```sh
export PS5_PAYLOAD_SDK=/opt/ps5-payload-sdk
make clean all
```

Output:

```text
EPINOR-Account-Activator.elf
```

## GitHub Actions

`.github/workflows/ps5.yml` checks out SDK v0.42, builds the SDK into the runner tool-cache, builds the payload, verifies the resulting ELF, and uploads an artifact.

Pushing a tag such as `v1.0.0` also publishes the ELF and SHA-256 checksum as a GitHub Release.

## License / attribution

GPL-3.0-or-later. The account activation logic is derived from GPL-licensed etaHEN / LightningMods and PS5Dev / ps5-payload-dev/offact sources.
