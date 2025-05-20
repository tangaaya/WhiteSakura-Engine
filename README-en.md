### [简体中文](README.md)
### [English](README-en.md)

# White Sakura Engine / 白樱引擎

A visual novel engine still under development (maybe?)
Completely written in C++, currently command-line only
The author is purely an amateur and updates when free
If you're serious about developing a visual novel, please choose Ren'Py or KRKR instead, as this project currently can't do much

## Currently Implemented
1. Reading scripts from files (index.wst)
2. Basic save/load functionality (save.wst)
3. Viewing dialogue history
4. Single protagonist single-route story playback

## Limitations
1. No images or sound (temporary, so it's not really an engine yet, just a framework)
2. No GUI (might be unfriendly to some users)
3. Limited script playback format (multi-route support will be added later)
Will address these limitations when time permits

## Advantages
1. Extremely concise with no redundant elements
2. Leverages C++ advantages for minimal resource usage
3. Cross-platform with good compatibility

## index.wst Format
First line contains a single integer representing total script length (excluding other content)
Subsequent lines follow the format: [Character] [Content] [Type]
- Character: Name as string, use "0" for narration (don't leave blank)
- Content: Dialogue text as string
- Type: Used later for distinguishing choices from normal dialogue (currently only supports normal dialogue - use 0)
**Must be placed in the compiled program's directory when running**

## save.wst Format
15 space-separated integers corresponding to line numbers in index.wst (excluding first line), starting from 1. 0 indicates no save file, which won't be displayed in save/load screens.
**Must be placed in the compiled program's directory when running**

## If you're interested in this project
Feel free to contact me: 1015239807@qq.com
