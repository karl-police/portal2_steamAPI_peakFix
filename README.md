# Portal 2 Steam API Peak Fix
![image](https://github.com/karl-police/portal2_steamAPI_peakFix/assets/12023782/4360ca1d-f983-437c-8218-d83af74ca405)

There's a bug in Portal 2: https://github.com/ValveSoftware/portal2/issues/414<br>
I reported it a long time ago, but I never really knew the exact cause of this issue.

Much later, I started to do precise debugging regarding that issue, and managed to figure out the exact cause.


I don't know if someone at Valve will fix this issue, therefore I created this Plugin that fixes the issue.

&nbsp;

## Explanation
If you experience lag peak issues, it may be caused by that bug. This is because Portal 2 registers a callback from the Steam API, that has the ID 304.
This specific callback is the [``PersonaStateChange``](https://partner.steamgames.com/doc/api/ISteamFriends#GetPersonaState).
> The friend state of the current user. (Online, Offline, In-Game, etc)

Portal 2 would fetch all of your friends and try to retrieve the ``PersonaStateChange`` everytime... for some reason. I think the more friends you have on Steam, the worser the peaks may be.

Blocking callback 304, doesn't seem to cause any issues, and Co-op player search works fine.

&nbsp;
&nbsp;

## Building
I have especially created a **CMake** project, for _global_ building convenience. But I have not much knowledge about it and I failed to do Linux support.

You do not need to build this, as I build and put it in the **Release** section.

There are a lot of build presets. You only need ``x86 Release``, because apparently Portal 2 is in 32 Bit. However, if that's not the case you may choose the right preset that applies to you.

If you do ever need to build it for 64 Bit, you need the ``.lib`` files of every dependency.


### Dependencies
https://github.com/microsoft/Detours - Version 4.0.1
This is used to hook and "override" functions. Perhaps there are other libraries, I used this one.

&nbsp;
&nbsp;

## Installing
1. Simply build or download it from Release
2. Create an ``addons`` folder under ``<YOUR_PORTAL_2_DIRECTORY>/portal2``. Should sorta look like this ``<YOUR_PORTAL_2_DIRECTORY>/portal2/addons``
3. Drag in the files in the ``addons`` folder!

Portal 2 doesn't have VAC and plugins load without ``-insecure`` in the Launch Options, but you can feel free to add that launch parameter.

You're free to re-name the ``.dll`` to whatever you want, just make sure to update the ``.vdf`` file.


&nbsp;
&nbsp;

## Other

Thanks to everyone that has showed me things related to Plugin creation. Especially members from AlliedModders, Source Engine and pajlada's Twitch Chat.
