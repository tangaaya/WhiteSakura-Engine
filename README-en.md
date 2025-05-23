### [Chinese (Simplified)](README.md)
### [English](README-en.md)

# White Sakura Engine / WhiteSakura-Engine

A visual novel engine still under development (maybe?)
Written entirely in C++, currently CLI-only
Author is a hobbyist, updates when free
If you want to seriously develop a visual novel, choose Ren'Py or Kirikiri as this project can barely do anything yet

## Implemented Features
1. Read scripts from files (index.wst)
2. Basic save/load system (save.wst)
3. View historical dialogue
4. Multi-line narrative support (single protagonist/multi-heroine or vice versa) with multiple endings
5. Abbreviation support in script files to reduce input workload

## Limitations
1. No images/sound support (temporary - still essentially a framework)
2. No GUI (CUI optimization in progress)

## Advantages
1. Extremely minimal with zero redundant components
2. Leverages C++ for minimal resource usage
3. Cross-platform compatibility

## **Place runtime files in the compiled executable directory**

## index.wst File Format
- First line: Integer indicating total script length (excluding header)
- Subsequent lines follow format: [Character] [Dialogue] [Type]
  - Character: String (use "0" for narration)
  - Dialogue: String content
  - Type: Integer (0 for normal dialogue)

## save.wst File Format
- Two lines with 15 integers each (space-separated), representing saved positions in index.wst (1-based index, 0 = no save)
- Second line: Save affection values
- Note: 
  - Old saves without second line initialize affection to 0
  - New saves enforce two-line format

## indexque.wst File Format
[Identifier] [Answer1] [Num] [Answer2] [Num] [Answer3] [Num]
- In index.wst, lines with "&" in character names trigger questions
- Identifier column uses the &-prefixed character name
- Output sequence:
  1. Display question
  2. Answer1
  3. Answer2
  4. Answer3 (user input prompt: "Select a lowercase letter")
- Selections (a/b/c) trigger corresponding answer and modify `feelnum` variable
- Affection changes follow specified values for each answer

## indexcon.wst File Format (Chapter Control)
[Type] [Count] [Name] [StartLine]
- Type: a=chapter, b=act
- Count: Sequential number
- Name: String identifier
- StartLine: Line count in index.wst (script line identifier)
- Function: `jumpscreen` displays transition screen between acts/chapters
  - Shows "Next scenario"
  - Displays current chapter/act numbers and names
  - Waits 5 seconds before resuming

## muitlending.wst File Format (Multiple Endings)
- First line: Number of endings
- Subsequent lines: `[AffRangeStart] [AffRangeEnd] [EndingStartLine] [EndingEndLine]`
- When triggered:
  - Checks current `feelnum` against ranges
  - Jumps to specified line if within range
  - Ends game if ending statement is reached

## name.wst File Format (Abbreviation Table)
- First line: Integer count
- Subsequent lines: `[DisplayName] [Abbreviation]`
- Used to convert abbreviations in index.wst to full names during display

## Interested in this project?
Contact me: 1015239807@qq.com
