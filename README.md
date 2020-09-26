# RiskierRain
A very messy and unfinished mod for Risk of Rain 2 written in C++

Change speed, jump, skill cooldown, etc. Also supports inputting character IDs and playing as any character, including enemies and bosses. This used to work in multiplayer without having to be host, by changing the JIL before it is compiled, but has broken since the 1.0 update.

This requires a DLL injector, which can be written or found easily. This does not use mono injection even though it is a Unity game. The code is very messy and not very modular, and changing character attributes works by traversing the GameObject linked list and finding any valid characters by name.
