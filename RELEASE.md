# Release checklist

## 1. Upload

Upload the contents of this folder to the root of your GitHub repository. Keep the `.github/workflows/` directory exactly as-is.

## 2. Build check

Open **Actions** and select **Build PS5 ELF**. Click **Run workflow**.

A successful run produces the artifact:

- `EPINOR-Account-Activator`

containing:

- `EPINOR-Account-Activator.elf`
- `SHA256SUMS.txt`

## 3. First console test

Before publishing a release, download the generated ELF artifact and test it on the target PS5 payload loader.

Expected successful notification:

```text
Account Activator

Account activated successfully
User: <current user>

Coded by EPINOR
```

When already activated:

```text
Account Activator

Account is already activated
User: <current user>

Coded by EPINOR
```

## 4. Publish release

Create and push a semantic version tag, for example:

```bash
git tag v1.0.0
git push origin v1.0.0
```

The **Release PS5 ELF** workflow will build the tagged commit and create the GitHub Release with the ELF and SHA-256 checksum attached.
