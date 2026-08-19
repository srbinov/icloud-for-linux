# iCloud for Linux

[![Snap](https://bit.ly/2ZWfetD)](https://snapcraft.io/icloud-for-linux)

Desktop apps for iCloud and Apple web services on Linux. One Snap package installs each service as its own launcher in your application menu.

These are WebKit wrappers around the official websites, not native Apple clients.

## Apps

| App | Menu name |
| --- | --- |
| Mail | iCloud Mail |
| Contacts | iCloud Contacts |
| Calendar | iCloud Calendar |
| Photos | iCloud Photos |
| iCloud Drive | iCloud Drive |
| Notes | iCloud Notes |
| Reminders | iCloud Reminders |
| Pages | iCloud Pages |
| Numbers | iCloud Numbers |
| Keynote | iCloud Keynote |
| Find My | iCloud Find |
| Maps | Apple Maps |
| TV | Apple TV |

## Install

You need [Snap](https://snapcraft.io/docs/installing-snapd) (it is already installed on Ubuntu).

### From the Snap Store

```bash
sudo snap install icloud-for-linux
```

You can also open **Ubuntu Software** (or another Snap Store client), search for **iCloud for Linux**, and click Install.

### From this repository

Build a local Snap, then install it:

```bash
git clone --recurse-submodules https://github.com/cross-platform/icloud-for-linux.git
cd icloud-for-linux
snapcraft
sudo snap install --dangerous ./icloud-for-linux_*.snap
```

`--dangerous` is required for a Snap that is not signed by the store.

## Using the apps

After install, open your application menu and search for any name in the table above, for example **iCloud Mail** or **Apple Maps**.

You can also launch them from a terminal:

```bash
icloud-for-linux.mail
icloud-for-linux.photos
icloud-for-linux.maps
icloud-for-linux.tv
```

Sign in with your Apple ID the first time. The login is stored on disk so later launches should stay signed in until Apple expires the session or asks for two-factor authentication again.

One sign-in is shared across the iCloud apps.

### Apple Maps location

To allow Maps to use your location:

```bash
sudo snap connect icloud-for-linux:location
```

### Apple TV

The site loads, but Apple TV+ video often needs Safari DRM and may not play in this wrapper.

### China accounts

If your iCloud account uses `.com.cn`, write that TLD into the Snap common directory (once) and relaunch:

```bash
echo -n '.com.cn' > "$HOME/snap/icloud-for-linux/common/tld"
```

## Uninstall

```bash
sudo snap remove icloud-for-linux
```
