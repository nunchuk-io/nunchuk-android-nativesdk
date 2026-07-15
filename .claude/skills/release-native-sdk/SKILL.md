---
name: release-native-sdk
description: >-
  Release nunchuk-android-nativesdk. The team develops on GitLab and mirrors to
  GitHub once per release; that push to GitHub `main` triggers GitHub Actions,
  which builds the reproducible .aar (linux/amd64, both arm ABIs) and publishes
  it to the prebuild repo + JitPack. Use when the user wants to "release the
  native SDK", "publish the aar", "cut a nativesdk version", or "push to jitpack".
---

# Release nunchuk-android-nativesdk

## TL;DR — the release IS a push to GitHub `main`

A push to **GitHub `main`** triggers `.github/workflows/ci.yml` (native amd64):
build the reproducible `.aar` (both arm ABIs) → on `main`, `upload_aar.py`
uploads it to `nunchuk-android-nativesdk-prebuild` + creates tag `VERSION` →
**JitPack** builds that tag. Consumers use
`com.github.nunchuk-io:nunchuk-android-nativesdk-prebuild:<VERSION>`.

### Per-release checklist
1. **Bump `def VERSION`** in `build.gradle` (MUST change every release — publish runs on every `main` push; an existing tag makes it re-upload then fail on `create_git_release`). `upload_aar.py` is now idempotent on the release step, but the version still must be new for JitPack to serve fresh bytes.
2. **Bump `libnunchuk` submodule** to latest `main` AND confirm that commit is on the **GitHub mirror** `nunchuk-io/libnunchuk` (CI pulls the pinned commit from the mirror).
3. **Bindings gate (critical):** a libnunchuk bump only adds C++ APIs — the app also needs the matching **Kotlin+JNI bindings on `main`**. If the app's release code calls new APIs (e.g. Liquid/USDT), those bindings may live on a **feature branch** (e.g. `usdt`), not `main`. Merge them in first (see Pitfall F). A plain-`main` release will *build* but the app's `compileReleaseKotlin` fails with `Unresolved reference`.
4. **Update release notes** — source of truth is the **Slack release-notes thread** (channel `C01QW8XJDPT`); **always read the whole thread**, not just the top message (replies often correct the version / wording). Mirror the agreed bullets into: nativesdk `release_note.md` (nativesdk GitHub release body), and the app's `distribution/whatsnew/whatsnew-en-US` (Play "what's new" + the app GitHub release body).
5. **Verify BEFORE publishing** — push HEAD to a **slash-free** GitHub branch (`git push origin-github main:refs/heads/verify-<ver>`). CI builds on any branch but only publishes on `main`, so a green **"Build the sdk"** confirms it compiles with zero publish side-effects. ⚠️ The trigger is `branches: ["*"]` and `*` matches ONE path segment — a slashed name like `ci/verify` silently does NOT trigger.
6. **Publish:** push `main` to GitLab **and** GitHub (see topology below).
7. **VALIDATE the published aar** (don't trust JitPack's green — see Pitfall E): download it and `unzip -t` (must be a real zip, not base64 text) + confirm a new API is in `classes.jar`.
8. **Bump the app** `prebuildNativeSdk` in `nunchuk-android/gradle/libs.versions.toml` + bump `versionCode` (versionName may stay). Push GitLab `main`, then GitHub `master` to trigger the AAB→Play draft.
9. **Tag + GitHub binary release for the app:** tag `android.<versionName>` on both remotes, then publish the universal APK + signed checksums with `nunchuk-android/scripts/publish-github-release.sh` (see "App GitHub binary release" below).

## Automation status — the only required human step is the Play Console rollout
Steps 1–9 are fully automatable and have all been exercised end-to-end. Drive them in one go on "release"; the operator's **only required manual action is Play Console → review & roll out the production draft** (CI intentionally uploads `status: draft`, not `completed`). Creds live in gitignored files (`githubToken`/`GPGpass` in `local.properties`, `nunchuk-service-account.json`); the app GitHub release is now a **zero-arg** `./scripts/publish-github-release.sh` (auto-derives VERSION/VERSION_CODE from `nunchuk-app/build.gradle.kts`, auto-reads creds).

Only two things can *interrupt* automation, both exceptional (not per-release):
- **Bindings gate (step 3):** when the libnunchuk bump changes/adds public C++ APIs, the Kotlin+JNI bindings must be written by hand (Pitfall A/F). This is **auto-detected** by the verify-branch build (step 5) — a red build stops the release for a human; a no-API-change bump sails through.
- **JitPack cached failure (Pitfall E):** a transient failed build is cached and must be deleted on jitpack.io by the repo owner (web UI). Rare.

Soft/optional: **release notes** come from the Slack thread (channel `C01QW8XJDPT`, read the whole thread) — copy the agreed bullets into `release_note.md` + `distribution/whatsnew/whatsnew-en-US`; no wording judgment needed once posted. `versionCode` can be auto-picked as `max(existing on Play)+1` to avoid the "already used" reject.

Policy note: the **publish push (step 6)** and the app **AAB→Play push (step 8)** are irreversible/outward-facing; proceed only under the user's standing "automate everything" authorization, and always gate on a green verify build first.

## Repo topology (IMPORTANT — differs from old docs)
- `origin` = GitLab, `origin-github` = GitHub (nativesdk). App repo: `origin` = GitLab, `github_origin` = GitHub (HTTPS — push via the `git@github.com:` SSH URL instead, HTTPS has no creds locally).
- **The live line is `main` on BOTH remotes.** GitLab `master` is stale (stuck ~1.1.85), so **`push_code.py` (which mirrors `master`) is NOT usable** — push `main` directly to both remotes:
  ```bash
  git push origin main           # GitLab (source of truth)
  git push origin-github main     # GitHub  → triggers the release
  ```
- App repo mirror: GitLab `main` ↔ GitHub `master` (note the name flip); the AAB auto-builds on GitHub `master`.

## One-time setup (already done)
- Secret `NATIVESDK_PUBLISH_TOKEN` (PAT, write to prebuild + nativesdk) is set.
- Auto-publish (`ci.yml` + `upload_aar.py`) is merged to `main`.
- `upload_aar.py` base64 bug is fixed (see Pitfall D).

## Verify then publish (commands)
```bash
L=src/main/native/libnunchuk
git -C $L fetch origin main && git -C $L checkout origin/main
git -C $L submodule update --init --recursive          # nested submodules
git -C $L remote add github git@github.com:nunchuk-io/libnunchuk.git 2>/dev/null; git -C $L fetch github main
git -C $L merge-base --is-ancestor $(git -C $L rev-parse HEAD) github/main && echo "mirror synced ✓"
git add $L
# ... bump VERSION, release_note.md, ensure bindings on main, commit ...
git push origin-github main:refs/heads/verify-$(grep -oP 'def VERSION = "\K[^"]+' build.gradle)   # build-only check
# once green:
git push origin main && git push origin-github main
```
Verify the artifact (after CI + JitPack):
```bash
B=https://jitpack.io/com/github/nunchuk-io/nunchuk-android-nativesdk-prebuild/<VER>
curl -sL "$B/nunchuk-android-nativesdk-prebuild-<VER>.pom" -o /dev/null -w "%{http_code}\n"  # 200 = built
curl -sL "$B/...-<VER>.aar" -o v.aar; file v.aar; unzip -t v.aar >/dev/null && echo "real zip"
unzip -p v.aar classes.jar | LC_ALL=C grep -a -c "<someNewApiName>"   # >0 = bindings present
```

## Pitfalls (each of these bit us — check them)

**A. libnunchuk bump breaks the JNI compile.** New libnunchuk changes C++ signatures (e.g. `TxOutput` became a struct not a pair; per-chain servers → `set_electrum_servers`/`set_liquid_servers`; block/connection listeners gained a `bool liquid`; `GetHot*Mnemonic`→`GetUnbackedUp*`; `get_user_outputs`/`get_receive_outputs`/`get_change_index` removed → reconstruct from `TxOutput.isReceive/isChange/userAmount`). The JNI in `src/main/native/*-jni.cpp` + `deserializer.cpp` must be adapted. The verify-branch build catches this.

**B. New native dependency: libwally-core.** Recent libnunchuk links `libwally` as a prebuilt static lib at `contrib/libwally-core/build/${ANDROID_ABI}/libwallycore.a`. `.install_deps.sh` cross-compiles it per ABI (`installWally`) with **`--with-pic`** (mandatory — it links into a shared `.so`; else `relocation R_AARCH64_* against wally_* ; recompile with -fPIC`). `--enable-elements` stays on (default) for Liquid; do NOT use `--enable-standard-secp` (incompatible with elements). wally's secp symbols resolve against **bitcoin's** secp — `contrib/bitcoin` is secp256k1-zkp with all elements modules on.

**C. Dockerfile needs `python` for wally's configure.** wally's `./configure` (AX_PYTHON_DEVEL) needs a `python` binary with working `distutils`. Trixie ships Python 3.13 (no distutils) and no `python` symlink → Dockerfile installs `python-is-python3 python3-setuptools` (setuptools' vendored `_distutils` satisfies `import distutils`).

**D. `upload_aar.py` base64 corruption (fixed — keep it fixed).** Pass the **raw aar bytes** to PyGithub's `create_file`/`update_file` — PyGithub base64-encodes for the API itself, so pre-encoding stores base64 *text* as the file → JitPack serves a non-zip → app fails with `AarResourcesCompilerTransform`/`AndroidManifest.xml`. Symptom: the "aar" starts with `UEsDBBQ` (base64 of `PK`) and is ~4/3 the real size.

**E. JitPack's green ✓ is misleading + it caches failures.** "Get it" only means `mvn install-file` ran; it doesn't validate the aar (that's how base64 corruption shipped in 1.2.16). ALWAYS validate (step 7). Also, a transient Maven-Central blip on JitPack (`Could not transfer metadata … repo.maven.apache.org` → `No plugin found for prefix 'install'`) fails the build and JitPack **caches** it — re-requesting serves the cached error. The **repo owner must delete that version's build on jitpack.io** to force a clean rebuild (the uploaded aar is fine).

**F. App-required bindings may live on a feature branch.** The Liquid/USDT bindings (createLiquidWallet, getAddressAssets, get{USDT,LBTC}AssetId, WalletType.LIQUID, 3-arg getAddressPath, liquid tx/listener params, getMnemonicFromUnbackedUp{Wallet,Key}) were on `usdt`, not `main`. Merge the feature branch into `main`: take its **binding files wholesale** (Kotlin `nativelib/*.kt` + the `*-jni.cpp` it changed) — JNI method-name↔Kotlin-extern mismatches are silent **runtime** crashes, not compile errors, so don't line-merge them. Keep `main`'s build infra + libnunchuk pointer + fixed `upload_aar.py`; bump VERSION; re-run the verify-branch build (feature branch may target an older libnunchuk).

**G. Version reuse / JitPack cache.** Can't cleanly re-fix a bad version in place (JitPack caches the build; the prebuild tag points at the bad file). Cut a fresh VERSION instead.

## App consumption (`nunchuk-android`)
- Modern branches (2.6.1+, Kotlin DSL): version catalog `gradle/libs.versions.toml` → `prebuildNativeSdk = "<VER>"`. The `NativeSdkConventionPlugin` builds the coordinate and appends `@aar` — so put the **bare** version in the catalog (NOT `<VER>@aar`, which would double). `nativeSdk` (separate) is the maven debug dep.
- Older branches (2.2): `configs/dependencies.gradle` → `prebuildNativeSdkVersion = '<VER>@aar'`.
- Play publish: app `ci.yml` builds+signs `bundleProductionRelease` and (if `PLAY_SERVICE_ACCOUNT_JSON` secret set) uploads a **production draft** via `r0adkll/upload-google-play` with notes in `distribution/whatsnew/`. `versionCode` must strictly increase per upload or Play rejects it (`Version code NNN has already been used`) — bump it again (a prior CI re-trigger may have already consumed the value). versionName can stay the same across rebuilds.
- **Tag the app release:** lightweight tag `android.<versionName>` (e.g. `android.2.7.0`) on the released commit, pushed to **both** remotes (GitLab `origin` + GitHub via the `git@github.com:` SSH URL). Convention: `git cat-file -t android.2.6.0` → `commit` (lightweight, not annotated).

### App GitHub binary release (universal APK + signed checksums)
Every app GitHub release carries 3 assets matching prior releases: `<versionName>.apk` (~145 MB), `SHA256SUMS` (76 B), `SHA256SUMS.asc` (958 B, GPG clearsigned with the *Ta Tat Tai (Nunchuk binary release signing key)*). Automated by **`nunchuk-android/scripts/publish-github-release.sh`** — now **zero-arg** from the app repo root:
```bash
./scripts/publish-github-release.sh
```
It auto-derives `VERSION`/`VERSION_CODE` from `nunchuk-app/build.gradle.kts`, reads `githubToken`+`GPGpass` from `local.properties` (searches the app repo then `../nunchuk-android-nativesdk/local.properties`), finds `nunchuk-service-account.json`, and sets the **release body from `distribution/whatsnew/whatsnew-en-US`** (override with `RELEASE_BODY`/`RELEASE_BODY_FILE`). Idempotent: re-running updates the notes + re-uploads assets. Any value is still env-overridable.
- **Download the Play-signed APK, do NOT build locally.** With Play App Signing on, the installed APK is signed by Google's **app signing key**, not the local upload key (`nunchuk-keystore.jks`). The script pulls the Google-signed **universal** APK via the Play Developer API `generatedApks.list`/`:download` (needs the `PLAY_SERVICE_ACCOUNT_JSON` service account; `androidpublisher` scope). Fallback: `APK_PATH=<file>` to use a manually-downloaded "Signed, universal APK" from Play Console → App bundle explorer → the versionCode → Downloads.
- Creds locally: `GithubToken` (a repo-write PAT — the nativesdk `local.properties` one works, `JayceLuong` has push) and `GPGpass` live in `local.properties` (gitignored). The APK download waits on Play finishing processing the uploaded AAB for that versionCode.
- **Non-interactive GPG:** this mac has no GUI pinentry (only `pinentry-curses`), so a headless `gpg --clearsign` dies with `Inappropriate ioctl for device`. The script uses `--pinentry-mode loopback --passphrase-fd 0` (passphrase via stdin, not argv). Script is **idempotent/re-runnable**: skips re-download with `APK_PATH`, skips signing if `SHA256SUMS.asc` already verifies. Uses the API (not a push) so it triggers no CI.
- Keep the script **ASCII-only** — a Unicode ellipsis right after `$TAG` was parsed into the variable name under `set -u` (`TAG…: unbound variable`).

## Local fallback (Docker on macOS — prefer CI)
The GHCR builder image is **private** (pull = unauthorized), so build it locally.
Rosetta amd64 emulation **segfaults intermittently** (esp. compiling OpenSSL) —
that's a flake, not a code error; prefer the CI verify-branch. See
[[macos13-rosetta-amd64-blocker]] and [[nativesdk-release-setup]].
```bash
colima start --vm-type vz --vz-rosetta
docker build --platform linux/amd64 -t nunchuk-nativesdk-builder .   # add build deps as a LATE layer to keep SDK/NDK cached
cp local.properties /tmp/lp.bak; printf '\nsdk.dir=/sdk\nndk.dir=/sdk/ndk/27.2.12479018\n' >> local.properties
docker run --rm --platform linux/amd64 -v "$PWD":/nunchuk-android-nativesdk -w /nunchuk-android-nativesdk/src/main/native nunchuk-nativesdk-builder bash ./.install_deps.sh
docker run --rm --platform linux/amd64 -v "$PWD":/nunchuk-android-nativesdk -w /nunchuk-android-nativesdk nunchuk-nativesdk-builder ./gradlew clean assembleArmRelease --stacktrace
cp /tmp/lp.bak local.properties
```
Output: `build/outputs/aar/nunchuk-android-nativesdk-arm-release.aar` (both ABIs); `upload_aar.py` renames to `-arm8-release.aar` on upload (matches `jitpack.yml`).
