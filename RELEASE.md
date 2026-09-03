# Release procedure

1. Upload the contents of this directory to the **root** of your GitHub repository (not inside another folder).
2. Make sure the workflow path is exactly `.github/workflows/ps5.yml` and that this file is on the default branch.
3. Commit and push to `main`.
4. Open **Actions → PS5 Account Activator → Build ELF** and run it manually, or wait for the push-triggered build.
5. Download the `EPINOR-Account-Activator` artifact and test the ELF on your target PS5 payload loader.
6. After the device test succeeds, create and push a tag:

```sh
git tag v1.0.0
git push origin v1.0.0
```

The same workflow will create the GitHub Release and attach:

- `EPINOR-Account-Activator.elf`
- `SHA256SUMS.txt`

Expected success notification:

```text
Account Activator

Account activated successfully
User: <current user>

Coded by EPINOR
```

Expected already-active notification:

```text
Account Activator

Account is already activated
User: <current user>

Coded by EPINOR
```
